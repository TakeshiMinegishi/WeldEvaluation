@rem @echo off

@rem ===================================================================
@rem �������e�F
@rem �@k-means�ɂ��N���X�^�����O
@rem �Ăяo�����F
@rem �@kmeans.bat ���̓t�@�C���� �o�̓t�@�C���� �����N���X��
@rem ===================================================================

echo kmeans.bat %1 %2 %3

@rem �J�����g�f�B���N�g���̕ۑ�
set cwd=%~dp0

@rem Anaconda���iPython�j�̌Ăяo��
call C:\Users\hirotec\anaconda3\Scripts\activate.bat
call conda activate hirotec
cd "D:\WeldEvaluation\python"
d:

@rem �R�}���h�̎��s
python kmeans.py --input_file %1 --output_file %2 --num_classes %3
echo %ERRORLEVEL% > return.txt

@rem Anaconda���̉��
call conda deactivate
call conda deactivate

@rem �J�����g�f�B���N�g�������ɖ߂�
cd %cwd%
d:
