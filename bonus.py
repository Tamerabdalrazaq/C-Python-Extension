import pandas as pd
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.cluster import KMeans
from matplotlib.patches import Rectangle



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
    plt.title('Elbow Method for selection of optimal "K" clusters')
    plt.annotate('Elbow Points', xy=(3, 300), xytext=(3.3, 310), ha='center', va='center')
    plt.annotate('', xy=(2, inertias[1]), xytext=(3, 300),arrowprops=dict(facecolor='black',arrowstyle='->'))
    plt.annotate('', xy=(3, inertias[2]), xytext=(3, 300),arrowprops=dict(facecolor='black',arrowstyle='->'))
    s1 = Rectangle((1.7, inertias[1]-10), 0.5,30, edgecolor='black', facecolor='none')
    s2 = Rectangle((2.7, inertias[2]-10), 0.5,30, edgecolor='black', facecolor='none')
    axis = plt.gca()
    axis.add_patch(s1)
    axis.add_patch(s2)
    plt.savefig('elbow.png')
    plt.show()

main()
