#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
#  TOPPERS Software
#      Toyohashi Open Platform for Embedded Real-Time Systems
# 
#  Copyright (C) 2016-2018 by Embedded and Real-Time Systems Laboratory
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
#  $Id: testexec.rb 608 2018-12-17 00:34:06Z ertl-hiro $
# 

Encoding.default_external = 'utf-8'
require "pp"

#
#  テストプログラム毎に必要なオプションの定義
#
TEST_SPEC = {
  # 機能テストプログラム
  "calsvc"   => { SRC: "test_calsvc" },
  "cpuexc1"  => { SRC: "test_cpuexc1" },
  "cpuexc2"  => { SRC: "test_cpuexc2" },
  "cpuexc3"  => { SRC: "test_cpuexc3" },
  "cpuexc4"  => { SRC: "test_cpuexc4" },
  "cpuexc5"  => { SRC: "test_cpuexc5" },
  "cpuexc6"  => { SRC: "test_cpuexc6" },
  "cpuexc7"  => { SRC: "test_cpuexc7" },
  "cpuexc8"  => { SRC: "test_cpuexc8" },
  "cpuexc9"  => { SRC: "test_cpuexc9" },
  "cpuexc10" => { SRC: "test_cpuexc10" },
  "dlynse"   => { SRC: "test_dlynse" },
  "dtq1"     => { SRC: "test_dtq1" },
  "extsvc1"  => { SRC: "test_extsvc1" },
  "exttsk"   => { SRC: "test_exttsk", CDL: "test_pf_bitkernel" },
  "flg1"     => { SRC: "test_flg1" },
  "hrt1"     => { SRC: "test_hrt1" },
  "int1"     => { SRC: "test_int1" },
  "lmtdom1"  => { SRC: "test_lmtdom1" },
  "lmtdom2"  => { SRC: "test_lmtdom2" },
  "messagebuf1" => { SRC: "test_messagebuf1", CDL: "test_pf_bitkernel" },
  "messagebuf2" => { SRC: "test_messagebuf2", CDL: "test_pf_bitkernel" },
  "mprot1"   => { SRC: "test_mprot1" },
  "mprot2"   => { SRC: "test_mprot2" },
  "mutex1"   => { SRC: "test_mutex1", CDL: "test_pf_bitmutex" },
  "mutex2"   => { SRC: "test_mutex2", CDL: "test_pf_bitmutex" },
  "mutex3"   => { SRC: "test_mutex3", CDL: "test_pf_bitmutex" },
  "mutex4"   => { SRC: "test_mutex4", CDL: "test_pf_bitmutex" },
  "mutex5"   => { SRC: "test_mutex5", CDL: "test_pf_bitmutex" },
  "mutex6"   => { SRC: "test_mutex6", CDL: "test_pf_bitmutex" },
  "mutex7"   => { SRC: "test_mutex7", CDL: "test_pf_bitmutex" },
  "mutex8"   => { SRC: "test_mutex8", CDL: "test_pf_bitmutex" },
  "notify1"  => { SRC: "test_notify1" },
  "raster1"  => { SRC: "test_raster1", CDL: "test_pf_bitkernel" },
  "raster2"  => { SRC: "test_raster2" },
  "sched1"   => { SRC: "test_sched1" },
  "sem1"     => { SRC: "test_sem1" },
  "sem2"     => { SRC: "test_sem2" },
  "suspend1" => { SRC: "test_suspend1" },
  "sysman1"  => { SRC: "test_sysman1" },
  "sysstat1" => { SRC: "test_sysstat1" },
  "sysstat2" => { SRC: "test_sysstat2" },
  "task1"    => { SRC: "test_task1", CDL: "test_pf_bitkernel" },
  "tmevt1"   => { SRC: "test_tmevt1" },
  "tprot1"   => { SRC: "test_tprot1", CDL: "test_pf_bitkernel" },
  "tprot2"   => { SRC: "test_tprot2", CDL: "test_pf_bitkernel" },
  "tprot3"   => { SRC: "test_tprot3", CDL: "test_pf_bitkernel" },
  "tprot4"   => { SRC: "test_tprot4", CDL: "test_pf_bitkernel" },
  "tprot5"   => { SRC: "test_tprot5", CDL: "test_pf_bitkernel" },
  "twdnfy"   => { SRC: "test_twdnfy" },

  # オーバランハンドラ機能拡張パッケージの機能テストプログラム
  "ovrhdr1"  => { SRC: "test_ovrhdr1" },
  "ovrhdr2"  => { SRC: "test_ovrhdr2" },
  "ovrhdr3"  => { TARGET: 1, SRC: "simt_ovrhdr3", DEFS: "-DHRT_CONFIG1" },
  "ovrhdr4"  => { SRC: "test_ovrhdr4" },
  "ovrhdr5"  => { SRC: "test_ovrhdr5" },

  # 制約タスク拡張パッケージの機能テストプログラム
  "rstr1"    => { SRC: "test_rstr1" },
  "rstr2"    => { SRC: "test_rstr2" },

  # サブ優先度機能拡張パッケージの機能テストプログラム
  "subprio1" => { SRC: "test_subprio1" },
  "subprio2" => { SRC: "test_subprio2" },

  # システム時刻管理機能テストプログラム
  "systim1" => { TARGET: 1, SRC: "simt_systim1",
								DEFS: "-DHRT_CONFIG1 -DHOOK_HRT_EVENT" },
  "systim2" => { TARGET: 1, SRC: "simt_systim2",
								DEFS: "-DHRT_CONFIG1 -DHOOK_HRT_EVENT" },
  "systim3" => { TARGET: 1, SRC: "simt_systim3",
								DEFS: "-DHRT_CONFIG1 -DHOOK_HRT_EVENT" },
  "systim4" => { TARGET: 1, SRC: "simt_systim4",
								DEFS: "-DHRT_CONFIG2 -DHOOK_HRT_EVENT" },

  # タイムウィンドウ管理機能のテストプログラム
  "twd1" => { TARGET: 1, SRC: "simt_twd1", DEFS: "-DHRT_CONFIG1" },

  # ドリフト調整機能拡張パッケージのシステム時刻管理機能テストプログラム
  "drift1"   => { TARGET: 1, SRC: "simt_drift1",
								DEFS: "-DHRT_CONFIG1 -DHOOK_HRT_EVENT" },
  "drift1-64ops"  => { TARGET: 1, SRC: "simt_drift1",
				DEFS: "-DHRT_CONFIG1 -DHOOK_HRT_EVENT -DUSE_64BIT_OPS" },
  "systim1-64ops" => { TARGET: 1, SRC: "simt_systim1",
				DEFS: "-DHRT_CONFIG1 -DHOOK_HRT_EVENT -DUSE_64BIT_OPS" },

  # 性能評価プログラム
  "perf0" => { CDL: "perf_pf" },
  "perf1" => { CDL: "perf_pf" },
  "perf2" => { CDL: "perf_pf" },
  "perf3" => { CDL: "perf_pf" },
  "perf4" => { CDL: "perf_pf" },
  "perf5" => { CDL: "perf_pf" },

  "perf0u" => { SRC: "perf0", CFG: "perf0u", CDL: "perf_pf" },
  "perf1u" => { SRC: "perf1", CFG: "perf1u", CDL: "perf_pf" },

  # ライブラリのテスト
  "prbstr"  => { SRC: "test_prbstr" },
}

