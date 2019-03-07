#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
#  TOPPERS Software
#      Toyohashi Open Platform for Embedded Real-Time Systems
# 
#  Copyright (C) 2018 by Embedded and Real-Time Systems Laboratory
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
#  $Id: checkextension.rb 501 2018-10-27 07:56:59Z ertl-hiro $
# 

Encoding.default_external = 'utf-8'
require "fileutils"
require "pp"

#
#  リビジョン番号の取り出し
#
def get_revno(fileName)
  id_lines = File.readlines(fileName).grep(/\$Id:.*$/)
  if id_lines.size > 0
    id_lines[0].chomp!
    if revno_strings = id_lines[0].match(/\$Id: [^\s]+ (\d+) /)
      return(revno_strings[1].to_i)
    end
  end
  return(nil)
end

#
#  ファイルのチェック処理
#
def checkFile(fileName, basePath)
  extFileName = basePath + "/" + fileName
  if File.exist?(fileName)
    revno1 = get_revno(fileName)
    revno2 = get_revno(extFileName)
    if !revno1.nil? && !revno2.nil?
      if revno1 > revno2
        print "#{extFileName} has older revision number than #{fileName}.\n"
        print "| #{fileName}: #{revno1}\n"
        print "| #{extFileName}: #{revno2}\n\n"
      end
    else
      mtime1 = File.stat(fileName).mtime
      mtime2 = File.stat(extFileName).mtime
      if (mtime1 > mtime2)
        print "#{extFileName} has older time stamp than #{fileName}.\n"
        print "| #{fileName}: #{mtime1}\n"
        print "| #{extFileName}: #{mtime2}\n\n"
      end
    end
  end
end

#
#  ディレクトリのチェック処理
#
def checkDir(dirName, basePath)
  dirPath = basePath
  dirPath += "/" + dirName if dirName != ""
  Dir::foreach(dirPath) do |fileName|
    next if fileName =~ /^\./
    filePath = fileName
    filePath = dirName + "/" + fileName if dirName != ""
    if File::ftype(dirPath + "/" + fileName) == "directory"
      checkDir(filePath, basePath)
    else
      checkFile(filePath, basePath)
    end
  end
end

#
#  エラーチェック
#
if ARGV.length < 1
  abort("Usage: ruby checkextention.rb <dirlists>")
end

#
#  拡張パッケージ毎に処理する
#
ARGV.each do |arg|
  next unless File::ftype(arg) == "directory"
  extDirName = arg.sub(/\/$/, "")
  checkDir("", extDirName)
end
