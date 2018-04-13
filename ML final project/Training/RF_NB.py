import math

import pandas

import sklearn
import sklearn.tree
import sklearn.ensemble
import sklearn.model_selection
import sklearn.neighbors 
import sklearn.naive_bayes
import sklearn.preprocessing
import sklearn.metrics
import matplotlib.pyplot as plt
import plot_confusion_matrix as plot_conf
import numpy

def enc_dir(x):
    if x=="N":
        return 0
    else :
        return 1
def enc_speed(x):
    if x>=60:
        if x>110:
            x=110
        return math.floor((x-60)/10)
    else:
        return -1
def get_max_prob_label(x,targets):
    return int(targets[numpy.argmax(x)])

etag_train_frame=pandas.read_csv('D:\\train.csv',index_col=0)
etag_train_frame.loc[:,'direction']=etag_train_frame.loc[:,'direction'].map(enc_dir)
etag_train_feature=etag_train_frame.drop(['hpa','longitude','latitude','speed'],axis=1)
etag_train_target=etag_train_frame.loc[:,'speed'].map(enc_speed)

etag_test_frame=pandas.read_csv('D:\\test.csv',index_col=0)
etag_test_frame.loc[:,'direction']=etag_test_frame.loc[:,'direction'].map(enc_dir)
etag_test_feature=etag_test_frame.drop(['hpa','longitude','latitude','speed'],axis=1)
etag_test_target=etag_test_frame.loc[:,'speed'].map(enc_speed)

#load the iris dataset
#etag_feature_train, etag_feature_test, etag_target_train, etag_target_test =  sklearn.model_selection.train_test_split(etag_feature, etag_target, test_size=0.2)
#split the dataset as required
print("start RF")
etag_rf_clf=sklearn.ensemble.RandomForestClassifier()
etag_rf_clf.fit(etag_train_feature,etag_train_target)
etag_rf_predict=etag_rf_clf.predict(etag_test_feature)
target_label=list(range(5,-2,-1))
cm=sklearn.metrics.confusion_matrix(etag_test_target,etag_rf_predict,labels=target_label)
numpy.set_printoptions(precision=2)
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=False,title='RF Unnormalized confusion matrix')
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=True,title='RF Normalized confusion matrix')
print(etag_rf_clf.score(etag_test_feature,etag_test_target))
print("finish RF")

print("start NB")

etag_nbys_cat=sklearn.naive_bayes.MultinomialNB()
etag_nbys_cont=sklearn.naive_bayes.GaussianNB()

etag_train_feature_cat=etag_train_feature.loc[:,['direction','day']]
#etag_train_feature_cont_=etag_train_feature.drop(['direction','day'],axis=1)
etag_test_feature_cat_=etag_test_feature.loc[:,['direction','day']]
#etag_test_feature_cont_=etag_test_feature.drop(['direction','day'],axis=1)

etag_train_feature_cont_=pandas.get_dummies(etag_train_feature, columns=['day','direction'], sparse=False).drop(['rainfall','speed_limit'],axis=1)
etag_test_feature_cont_=pandas.get_dummies(etag_test_feature, columns=['day','direction'], sparse=False).drop(['rainfall','speed_limit'],axis=1)

etag_nbys_cat.fit(etag_train_feature_cat,etag_train_target)
etag_nbys_cont.fit(etag_train_feature_cont_,etag_train_target)

etag_nbys_cat_prob=etag_nbys_cat.predict_log_proba(etag_test_feature_cat_)
etag_nbys_cont_prob=etag_nbys_cont.predict_log_proba(etag_test_feature_cont_)
#product_prob=numpy.add(etag_nbys_cat_prob,etag_nbys_cont_prob)
product_prob=etag_nbys_cont_prob
print(etag_nbys_cat.classes_)
#etag_nbys_predict=numpy.apply_along_axis(get_max_prob_label,1,product_prob,etag_nbys_cat.classes_)
etag_nbys_predict=etag_nbys_cont.predict(etag_test_feature_cont_)
print(etag_nbys_predict)
target_label=list(range(5,-2,-1))
cm=sklearn.metrics.confusion_matrix(etag_test_target,etag_nbys_predict,labels=target_label)
numpy.set_printoptions(precision=2)
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=False,title='NB Unnormalized confusion matrix')
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=True,title='NB Normalized confusion matrix')
print("finish GNB")
print(etag_nbys_cont.score(etag_test_feature_cont_,etag_test_target))
plt.show()