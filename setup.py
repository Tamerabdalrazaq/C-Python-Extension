from setuptools import Extension, setup

module = Extension("mykmeanssp", sources=['kmeans.c','kmeansmodule.c'])
setup(name='mykmeanssp',
     version='1.0',
     description='C module implementing K-Means algorithm integrated with Python K-Means_PP algorithm',
     ext_modules=[module])