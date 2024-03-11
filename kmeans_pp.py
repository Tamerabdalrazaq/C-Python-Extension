import numpy as np
import pandas as pd
import sys
import math

EPSILON = 1.1
ITER = 300
np.random.seed(0)
error_messages = {
    "k": "Invalid number of clusters!",
    "iter": "Invalid maximum iteration!",
}

# joins two given vectors of length n/2 into one vector of length n
def combine_data(data1, data2):
    df1 = pd.read_csv('data1', header=None)
    df2 = pd.read_csv('data2', header=None)
    df1.iloc[:, 0].name = 'indx'
    df2.iloc[:, 0].name = 'indx'
    df1.set_index('indx', inplace=True)
    df2.set_index('indx', inplace=True)
    result_str = pd.merge(df1, df2, on='indx', how='inner')
    result = result_str.astype(float)
    result_sorted = result.sort_index()
    return result_sorted


# data is a 2D matrix of the dimensions nxd
def kmeansplus(k, n, d, iter, data):
    ind = np.random.choice(n)
    rnd_clus = data[ind]
    clusters = [rnd_clus]
    for i in range(k):  # untill we get k clusters
        sum = 0
        DX_arr = []
        for row in data:
            DX = find_DX(clusters, row)
            sum+= DX
            DX_arr.append(DX)
        prob = [dx/sum for dx in DX_arr]
        new_clust_ind = np.random.choice(n, p=prob)
        clusters.append(data[new_clust_ind])
    return clusters


def euc_l2(v1, v2):
    dist = 0
    for i in range(len(v1)):
        dist += math.pow(v1[i] - v2[i], 2)
    return math.sqrt(dist)


def find_DX(clusters, v):
    DX = math.inf
    for row in clusters:
        distance = euc_l2(row, v)
        if distance < DX:
            DX = distance
    return DX


# needs to be fixed (like the kmeans in project1)
def main():
    k = iter = data1 = data2 = None
    args = sys.argv
    k = args[1]
    
    if  len(sys.argv) >= 5:
        iter = args[2]
        data1 = args[3]
        data2 = args[4]
        if (not iter.isdigit()): return (print(error_messages["iter"]))
        else: iter = int(iter)
        data = combine_data(data1, data2)
    else:
        iter = ITER
        data1 = args[2]
        data2 = args[3]
        data = combine_data(data1, data2)
    k= int(k)
    #checks if the arguments are natural numbers
    if (not k.isdigit()): return (print(error_messages["k"]))

    #checks the validity of the arguments
    if (K<=1 or N<=K): return print(error_messages["K"])
    if (not 1<iter<1000): return print(error_messages["iter"])

    np_arr = data.values
    n = np_arr.shape[0]
    d = np_arr.shape[1]
    kmeansplus(k, n, d, iter, data)


    
