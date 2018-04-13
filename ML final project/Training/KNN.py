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

etag_train_frame=pandas.read_csv('train.csv',index_col=0)
etag_train_frame.loc[:,'direction']=etag_train_frame.loc[:,'direction'].map(enc_dir)
etag_train_feature=etag_train_frame.drop(['hpa','longitude','latitude','speed'],axis=1)
etag_train_target=etag_train_frame.loc[:,'speed'].map(enc_speed)

etag_test_frame=pandas.read_csv('test.csv',index_col=0)
etag_test_frame.loc[:,'direction']=etag_test_frame.loc[:,'direction'].map(enc_dir)
etag_test_feature=etag_test_frame.drop(['hpa','longitude','latitude','speed'],axis=1)
etag_test_target=etag_test_frame.loc[:,'speed'].map(enc_speed)

#load the iris dataset
#etag_feature_train, etag_feature_test, etag_target_train, etag_target_test =  sklearn.model_selection.train_test_split(etag_feature, etag_target, test_size=0.2)
#split the dataset as required
print("start KNN")
etag_knn_clf=sklearn.neighbors.KNeighborsClassifier(n_neighbors = 1,algorithm = 'kd_tree')
etag_knn_clf.fit(etag_train_feature,etag_train_target)
etag_knn_predict=etag_knn_clf.predict(etag_test_feature)
target_label=list(range(5,-2,-1))
cm=sklearn.metrics.confusion_matrix(etag_test_target,etag_knn_predict,labels=target_label)
numpy.set_printoptions(precision=2)
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=False,title='KNN Unnormalized confusion matrix')
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=True,title='KNN Normalized confusion matrix')
print(etag_knn_clf.score(etag_test_feature,etag_test_target))

etag_knn_clf=sklearn.neighbors.KNeighborsClassifier(n_neighbors = 5,algorithm = 'kd_tree')
etag_knn_clf.fit(etag_train_feature,etag_train_target)
etag_knn_predict=etag_knn_clf.predict(etag_test_feature)
target_label=list(range(5,-2,-1))
cm=sklearn.metrics.confusion_matrix(etag_test_target,etag_knn_predict,labels=target_label)
numpy.set_printoptions(precision=2)
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=False,title='KNN Unnormalized confusion matrix')
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=True,title='KNN Normalized confusion matrix')
print(etag_knn_clf.score(etag_test_feature,etag_test_target))
etag_knn_clf=sklearn.neighbors.KNeighborsClassifier(n_neighbors = 10,algorithm = 'kd_tree')
etag_knn_clf.fit(etag_train_feature,etag_train_target)
etag_knn_predict=etag_knn_clf.predict(etag_test_feature)
target_label=list(range(5,-2,-1))
cm=sklearn.metrics.confusion_matrix(etag_test_target,etag_knn_predict,labels=target_label)
numpy.set_printoptions(precision=2)
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=False,title='KNN Unnormalized confusion matrix')
plt.figure()
plot_conf.plot_confusion_matrix(cm, classes=target_label, normalize=True,title='KNN Normalized confusion matrix')
print(etag_knn_clf.score(etag_test_feature,etag_test_target))


plt.show()

