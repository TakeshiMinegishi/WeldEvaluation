import os
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
        #lines
        if st[0] == "lines":
            height = int(st[1])
        #samples
        if st[0] == "samples":
            width = int(st[1])  
    return bands,data_type,height,width

#バイナリデータを取得
def get_bndata(bn_file, bands, height, width):
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

#データを抽出する.
def get_sumple(data, num_sample):
    shuffle_data = np.random.permutation(data)
    new_data = shuffle_data[:num_sample,:]
    return new_data
