# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
from sklearn import tree
from sklearn import metrics
from sklearn import neighbors
from sklearn.naive_bayes import GaussianNB
from sklearn.naive_bayes import MultinomialNB
import scipy.stats as st
import matplotlib.pyplot as plt
import numpy as np
import csv
import random

iris_train = []
iris_test = []
iris_train_feature = []
iris_test_feature  = []
f = open('iris.data', 'r')
csv_iris = csv.reader(f)
iris = list(csv_iris)
iris.pop()
random.shuffle(iris)

#preprocessing
for i in range (0, 150):
    if iris[i][4] == 'Iris-setosa':
        iris[i][4] = 0
    elif iris[i][4] == 'Iris-versicolor':
        iris[i][4] = 1
    else:
        iris[i][4] = 2


iris_train = iris[0:105]
iris_test = iris[105:150]

for i in range (0, 105):
    iris_train_feature.append(iris_train[i][4])
    iris_train[i].pop()

for i in range (0, 45):
    iris_test_feature.append(iris_test[i][4])
    iris_test[i].pop()

for i in range (0, 105):
    iris_train[i][:] = [float(x) for x in iris_train[i]]
for i in range (0, 45):   
    iris_test[i][:] = [float(x) for x in iris_test[i]]

iris_train = np.array(iris_train)
#decision tree part of iris.data
dec_clf = tree.DecisionTreeClassifier()
dec_clf = dec_clf.fit(iris_train, iris_train_feature)
dec_predict = dec_clf.predict(iris_test)
dec_predict = list(dec_predict)

accuracy = metrics.accuracy_score(iris_test_feature, dec_predict)
print ('iris accuracy by the decision tree method:')
print (accuracy)

#kd tree part of iris.data
kd_clf = neighbors.KNeighborsClassifier(n_neighbors = 5,algorithm = 'kd_tree')
kd_clf = kd_clf.fit(iris_train, iris_train_feature)
kd_predict = kd_clf.predict(iris_test)
kd_predict = list(kd_predict)

accuracy = metrics.accuracy_score(iris_test_feature, kd_predict)
print ('iris accuracy by the kd knn method:')
print (accuracy)

#naive bayes
gnb_clf = GaussianNB()
np.array(iris_train)
np.array(iris_train_feature)

gnb_clf = gnb_clf.fit(iris_train, iris_train_feature)
gnb_predict = gnb_clf.predict(iris_test)
gnb_predict = list(gnb_predict)

accuracy = metrics.accuracy_score(iris_test_feature, gnb_predict)
print ('iris accuracy by the Gaussian naive bayes method:')
print (accuracy)

mnb_clf = MultinomialNB(alpha=1.0)
np.array(iris_train)
np.array(iris_train_feature)

mnb_clf = mnb_clf.fit(iris_train, iris_train_feature)
mnb_predict = mnb_clf.predict(iris_test)
mnb_predict = list(mnb_predict)

accuracy = metrics.accuracy_score(iris_test_feature, mnb_predict)
print ('iris accuracy by the Multinomial naive bayes method:')

print (accuracy)
for i in range (0, 4):
    t = iris_train[:,i]
    t.sort()
    mean = np.mean(t)
    std = np.std(t)
    pdf = st.norm.pdf(t,mean,std)
    plt.subplot(2,4, i + 1)
    plt.title (f'feature {i + 1} PDF')
    plt.plot(t,pdf)
plt.tight_layout()
plt.savefig("iris.png",dpi = 600)