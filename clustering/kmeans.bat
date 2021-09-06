@echo off

@rem ===================================================================
@rem 処理内容：
@rem 　k-meansによるクラスタリング
@rem 呼び出し方：
@rem 　kmeans.bat 入力ファイル名 出力ファイル名 分割クラス数
@rem ===================================================================

@rem カレントディレクトリの保存
set cwd=%~dp0

@rem Anaconda環境（Python）の呼び出し
call C:\Users\mine\anaconda3\Scripts\activate.bat
call conda activate JAMSTEC
cd "D:\AnacondaProjectFolder\JAMSTEC\HyperSpectrumCamera"
d:

@rem コマンドの実行
python kmeans.py --input_file %1 --outpurt_file %2 --num_classes %3
echo %ERRORLEVEL% > return.txt

@rem Anaconda環境の解放
call conda deactivate
call conda deactivate

@rem カレントディレクトリを元に戻す
cd %cwd%
d:
