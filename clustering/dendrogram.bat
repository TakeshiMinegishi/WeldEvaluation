@echo off

@rem ===================================================================
@rem 処理内容：
@rem 　階層クラスタリングによるクラスタリング
@rem 呼び出し方：
@rem 　dendrogram.bat 入力ファイル名 出力ファイル名 分割クラス数
@rem ===================================================================

@rem カレントディレクトリの保存
set cwd=%~dp0

@rem Anaconda環境（Python）の呼び出し
call C:\Users\mine\anaconda3\Scripts\activate.bat
call conda activate hirotec
cd "D:\WeldEvaluation\python"
d:

@rem コマンドの実行
python dendrogram.py --input_file %1 --outpurt_file %2 --num_classes %3
echo %ERRORLEVEL% > return.txt

@rem Anaconda環境の解放
call conda deactivate
call conda deactivate

@rem カレントディレクトリを元に戻す
cd %cwd%
d:
