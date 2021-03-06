@rem @echo off

@rem ===================================================================
@rem 処理内容：
@rem 　階層クラスタリングによるクラスタリング
@rem 呼び出し方：
@rem 　dendrogram.bat 入力ファイル名 出力ファイル名 分割クラス数 左上X座標 左上Y座標 幅 高さ
@rem ===================================================================

@rem カレントディレクトリの保存
set cwd=%~dp0

@rem Anaconda環境（Python）の呼び出し
call C:\Users\hirotec\anaconda3\Scripts\activate.bat
call conda activate hirotec
cd "D:\WeldEvaluation\python"
d:

@rem コマンドの実行
python dendrogram.py --input_file %1 --output_file %2 --num_classes %3 --coord_x %4 --coord_y %5 --width %6 --height %7
echo %ERRORLEVEL% > return.txt

@rem Anaconda環境の解放
call conda deactivate
call conda deactivate

@rem カレントディレクトリを元に戻す
cd %cwd%
d:
