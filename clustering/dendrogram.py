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
    #座標
    parser.add_argument("--coord_x", type=int, default=0)
    parser.add_argument("--coord_y", type=int, default=0)
    #有効領域の幅を指定
    parser.add_argument("--width", type=int, default=None)
    #有効領域の高さを指定
    parser.add_argument("--height", type=int, default=None)
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
        #有効領域の左上座標を取得
        xy = (args.coord_x, args.coord_y)
        #幅と高さを取得
        valid_width = args.width if args.width is not None else width
        valid_height = args.height if args.height is not None else height

        #全要素が-1の行列を作成
        result = -np.ones((height, width))

        #データの取得
        valid_data, end_x, end_y = get_bndata(bn_file, bands, height, width, xy, valid_height, valid_width)
        #データを抽出
        ex_data = get_sumple(valid_data, num_sample)

        #階層クラスタリングの実行
        Z = linkage(ex_data, method="ward", metric="euclidean")
        #どのクラスに属しているのかをリストとして取得
        predict = fcluster(Z, t=args.num_classes, criterion="maxclust")
        #random forestを実行
        model = RandomForestClassifier(random_state=1234)
        model.fit(ex_data, predict-1)

        #全データの結果を予測
        predict_all = model.predict(valid_data)

        #分類させた情報を(height,width)の形に整形
        predict_out = predict_all.reshape(valid_height,valid_width)
        #データの統合
        result[xy[1]:end_y, xy[0]:end_x] = predict_out
        #.csvの出力ファイルを作成
        np.savetxt(args.output_file, result, delimiter=',', fmt='%d')
        print("Done")
    except:
        #エラーの内容を表示
        traceback.print_exc()
        #異常終了
        sys.exit(-1)

    #正常終了
    sys.exit()
