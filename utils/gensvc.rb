#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
#  TOPPERS Software
#      Toyohashi Open Platform for Embedded Real-Time Systems
# 
#  Copyright (C) 2001 by Embedded and Real-Time Systems Laboratory
#                              Toyohashi Univ. of Technology, JAPAN
#  Copyright (C) 2005-2018 by Embedded and Real-Time Systems Laboratory
#              Graduate School of Information Science, Nagoya Univ., JAPAN
# 
#  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
#  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
#  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
#  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
#      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
#      スコード中に含まれていること．
#  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
#      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
#      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
#      の無保証規定を掲載すること．
#  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
#      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
#      と．
#    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
#        作権表示，この利用条件および下記の無保証規定を掲載すること．
#    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
#        報告すること．
#  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
#      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
#      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
#      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
#      免責すること．
# 
#  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
#  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
#  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
#  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
#  の責任を負わない．
# 
#  $Id: gensvc.rb 524 2018-11-04 13:01:46Z ertl-hiro $
# 

#
#		サービスコールインタフェース生成プログラム（HRP3カーネル用）
#
#  サービスコール定義ファイル（svc.def）を読み込んで，以下のファイルを
#  生成する．
#
#	svc_table.c
#	../include/kernel_fncode.h
#	../include/svc_call.h
#	../arch/gcc/tool_svc.h（GCC用）
#	../arch/shc/kernel_fncode.inc（HEW用）
#	../arch/shc/svc_call.inc（HEW用）

Encoding.default_external = 'utf-8'
require "optparse"

#  オプションの定義
#
#  -kernel <KERNEL>		カーネル名
#  -H					HEW用のファイルを生成する

#
#  グローバル変数の初期化
#
$flag_regexp = Regexp.new("[SP]")		# デフォルトはHRP3カーネル

#
#  オプションの処理
#
OptionParser.new do |opt|
  opt.on("-k KERNEL", "--kernel KERNEL", "kernel profile name") do |val|
    $kernel = val
  end
  opt.on("-H", "--HEW", "generate files for HEW") do
    $genHew = true
  end
  opt.parse!(ARGV)
end

#
#  カーネルオプションの処理
#
case $kernel
when /^asp/
	$flag_regexp = Regexp.new("[S]")
when /^hrp/
	$flag_regexp = Regexp.new("[SP]")
when /^fmp/
	$flag_regexp = Regexp.new("[SM]")
when /^hrmp/
	$flag_regexp = Regexp.new("[SPM]")
end

#
#  グローバル変数の初期化
#
$minFncd = 0;
$svcTable = {};
$condList = {};

#
#  ファイルのオープン
#
$inFileName = ARGV[0] || "svc.def"
begin
  $inFile = File.open($inFileName)
rescue Errno::ENOENT, Errno::EACCES => ex
  abort(ex.message)
end

#
#  エラー出力
#
def error(msg)
  abort("#{msg} at line #{$inFile.lineno}.")
end

def eofError
  abort("Unexpected EOF.")
end

#
#  #ifdef／#endifの生成
#
def generateIfdef(svc, outFile)
  if !(svc[:cond].empty?)
    outFile.puts("#ifdef TOPPERS_SUPPORT_#{svc[:cond]}\n")
  end
end

def generateEndif(svc, outFile)
  if !(svc[:cond].empty?)
    outFile.puts("#endif /* TOPPERS_SUPPORT_#{svc[:cond]} */\n")
  end
end

