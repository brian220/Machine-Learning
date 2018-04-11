#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 22 00:45:29 2017

@author: ubuntu
"""

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
import math
fire_train = []
fire_test = []
fire_train_feature = []
fire_test_feature  = []
f = open('forestfires.csv', 'r')
csv_fire = csv.reader(f)
fire = list(csv_fire)
fire = fire[1:518]
random.shuffle(fire)
#preprocessing
for i in range (0, 517):
    fire[i] = fire[i][4:13]

for i in range (0,517):
    fire[i][:] = [float(x) for x in fire[i]]
#devide into 6 classes
for i in range (0,517):
    if fire[i][8] != 0.0:
        fire[i][8] = int(math.log10(fire[i][8]))
     
fire_train = fire[0:362]
fire_test = fire[362:517]

for i in range (0, 362):
    fire_train_feature.append(fire_train[i][8])
    fire_train[i].pop()

for i in range (0, 155):
    fire_test_feature.append(fire_test[i][8])
    fire_test[i].pop()
    
fire_train = np.array(fire_train)
#decision tree part of fire.data
dec_clf = tree.DecisionTreeClassifier()
dec_clf = dec_clf.fit(fire_train, fire_train_feature)
dec_predict = dec_clf.predict(fire_test)
dec_predict = list(dec_predict)

accuracy = metrics.accuracy_score(fire_test_feature, dec_predict)
print ('forest fire accuracy by the decision tree method:')
print (accuracy)
#kd tree part of .data
kd_clf = neighbors.KNeighborsClassifier(n_neighbors = 5,algorithm = 'kd_tree')
kd_clf = kd_clf.fit(fire_train, fire_train_feature)
kd_predict = kd_clf.predict(fire_test)
kd_predict = list(kd_predict)

accuracy = metrics.accuracy_score(fire_test_feature, kd_predict)
print ('forest fire accuracy by the kd knn method:')
print (accuracy)

#naive bayes
gnb_clf = GaussianNB()
np.array(fire_train)
np.array(fire_train_feature)

gnb_clf = gnb_clf.fit(fire_train, fire_train_feature)
gnb_predict = gnb_clf.predict(fire_test)
gnb_predict = list(gnb_predict)

accuracy = metrics.accuracy_score(fire_test_feature, gnb_predict)
print ('forest fire accuracy by the Gaussian naive bayes method:')
print (accuracy)

mnb_clf = MultinomialNB(alpha=1.0)
np.array(fire_train)
np.array(fire_train_feature)

mnb_clf = mnb_clf.fit(fire_train, fire_train_feature)
mnb_predict = mnb_clf.predict(fire_test)
mnb_predict = list(mnb_predict)

accuracy = metrics.accuracy_score(fire_test_feature, mnb_predict)
print ('forest fire accuracy by the Multinomial naive bayes method:')
print (accuracy)


for i in range (0, 8):
    t = fire_train[:,i]
    t.sort()
    mean = np.mean(t)
    std = np.std(t)
    pdf = st.norm.pdf(t,mean,std)
    plt.subplot(2,4, i + 1)
    plt.title (f'feature {i+1} PDF')
    plt.plot(t,pdf)
plt.tight_layout()
plt.savefig("fire.png",dpi = 600)













