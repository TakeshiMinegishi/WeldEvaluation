@echo off

@rem ===================================================================
@rem �������e�F
@rem �@�K�w�N���X�^�����O�ɂ��N���X�^�����O
@rem �Ăяo�����F
@rem �@dendrogram.bat ���̓t�@�C���� �o�̓t�@�C���� �����N���X��
@rem ===================================================================

@rem �J�����g�f�B���N�g���̕ۑ�
set cwd=%~dp0

@rem Anaconda���iPython�j�̌Ăяo��
call C:\Users\mine\anaconda3\Scripts\activate.bat
call conda activate hirotec
cd "D:\WeldEvaluation\python"
d:

@rem �R�}���h�̎��s
python dendrogram.py --input_file %1 --outpurt_file %2 --num_classes %3
echo %ERRORLEVEL% > return.txt

@rem Anaconda���̉��
call conda deactivate
call conda deactivate

@rem �J�����g�f�B���N�g�������ɖ߂�
cd %cwd%
d:
