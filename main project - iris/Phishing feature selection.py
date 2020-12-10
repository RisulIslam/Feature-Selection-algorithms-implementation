import numpy as np
from sklearn import preprocessing, cross_validation, neighbors
import pandas as pd

import matplotlib.pyplot as plt
import seaborn as sns

from sklearn import metrics
from sklearn import preprocessing, cross_validation, neighbors, svm
from sklearn.preprocessing import StandardScaler

from sklearn.model_selection import KFold
from collections import Counter

import itertools
import heapq

t_size=0.1
n_splits=10



plot_x=[]
plot_y=[]

def forward_selection():
    

    print("\n\n\nForward Selection------------------------------------>>>>>>>>>>>>\n\n\n\n")

    

    total_instance=y.shape[0]
    high=Counter(y).most_common(1)[0][1]
    #print("Highest: ",high)
    default_rate=high/total_instance

    print("Deafult Rate: ",default_rate)
    plot_x.insert(len(plot_x),default_rate)
    plot_y.insert(len(plot_y),0)

    all_feature=[]

    for i in range(1,total_feature+1):
        f="f"
        f+=str(i)
        all_feature.insert(len(all_feature),f)
    print("All features: ",all_feature)


    final_max_accuracy=0
    final_max_accuracy_feature=[]
    current_feature_set=[]
    max_feature_set=[]
    level=1
    for j in range(1,total_feature+1):
        
        max=0
        
        last_max=[]
        
        for f in all_feature:
            
            current_feature_set.insert(len(current_feature_set),f)
            #print("Current feature set: ",current_feature_set)
            X=np.array(df[current_feature_set])
            #print("X modified: ",X[0:5])




            #accurcy
            kf = KFold(n_splits)
            sum = 0
            for train, test in kf.split(X):
                X_train = np.array(X)[train]
                y_train= np.array(y)[train]
                X_test = np.array(X)[test]
                y_test= np.array(y)[test]
                #print("y_train shape:",y_train.shape)
                clf=neighbors.KNeighborsClassifier()
                clf.fit(X_train,y_train)
                sum = (sum+ clf.score(X_test,y_test))
                #print("Sum=",sum)
            average = (sum/n_splits)
            print("    Current Feature set: ",current_feature_set," Accuracy= ",average)
            #print("==>>")
                
            if average>max:
                max=average
                max_feature_set=list(current_feature_set)
                last_max=f
            
            current_feature_set.remove(f)
        print("Current Max Feature Set: ",max_feature_set, "  Accuracy: ",max)
        all_feature.remove(last_max)
        current_feature_set.insert(len(current_feature_set),last_max)

        #print("After iteration all feature set: ",all_feature," current_feature_set: ",current_feature_set)

        plot_x.insert(len(plot_x),max)
        plot_y.insert(len(plot_y),level)
        level+=1
        
        if max>final_max_accuracy:
            final_max_accuracy=max
            final_max_accuracy_feature=list(current_feature_set)
        else:
            print("Warning Accuracy Decreased. May be Local maxima!")

    print("\\n\n\n\nFinal Feature set:  ",final_max_accuracy_feature,"  Accuracy: ",final_max_accuracy)

    plt.figure(figsize=(10,10))
    plt.plot(plot_y,plot_x, linewidth=4, color='red')
    plt.scatter(plot_y,plot_x,c=plot_y,cmap='plasma',s=120)
    plt.xlabel('Feature Set Level for Forward Selection')
    plt.ylabel('Accuracy')
    plt.savefig('Foeward search figure')
    #plt.show()
    plt.close()


plot_xb=[]
plot_yb=[]