#
#  定義ファイルの読み込み
#
$nextFncd = -1
while line = $inFile.gets do
  line.chomp!
  line.sub!(/#.*$/, "")			# コメントの削除
  line.rstrip!
  next if line.empty?			# 空行のスキップ

  # 継続行（行末が '\' の行）の処理
  while /\\$/ =~ line
    line.chop!
    if !(line1 = $inFile.gets)
      eofError()
    end
    line1.chomp!
    line1.sub!(/#.*$/, "")			# コメントの削除
    line1.rstrip!
    line += line1
  end

  # サービスコール定義の解析
  if /^(-?[0-9]*)[ \t]+([^ \t]+)[ \t]*(.*)$/ =~ line
    fncd = $1;
    flags = $2;
    proto = $3;
    proto.sub!(/([^ \t])[ \t]+$/, "\1")
    proto.gsub!(/[ \t]+/, " ")
  else
    error("Syntax error")
  end

  # 機能コードをチェック
  if fncd.empty?
    fncd = $nextFncd
  else
    fncd = fncd.to_i
    if fncd != $nextFncd
      STDERR.puts("Function code skipped at line: #{$inFile.lineno}")
    end
  end

  # 次の機能コードの計算
  $nextFncd = fncd - 1

  # 条件の解析
  if !(flags.sub!(/\[([^\]]+)\]/, "").nil?)
    cond = $1
    if !$condList.has_key?(cond)
      $condList[cond] = []
    end
    $condList[cond].push(fncd)
  else
    cond = ""
  end

  # 呼び出すマクロのサフィックスの解析
  if !(flags.sub!(/\<([^<]+)\>/, "").nil?)
    suffix = "_" + $1
  else
    suffix = ""
  end

  # ハッシュの初期化
  svc = $svcTable[fncd] = {}

  # サービスコール以外のスキップ
  if $flag_regexp !~ flags
    if /^L/ =~ flags
      # サービスコール以外の機能（Lフラグ）の処理
      svc[:label] = proto
    else
      # 未サポートのサービスコールの処理
      if /^(\w+)[ \t]+(([a-z_]+)\((.*)\))$/ =~ proto
        svc[:name] = $3
      elsif /^([a-z_]+)$/ =~ proto
        svc[:name] = $1
      end
    end
    next		# スキップ
  end

  # 機能コードの最小値の計算
  $minFncd = fncd if $minFncd > fncd

  # サービスコール情報の登録
  svc[:flags] = flags
  svc[:cond] = cond
  svc[:suffix] = suffix

  # サービスコール定義の解析
  if /^(\w+)[ \t]+(([a-z_]+)\((.*)\))$/ =~ proto
    svc[:type] = $1
    svc[:proto] = $2
    svc[:name] = $3
    svc[:args] = $4.split(/, */)
    svc[:proto].sub!(/\(\)/, "(void)")
  else
    error("Syntax error")
  end

  svc[:args].each do |arg|
    unless /^([\w* ]+)( +|\*)([a-z_]+)$/ =~ arg
      error("Syntax error")
    end
  end
end

#
#	svc_table.cの生成
#
outFile = File.open("svc_table.c", "w")
outFile.puts(<<EOS)
/*  This file is generated from #{$inFileName} by gensvc.rb. */

#include "kernel_impl.h"
#include "omit_svc.h"

static ER
no_support(void)
{
	return(E_NOSPT);
}
EOS

$condList.each_key do |cond|
  outFile.puts
  outFile.puts("#ifndef TOPPERS_SUPPORT_#{cond}")
  $condList[cond].each do |fncd|
    outFile.puts("#define #{$svcTable[fncd][:name]} no_support")
  end
  outFile.puts("#endif /* TOPPERS_SUPPORT_#{cond} */")
end

outFile.puts(<<EOS)

typedef ER_UINT	(*SVC)();

const SVC svc_table[-TMIN_FNCD+1] = {
	(SVC)(no_support),
EOS

# ジャンプテーブルの生成
-1.downto($minFncd).each do |fncd|
  svc = $svcTable[fncd]
  if !(svc[:flags].nil?)
    entry = svc[:name]
  elsif !(svc[:label].nil?)
    entry = svc[:label].upcase
  else
    entry = "no_support"
  end
  outFile.puts("\t(SVC)(#{entry}),")
end
outFile.puts("};")
outFile.close

#
#	kernel_fncode.hの生成
#
outFile = File.open("../include/kernel_fncode.h", "w")
outFile.puts(<<EOS)
/*  This file is generated from #{$inFileName} by gensvc.rb. */

#ifndef TOPPERS_KERNEL_FNCODE_H
#define TOPPERS_KERNEL_FNCODE_H

#define TMIN_FNCD (#{$minFncd})

EOS

$svcTable.each do |fncd, svc|
  if svc.has_key?(:name)
    outFile.puts("#define TFN_#{svc[:name].upcase} (#{fncd})")
  end
end

outFile.puts(<<EOS)

#endif /* TOPPERS_KERNEL_FNCODE_H */
EOS
outFile.close

#
#	svc_call.hの生成
#
# 関数呼出しで呼び出す場合のインタフェース
outFile = File.open("../include/svc_call.h", "w")
outFile.puts(<<EOS)
/*  This file is generated from #{$inFileName} by gensvc.rb. */

#ifndef TOPPERS_SVC_CALL_H
#define TOPPERS_SVC_CALL_H

#ifndef TOPPERS_MACRO_ONLY

EOS

$svcTable.each do |fncd, svc|
  if svc.has_key?(:flags)
    generateIfdef(svc, outFile)
    outFile.puts("extern #{svc[:type]} _kernel_#{svc[:proto]} throw();")
    generateEndif(svc, outFile)
  end
end

outFile.puts(<<EOS)

#endif /* TOPPERS_MACRO_ONLY */

#ifdef TOPPERS_SVC_CALL

EOS

$svcTable.each do |fncd, svc|
  if svc.has_key?(:flags)
    generateIfdef(svc, outFile)
    outFile.puts("#define #{svc[:name]} _kernel_#{svc[:name]}")
    generateEndif(svc, outFile)
  end
end

outFile.puts(<<EOS)

#endif /* TOPPERS_SVC_CALL */
#endif /* TOPPERS_SVC_CALL_H */
EOS
outFile.close

#
#	tool_svc.h（GCC用）の生成
#
# ソフトウェア割込みで呼び出す場合のインタフェース
outFile = File.open("../arch/gcc/tool_svc.h", "w")
outFile.puts(<<EOS)
/*  This file is generated from #{$inFileName} by gensvc.rb. */

#ifndef TOPPERS_TOOL_SVC_H
#define TOPPERS_TOOL_SVC_H

#ifndef TOPPERS_MACRO_ONLY

EOS

$svcTable.each do |fncd, svc|
  next unless svc.has_key?(:flags)

  generateIfdef(svc, outFile)

  # 関数ヘッダの生成
  outFile.puts("Inline #{svc[:type]}")
  outFile.puts(svc[:proto])

  # 関数本体の生成
  outFile.puts("{")

  outFile.print("\tCAL_SVC_#{svc[:args].size}M#{svc[:suffix]}(")
  outFile.print("#{svc[:type]}, TFN_#{svc[:name].upcase}")
  svc[:args].each do |arg|
    if /^([\w* ]+) +([a-z_]+)$/ =~ arg
      outFile.print(", #{$1}, #{$2}")
    elsif /^([\w* ]+\*)([a-z_]+)$/ =~ arg
      outFile.print(", #{$1}, #{$2}")
    end
  end
  outFile.puts(");")

  outFile.puts("}")
  generateEndif(svc, outFile)
  outFile.puts
end

outFile.puts(<<EOS)
#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TOOL_SVC_H */
EOS
outFile.close

#
#	kernel_fncode.incの生成（HEW用）
#
if $genHew
  outFile = File.open("../arch/shc/kernel_fncode.inc", "w")
  outFile.puts(<<EOS)
\;  This file is generated from #{$inFileName} by gensvc.rb.

TMIN_FNCD: .DEFINE "(#{$minFncd})"

EOS

  $svcTable.each do |fncd, svc|
    if svc.has_key?(:name)
      outFile.puts("TFN_#{svc[:name].upcase}: .DEFINE \"(#{fncd})\"")
    end
  end
  outFile.close
end

#
#	svc_call.incの生成（HEW用）
#
if $genHew
  # 関数呼出しで呼び出す場合のインタフェース
  outFile = File.open("../arch/shc/svc_call.inc", "w")
  outFile.puts(<<EOS)
\;  This file is generated from #{$inFileName} by gensvc.rb.

EOS

  $svcTable.each do |fncd, svc|
    if svc.has_key?(:flags)
      outFile.puts("_#{svc[:name]}: .DEFINE \"__kernel_#{svc[:name]}\"")
    end
  end
  outFile.close
end

$inFile.close
