import pandas as pd
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.cluster import KMeans



def main():
    iris = datasets.load_iris()
    X = iris.data
    inertias=[]
    k_vals=[i for i in range(1, 11)]
    for k in range(1,11):
        kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0)
        kmeans.fit(X)
        inertia = kmeans.inertia_
        inertias.append(inertia)
    plt.plot(k_vals,inertias)
    plt.xlabel('K')
    plt.ylabel('Average Dispersion')
    plt.legend('Elbow Method for selection of optimal "K" clusters')
    plt.show()

main()

