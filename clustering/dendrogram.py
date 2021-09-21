import argparse
import os
import sys
import traceback

import numpy as np
from scipy.cluster.hierarchy import dendrogram, linkage, fcluster
from sklearn.ensemble import RandomForestClassifier

from utils import get_data, get_bndata, get_fileinfo, get_sumple


#処理を実行する
if __name__ == "__main__":
    #入力ファイル・出力ファイル・クラス数を引数として設定
    parser = argparse.ArgumentParser()
    #入力ファイル
    parser.add_argument("--input_file", type=str)
    #出力ファイル
    parser.add_argument("--output_file", type=str)
    #クラス数
    parser.add_argument("--num_classes", type=int)
    args = parser.parse_args()

    #抽出するデータ数を指定
    num_sample = 2000

    try:
        #引数で与えられた入力ファイルのパスを取得し、ヘッダ情報を取得
        input_file = args.input_file
        bands,data_type,height,width = get_data(input_file)

        #入力ファイルと同名のバイナリファイルからデータを取得
        #入力ファイルからファイル名を取得
        filestem, dirname = get_fileinfo(input_file)
        #拡張子を.rawに変更
        bn_file = os.path.join(dirname, filestem) + '.raw'
        #データの取得
        bdata = get_bndata(bn_file, bands, height, width)
        #データを抽出
        ex_data = get_sumple(bdata, num_sample)

        #階層クラスタリングの実行
        Z = linkage(ex_data, method="ward", metric="euclidean")
        #どのクラスに属しているのかをリストとして取得
        predict = fcluster(Z, t=args.num_classes, criterion="maxclust")
        #random forestを実行
        model = RandomForestClassifier(random_state=1234)
        model.fit(ex_data, predict-1)
        #全データの結果を予測
        predict = model.predict(bdata)
        #分類させた情報を(height,width)の形に整形
        predict_out = predict.reshape(height,width)
        #.csvの出力ファイルを作成
        np.savetxt(args.output_file, predict_out, delimiter=',', fmt='%d')
        print("Done")
    except:
        #エラーの内容を表示
        traceback.print_exc()
        #異常終了
        sys.exit(-1)
    sys.exit()