#
#  カーネルライブラリの作成
#
def BuildKernel
  if !Dir.exist?("KERNELLIB")
    Dir.mkdir("KERNELLIB")
  end

  Dir.chdir("KERNELLIB") do
    puts("== building: KERNELLIB ==")
    configCommand = "ruby #{$usedSrcDir}/configure.rb"
    configCommand += " #{$targetOptions[0]}"
    configCommand += " -f"
    puts(configCommand)
    system(configCommand)
    system("make libkernel.a")
    if File.exist?("Makefile.bak")
      File.delete("Makefile.bak")
    end
  end
end

#
#  テストプログラムの作成
#
def BuildTest(test, testSpec, mkdirFlag=false)
  testName = test.tr("a-z", "A-Z")
  objDir = "OBJ-#{testName}"

  if !Dir.exist?(objDir)
    if mkdirFlag
      Dir.mkdir(objDir)
    else
      return
    end
  end

  Dir.chdir(objDir) do
    puts("== building: #{testName} ==")
    configCommand = "ruby #{$usedSrcDir}/configure.rb"
    if testSpec.has_key?(:TARGET)
      configCommand += " #{$targetOptions[testSpec[:TARGET]]}"
    else
      configCommand += " #{$targetOptions[0]}"
    end
    configCommand += " -a #{$usedSrcDir}/test"

    if (!testSpec.has_key?(:TARGET) || testSpec[:TARGET] == 0)
      configCommand += " -L ../KERNELLIB"
    end
    if testSpec.has_key?(:SRC)
      configCommand += " -A #{testSpec[:SRC]}"
    else
      configCommand += " -A #{test}"
    end
    if testSpec.has_key?(:CFG)
      configCommand += " -c #{testSpec[:CFG]}.cfg"
    end
    if testSpec.has_key?(:CDL)
      configCommand += " -C #{testSpec[:CDL]}.cdl"
    else
      configCommand += " -C test_pf.cdl"
    end
    if testSpec.has_key?(:SYSLIB)
      configCommand += " -S \"" \
			+ testSpec[:SYSLIB].split(/\s+/).map{|f| f+".o"}.join(" ") \
			+ "\""
    end
    if testSpec.has_key?(:APPLOBJ)
      configCommand += " -U \"" \
			+ testSpec[:APPLOBJ].split(/\s+/).map{|f| f+".o"}.join(" ") \
			+ "\""
    end
    if testSpec.has_key?(:DEFS)
      configCommand += " -O \"#{testSpec[:DEFS]}\""
    end
    puts(configCommand)
    system(configCommand)
    system("make")
    if File.exist?("Makefile.bak")
      File.delete("Makefile.bak")
    end
  end
