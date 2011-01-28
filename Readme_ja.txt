About this software:
  This software have been developed to provide a library to use
  scanning range sensors of Hokuyo Automatic Corporation.  Samples
  will help to know how to use them.

Authors:
  Satofumi Kamimura <satofumi@users.sourceforge.net> Design and implementation of the first version.
  Katsumi Kimoto

License:
  Simplified BSD License.
  See COPYRIGHT file.


Mailing list:
  urgwidget-users@lists.sourceforge.net


Library usage:

Visual Studio Solution (Windows)

  urg_library-X.X.X/visual_cpp/urg.sln をビルドします。

  ビルド後は、urg.lib のスタティックライブラリと各サンプルの
  実行ファイルが生成されています。



Visual Studio bat compile (Windows)

*以下の作業は全てコマンドプロンプト上で行ってください*

  1. Visual Studio が提供している設定用bat ファイルをコピーします。

  Microsoft Visual Studio 8\Common7\Tools\vsvars32.bat を
  urgwidget\current\windowsexeにコピーする。

  2. 環境変数を設定します。

  urgwidget\current\windowsexe\vsvars32.batを実行する。

　3. サンプルソースコンパイル用batファイルを実行する。

  urgwidget\current\windowsexe\compile.batを実行する。

>以上の3ステップでurgwidget\current\windowsexeに実行ファイルが生成されます。

*生成される実行ファイルを削除するには…

　urgwidget\current\windowsexe\cleanobj.batを実行すると 
　生成された実行ファイルが削除されます。


gcc (Linux, MinGW)

  必要ならば urg_library-X.X.X/Makefile の PREFIX を編集して
  インストール先を変更します。

  コンパイルとインストールを行います。

  % make
  # make install

  ライブラリの使い方は、urg-library-X.X.X/samples/ 中の Makefile をご覧下さい。
