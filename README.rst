.. rst2s5.py --no-compact-list --theme=small-black README.rst README.html

===========================
C言語でPostgreSQLを拡張する
===========================
:Author: 石田朗雄(id:iakio)
:Organization: Cycle of 5th / JPUG北海道支部
:Date: 2010-02-13

本題の前に(1)
===========================

.. class:: incremental

* 去年のLDDで講演された板垣さんがCommitterになりました。

  * http://archives.postgresql.org/pgsql-hackers/2009-12/msg00549.php

* 次のリリースは8.5ではなく9.0になります。

  * http://archives.postgresql.org/pgsql-hackers/2010-01/msg02056.php

本題の前に(2)
===========================

* その他PostgreSQLの最新情報なら

  * http://lets.postgresql.jp/

  * http://postgresql.g.hatena.ne.jp/


概要
===========================

PostgreSQLはC言語で関数、データ型、演算子、
キャスト、その他色々な機能をわりと簡単に拡張することができます。

そもそも組み込みの関数の実装も
拡張モジュールの作りとほとんど一緒です。
(src/backend/utils/adt/)

PostgreSQLのソースに興味がある方は
このあたりから始めてみてはいかがでしょうか。

例えば
======

* PostGIS

  * GIS(地理情報システム)拡張

* contrib/ltree

  * 木構造データ型

* contrib/hstore

  * key -> valueデータ型


何故
========

.. class:: incremental

* ユーザ定義関数くらいであれば、PL/pgSQLやPL/Perl、PL/Python等でも可能

* 速い、軽い

* C言語で書かれたライブラリを利用したい

* 新しいデータ型を定義したい

FAQ
=======

* Q: PostgreSQL本体のソースコードは必要ですか？

* A: 必須ではありません。例えばCentOSなら
  postgresql-develがインストールされていれば
  拡張モジュールのビルドは可能です。
  が、ソースからビルドしたほうが色々楽です。


今回の環境
==========

開発時は、PostgreSQLを$HOMEの下にインストールしてしまうのが楽です(postgresユーザーとか作らない)。

$HOME/pgsql/binにPATHを通すのを忘れずに。

::

  $ ./configure --prefix=$HOME/pgsql
  $ make && make install
  $ export PATH=$HOME/pgsq/bin:$PATH
  $ pg_config --pgxs
  /home/ishida/pgsql/lib/pgxs/src/makefiles/pgxs.mk


手順
=======

.. class:: incremental

1. C言語でモジュールを書く

2. make install → \*.so が $HOME/pgsql/lib に

3. データベースに接続して CREATE FUNCTION

CREATE FUNCTION
===============

::

  CREATE FUNCTION SQL関数名(引数の型)
    RETURNS 戻り値の型
    AS 'MODULE_PATHNAME', 'Cの関数名'
    LANGUAGE 'C' STRICT;

とりあえず作ってみます
======================

出来上がったものがこちらです
============================

http://github.com/iakio/ldd10w

お約束1
=======

* プロトタイプは`Datum 関数名(PG_FUNCTION_ARGS);`

* PG_GETARG_INT32(0);

  * 最初の引数をINT32型として取得

* PG_RETURN_INT32(datum);

  * datumをINT32型として返却

Datum / 値渡しと参照渡し
========================

* 引数も戻り値もDatum

* integerであれば値渡し

  * Datumの中身はinteger

* textであれば参照渡し

  * Datumの値はtext型へのポインタ

* pg_type.typbyvalで定義されている


お題1:最大公約数を求める
========================

::

  =# SELECT gcd(15,6);
   gcd
  -----
     3
  (1 row)

ユークリッドの互除法

お題2:有理数型ratの追加
=======================

::

  =# SELECT '1/2'::rat * '1/3'::rat;
   ?column?
  ----------
   1/6
  (1 row)

手順
==================

.. class:: incremental

1. 内部のデータ構造を決める

2. input, output関数を書く

3. CREATE TYPE


データ構造
==================

* 8byte 

* 参照渡し

::

  typedef struct {
    int32 num;
    int32 denom;
  } Rat;


内部表現と外部表現
==================

::

  内部表現              外部表現
  struct Rat {
    int num;    <====> cstring
    int denom;
  };

  { 1, 3 }      <====> "1/3"


CREATE TYPE
===========

::

  CREATE TYPE name (
    INPUT = input_function,
    OUTPUT = output_function
    [ , INTERNALLENGTH = { internallength | VARIABLE }
    [ , PASSEDBYVALUE ]
    ...
  )


演算子、キャスト
================

* rat_add(rat, rat):rat 関数を定義
 
 * → CREATE OPERAOR

* rat_tofloat8(rat):float8 関数を定義
 
 * → CREATE CAST

さらに、、、
==================

* エラー処理

* DirectFunctionCall

* make installcheck(自動テスト)

* 比較演算子

* 新しいデータ型に対するインデックスの定義

* JPUG北海道支部勉強会でやるかも？


ご静聴ありがとうございました
============================