end

#
#  全テストプログラムの作成
#
def BuildAll
  TEST_SPEC.each do |test, testSpec|
    BuildTest(test, testSpec)
  end
end

#
#  テストプログラムの実行
#
def ExecTest(test, testSpec)
  testName = test.tr("a-z", "A-Z")
  objDir = "OBJ-#{testName}"

  return unless Dir.exist?(objDir)

  Dir.chdir(objDir) do
    puts("== executing: #{testName} ==")
    if File.exist?("../TARGET_RUN")
      system(`cat ../TARGET_RUN`)
    else
      system("./hrp")
    end
  end
end

#
#  全テストプログラムの実行
#
def ExecAll
  TEST_SPEC.each do |test, testSpec|
    ExecTest(test, testSpec)
  end
end

#
#  カーネルライブラリのクリーン
#
def CleanKernel
  if Dir.exist?("KERNELLIB")
    Dir.chdir("KERNELLIB") do
      system("make clean")
    end
  end
end

#
#  テストプログラムのクリーン
#
def CleanTest(test, testSpec)
  testName = test.tr("a-z", "A-Z")
  objDir = "OBJ-#{testName}"

  return unless Dir.exist?(objDir)

  Dir.chdir(objDir) do
    system("make clean")
  end
end

#
#  全テストプログラムのクリーン
#
def CleanAll
  TEST_SPEC.each do |test, testSpec|
    CleanTest(test, testSpec)
  end
end

#
#  ソースディレクトリ名を取り出す
#
if /^(.*)\/test\/testexec/ =~ $0
  $srcDir = $1
else
  $srcDir = "."
end

if /^\// =~ $srcDir
  $usedSrcDir = $srcDir
else
  $usedSrcDir = "../" + $srcDir
end

#
#  ターゲット依存のオプションを読む
#
$targetOptions = {}
File.open("TARGET_OPTIONS") do |file|
  file.each_line.with_index do |line, index|
    line.chomp!
    $targetOptions[index] = line
  end
end

#
#  パラメータで指定された処理の実行
#
if ARGV.size == 0
  BuildKernel()
  BuildAll()
  ExecAll()
else
  ARGV.each do |param|
    case param
    when "kernel"
      BuildKernel()
    when "build"
      BuildAll()
    when "exec"
      ExecAll()
    when "all"
      BuildAll()
      ExecAll()
    when "clean_kernel"
      CleanKernel()
    when "clean"
      CleanKernel()
      CleanAll()
    else
      if TEST_SPEC.has_key?(param)
        BuildTest(param, TEST_SPEC[param], true)
        ExecTest(param, TEST_SPEC[param])
      else
        puts("invalid parameter: #{param}")
      end
    end
  end
end
