from sklearn import svm
from sklearn.model_selection import train_test_split,KFold,GridSearchCV,StratifiedKFold,cross_val_score,cross_validate
from sklearn import ensemble
from sklearn.metrics import accuracy_score,recall_score,f1_score,precision_score,classification_report
import json,numpy as np,seaborn as sns,sys
from joblib import dump, load
from sklearn.utils import resample
from sklearn import metrics
import joblib

seed = 42
timeout=800
par=10*timeout

def read_extracted_features():
    # return a dataset of row where the first column is the instance name and the others are the features
    dataset=[]
    with open("extracted_features.csv","r") as f:
        for line in f:
            row = line.strip().split(",")
            row = [float(row[i]) if i>=3 and i <=8 else int(row[i]) if i>0 else row[i] for i in range(len(row))]
            dataset.append(row)

    return dataset

def solved_uniquely(solvers,benchmarks):
    # return the list of instances solved uniquely by one backend solver
    # inst -> unique_solver
    
    # records contains for each instance the list of backend solvers that solved the instance if any 
    records={}
    # labels contains instances solved uniquely by one backend
    labels ={}
    total_instances=set()
    with open("Exec_Data_ICLP23.csv","r") as f:
        header = True
        for line in f:
            if not header:
                backend,solver,benchmark,instance,status,result,real,time,user,system,memory,status800,par10,par2 = line.strip().split(",")
                if solver not in solvers or benchmark not in benchmarks:
                    continue
                total_instances.add(instance)
                if float(par10) < par:
                    try:
                        records[instance].append(backend)
                    except:
                        records[instance]=[backend]
            else:
                header=False
        for inst in records:
            if len(records[inst]) == 1:
                labels[inst]=records[inst][0]
    print("Total instances:",len(total_instances),"Labeled instances",len(labels))
    return labels

def best_solver(solvers,benchmarks):
    # for each instance returns the best solver that solved the instance or the solver that took the smallest par score on domain of interest
    records={}
    labels ={}
    total_par_for_benchmark={}
    benchmark_for_instance={}
    with open("Exec_Data_ICLP23.csv","r") as f:
        header = True
        for line in f:
            if not header:

                backend,solver,benchmark,instance,status,result,real,time,user,system,memory,status800,par10,par2 = line.strip().split(",")
                if solver not in solvers or benchmark not in benchmarks:
                    continue
                benchmark_for_instance[instance]=benchmark
                try:
                    records[instance][backend]=float(par10)
                except:
                    records[instance]={backend:float(par10)}

                try:
                    total_par_for_benchmark[benchmark][backend]+=float(par10)
                except:
                    try:
                        total_par_for_benchmark[benchmark][backend]=float(par10)
                    except:
                        total_par_for_benchmark[benchmark]={backend:float(par10)}
            else:
                header=False

    best_solver_for_benchmark={}
    for benchmark in total_par_for_benchmark:
        min_=None
        best_backend=None
        for backend,score in total_par_for_benchmark[benchmark].items():
            if min_ is None or score < min_:
                min_=score
                best_backend=backend
        best_solver_for_benchmark[benchmark]=best_backend

    for inst in records:
        min_ = None
        best_backend = None
        for backend,score in records[inst].items():
            if min_ is None or score < min_:
                min_=score
                best_backend=backend
        if min_ is None or min_ >= par:
            labels[inst]=best_solver_for_benchmark[benchmark_for_instance[inst]]
        else:
            labels[inst]=best_backend

    return labels

def prepare(features,labels):
    #join features and labels on instance name and filter out instance name
    dataset = []
    for row in features:
        inst = row[0]
        try:
            target = labels[inst]
            dataset.append(row[1:]+[target])
        except:
            pass
    return dataset

def read_2qbf_features():
    dataset=[]
    with open("features.2qbf.uniq.csv","r") as f:
        for line in f:
            row = line.strip().split(",")
            row[0]="iclp23."+row[0]
            row = [float(row[i]) if i>=3 and i <=8 else int(row[i]) if i>0 else row[i] for i in range(len(row))]
            dataset.append(row)

    return dataset


def build_models_and_predication():
    solvers = ["wf.pyqasp.depqbf-blo.bash","wf.pyqasp.quabs.bash","wf.pyqasp.rareqs.bash"]
    benchmarks = ["ADFcoherence","fullQBFSelection.ch.aspq","MinmaxClique.fixed.pyqasp","paracoherent.aspq.fix","ponr","satParacohQasp.aspq.fix"]
    # training set contains only those instances that are uniquely solved by one backend solver

    #contains features for instances in paracoherent (441+73), ponr (94), ADFCoherence (326), QBF(992), MinMaxClique(45)
    extracted_features              = read_extracted_features()
    
    #contains features for instances in 2qbf 
    two_qbf_features                = read_2qbf_features()
    
    # contains for instances in paracoherent (441+73), ponr (94), ADFCoherence (326), QBF(992), MinMaxClique(45), the unique solver that solved such instance
    label_uniquely                  = solved_uniquely(solvers,benchmarks)
    
    #augmenting extracted_features and label_uniquely, adding features of 2qbf instances
    augmented_features                        = [row for row in extracted_features+two_qbf_features]
    label_uniquely_augmented                  = {}

    for row in two_qbf_features:
        label_uniquely_augmented[row[0]]="depqbf-blo.bash"
    
    for inst in label_uniquely:
        label_uniquely_augmented[inst]=label_uniquely[inst]
    
    datasets = {
        "uniquely"       : prepare(augmented_features,label_uniquely_augmented)
    }
    estimators={
        "random-forest":ensemble.RandomForestClassifier()
    }

    splits = {}
    trained_estimators={}
    for dataset_name in datasets:
        dataset = datasets[dataset_name]
        X = [row[:-1] for row in dataset]
        Y = [row[-1] for row in dataset]
        print("Considering dataset",dataset_name)

        X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=seed)
        splits[dataset_name]=[X_train, X_test, Y_train, Y_test]
        for estimator_name in estimators:
            print("Training estimator",estimator_name)
            estimator = estimators[estimator_name]
            estimator.fit(X_train,Y_train)
            dump(estimator, "models/"+dataset_name+"_"+estimator_name+".joblib")
            try:
                trained_estimators[dataset_name][estimator_name]=estimator
            except:
                trained_estimators[dataset_name]={estimator_name:estimator}
            yhat = estimator.predict(X_test)
            print("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
            print("Report 70-30")
            print(classification_report(Y_test,yhat))
            print("Training set size :",len(X_train))
            print("Test set size     :",len(X_test))
            
build_models_and_predication()