def backward_selection():
    
    #Backward selection
    print("\n\n\nBackward Elemination-------------------------------->>>>>>>>>>>>>>\n\n\n")
    

    X=np.array(df.drop(['label'],1))
    total_feature=X.shape[1]
    print("Total Features: ",total_feature)

    all_feature=[]

    for i in range(1,total_feature+1):
        f="f"
        f+=str(i)
        all_feature.insert(len(all_feature),f)
    print("All features: ",all_feature)



    #accurcy
    kf = KFold(n_splits)
    sum = 0
    for train, test in kf.split(X):
        X_train = np.array(X)[train]
        y_train= np.array(y)[train]
        X_test = np.array(X)[test]
        y_test= np.array(y)[test]
        #print("y_train shape:",y_train.shape)
        clf=neighbors.KNeighborsClassifier()
        clf.fit(X_train,y_train)
        sum = (sum+ clf.score(X_test,y_test))
        #print("Sum=",sum)
    average = (sum/n_splits)
    print("Features:",all_feature,"  Accuracy= ",average)
    plot_xb.insert(len(plot_xb),average)
    plot_yb.insert(len(plot_yb),total_feature)

    final_max_accuracy=0
    final_set=[]

    for j in range(total_feature-2,-1,-1):
        

        max_acc=0
        current_max_set=[]
        for i in range(0,len(list(itertools.combinations(all_feature, len(all_feature)-1)))):
            next_set=list(itertools.combinations(all_feature, len(all_feature)-1))
            #print("Combination: \n\n",next_set)
            #print("Total COmbination: ",len(next_set))
            X=np.array(df[list(next_set[i])])
            #print("Dataset: \n\n\n",M)


            
            #accurcy
            kf = KFold(n_splits)
            sum = 0
            for train, test in kf.split(X):
                X_train = np.array(X)[train]
                y_train= np.array(y)[train]
                X_test = np.array(X)[test]
                y_test= np.array(y)[test]
                #print("y_train shape:",y_train.shape)
                clf=neighbors.KNeighborsClassifier()
                clf.fit(X_train,y_train)
                sum = (sum+ clf.score(X_test,y_test))
                #print("Sum=",sum)
            average = (sum/n_splits)
            print("    Current Feature set: ",next_set[i],"  Accuracy= ",average)
            #print("==>>")

            if(average>max_acc):
                max_acc=average
                current_max_set=next_set[i]
        print("\n\nCurrent Max feature set: ",current_max_set," Accuracy: ",max_acc)
        extra=list(set(all_feature) - set(current_max_set))[0]
        #print("Discarded Feature: ",extra)
        all_feature.remove(extra)
        #print("New All feature: ",all_feature)
        plot_xb.insert(len(plot_xb),max_acc)
        plot_yb.insert(len(plot_yb),j+1)
        
        if(max_acc>=final_max_accuracy):
            final_set=current_max_set
            final_max_accuracy=max_acc
        else:
            print("Warning Accuracy Decreased. May be Local maxima!")
    print("\n\n\nFinal Feature Set: ",final_set," Accuracy: ",final_max_accuracy)

    total_instance=y.shape[0]
    high=Counter(y).most_common(1)[0][1]
    #print("Highest: ",high)
    default_rate=high/total_instance
    plot_xb.insert(len(plot_xb),default_rate)
    plot_yb.insert(len(plot_yb),0)



    plt.figure(figsize=(10,10))
    #print(plot_yb)
    plt.plot(plot_yb,plot_xb, linewidth=4, color='red')
    plt.scatter(plot_yb,plot_xb,c=plot_yb,cmap='plasma',s=120)
    plt.xlabel('(Total Feature - Feature Set Level) for Backward Elemination')
    plt.ylabel('Accuracy')
    plt.savefig('Backward search figure')
    #plt.show()
    plt.close()



plot_xo=[]
plot_yo=[]


