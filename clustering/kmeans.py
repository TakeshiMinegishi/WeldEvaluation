import argparse
import os
import sys
import traceback

from sklearn.cluster import KMeans
import numpy as np

#ヘッダファイルから情報を取得する
def get_data(input_file):
    #ヘッダファイルを開く
    with open(input_file,'r') as f:
        datalist = f.readlines()
    #取得した情報から必要な部分を抽出
    for st in datalist:
        #改行を取り除く
        st = st.replace('\n', '')
        #"="の前後で分ける
        st = st.split('=')
        for i, s in enumerate(st):
            #文字列の両端にある余計な文字(スペースなど)を削除
            st[i] = s.strip()

        #得たい情報を以下のifで取得
        #bands
        if st[0] == "bands":
            bands = int(st[1])
        #data_type
        if st[0] == "data type":
            data_type = int(st[1])
        #縦
        if st[0] == "lines":
            height = int(st[1])
        #横
        if st[0] == "samples":
            width = int(st[1])  
    return bands,data_type,height,width

#バイナリデータを取得
def get_bndata(bn_file):
    #バイナリファイルを開く
    with open(bn_file) as f:
        recttype = np.dtype(np.float32)
        bdata = np.fromfile(f,dtype = recttype)
    #nan(not a number)を0に変換
    bdata = np.nan_to_num(bdata)
    #データを-100~100の範囲に収める
    bdata = np.where(bdata < -100, -100, bdata)
    bdata = np.where(bdata > 100, 100, bdata)
    #(bands,height,width)の形に整形
    bdata = bdata.reshape([bands, height, width])
    #(bands,height,width)→(height,width,bands)に変形し、(height*width,bands)の形に直す
    bdata_new = bdata.transpose(1,2,0).reshape(-1,bands)
    return bdata_new

#ファイルの情報を取得する。(ファイル名・ディレクトリ名)
def get_fileinfo(f):
    #ディレクトリを取得
    dirname = os.path.dirname(f)
    #ファイル名を取得
    filename = os.path.basename(f)
    filestem = os.path.splitext(filename)[0]
    return filestem, dirname

#処理を実行する
if __name__ == "__main__":
    #入力ファイル・出力ファイル・クラス数を引数として設定
    parser = argparse.ArgumentParser()
    #入力ファイル
    parser.add_argument("--input_file", type=str, default="./normalized.hdr")
    #出力ファイル
    parser.add_argument("--output_file", type=str, default="./output/output_Kmean.csv")
    #クラス数
    parser.add_argument("--num_classes", type=int, default=3)
    args = parser.parse_args()

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
        bdata = get_bndata(bn_file)

        #Kmeansを実行
        cls = KMeans(n_clusters = args.num_classes)
        #どのクラスに属すのかをリストで取得
        predict = cls.fit_predict(bdata)
        
        #分類させた情報を(height,width)の形に整形
        predict_out = predict.reshape(height,width)
        #.csvの出力ファイルを作成
        np.savetxt(args.output_file, predict_out, delimiter=',', fmt='%d')
    except:
        #エラーの内容を表示
        traceback.print_exc()
        #異常終了
        sys.exit(-1)
    #正常終了
    sys.exit()