def original_search():
    
    print("\n\n\nOriginal search-------------------------------->>>>>>>>>>>>>>\n\n\n")
    

    X=np.array(df.drop(['label'],1))
    total_feature=X.shape[1]
    print("Total Features: ",total_feature)
    total_instance=y.shape[0]
    high=Counter(y).most_common(1)[0][1]
    #print("Highest: ",high)
    default_rate=high/total_instance
    plot_xo.insert(len(plot_xo),default_rate)
    plot_yo.insert(len(plot_yo),0)

    all_feature=[]

    for i in range(1,total_feature+1):
        f="f"
        f+=str(i)
        all_feature.insert(len(all_feature),f)
    print("All feature: ",all_feature)
    print("\n\n\n")

    #correlation priority
    heap = []
    for i in range(0,len(all_feature)):
        
        a=[all_feature[i]]
        a.insert(len(a),'label')
        #print("Feature each: ",a)
        data = df[a]
        correlation = data.corr(method='pearson')
        val=abs(np.array(correlation['label'])[0])
        #print("Correlation Coefficient between label and ",f, " is: ",val)

        heapq.heappush(heap, (val, [all_feature[i]]))

    print("Feature  Correlation Coefficient")
    for i in range(0,len(heap)):
        val=heap[len(heap)-(i+1)][1][0]
        key=heap[len(heap)-(i+1)][0]
        print(val,"       ",key)
    current=[]
    final=[]
    max=0
    for i in range(0,total_feature):
        sorted_feature=heap[len(heap)-(i+1)][1][0]
        #print(sorted_feature)
        current.insert(len(current),sorted_feature)
        #print(current)

        X=np.array(df[list(current)])
            #print("Dataset: \n\n\n",M)


            
            #accurcy
        kf = KFold(n_splits)
        sum = 0
        for train, test in kf.split(X):
            X_train = np.array(X)[train]
            y_train= np.array(y)[train]
            X_test = np.array(X)[test]
            y_test= np.array(y)[test]
            #print("y_train shape:",y_train.shape)
            clf=neighbors.KNeighborsClassifier()
            clf.fit(X_train,y_train)
            sum = (sum+ clf.score(X_test,y_test))
            #print("Sum=",sum)
        average = (sum/n_splits)
        print("Current Max Features set: ",current,"  Accuracy= ",average)
        plot_xo.insert(len(plot_xo),average)
        plot_yo.insert(len(plot_yo),i+1)
        
        if(average>=max):
            #print("Updated  max: ", max," Average: ",average)
            max=average
            final.insert(len(final),current[len(current)-1])    
        else:
            print("    Warning Accuracy Decreased. May be Local maxima!")
            
    print("\n\n\nFinal feature set: ", final," Accuracy: ",max)

    plt.figure(figsize=(10,10))
    #print(plot_yb)
    plt.plot(plot_yo,plot_xo, linewidth=4, color='red')
    plt.scatter(plot_yo,plot_xo,c=plot_yo,cmap='plasma',s=120)
    plt.xlabel('Feature Set Level for Backward Elemination')
    plt.ylabel('Accuracy')
    plt.savefig('Original search figure ')
    #plt.show()
    plt.close()

def showallgraph():
    plt.figure(figsize=(10,10))
    plt.plot(plot_yo,plot_xo, linewidth=4, color='red',label='Modified Algorithm')
    plt.scatter(plot_yo,plot_xo,c=plot_yo,cmap='plasma',s=120)
    plt.plot(plot_yb,plot_xb, linewidth=4, color='green',label='Backward Selection')
    plt.scatter(plot_yb,plot_xb,c=plot_yb,cmap='plasma',s=120)
    plt.plot(plot_y,plot_x, linewidth=4, color='blue',label='Forward Selection')
    plt.scatter(plot_y,plot_x,c=plot_y,cmap='plasma',s=120)
    legend = plt.legend(loc='lower center', shadow=True, fontsize='medium')
    legend.get_frame().set_facecolor('#FFFFFF')
    plt.xlabel('Feature Set Level ')
    plt.ylabel('Accuracy')
    plt.savefig('feature selection')
    #plt.show()
    plt.close()

   
while True:
    

    print("The input files are: \n    1. IRIS DATASET\n    2. CS205_SMALLdataset_9\n    3.CS205_BIGdataset_1\n    4. Phishing dataset\n\n")
    inputno=int(input("Enter Respective Input File Number: "))
    if inputno==1:
        filename="iris.csv"
    elif inputno==2 :
        filename="small.csv"
    elif inputno==3:
        filename="big.csv"
    elif inputno==4:
        filename="phishing.txt"
    print("Filename: ",filename)
    df = pd.read_csv(str(filename))
    #print("Full data: ")
    #print(df)

    #print("df size before drop: ")
    #print(df.shape)
    #print("Column 1: ")
    #print(df['f1'])

    #print("Result data: ")
    #print(df['label'])


    X=np.array(df.drop(['label'],1))
    temp_X=np.array(df.drop(['label'],1))

    total_feature=X.shape[1]
    print("Total Features: ",total_feature)

    y=np.array(df['label'])
    print("y shape: ",y.shape)
    print("X shape: ",X.shape)

    print("The Feature Slection ALgorithms are:\n")
    print("    1. Forward Selection\n    2. Backward Elemination\n    3. Original Search\n    4. All Algorithms\n")
    #print("\n\nEnter The Feature Selection Algorithm Number: ")

    inputno=int(input("\n\nEnter The Feature Selection Algorithm Number: "))
    if inputno==1:
        forward_selection()
    elif inputno==2 :
        backward_selection()
    elif inputno==3:
        original_search()
    elif inputno==4:
        forward_selection()
        backward_selection()
        original_search()
        showallgraph()
    print("\n\n\n\n\n")    





