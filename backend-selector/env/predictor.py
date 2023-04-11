import joblib


def read_execution_data(filename):
    
    records={}
    with open(filename,"r") as f:
        # backend,solver,benchmark,instance,status800,status,result,real,time,user,system,memory,par2,par10
        header = True
        for line in f:
            if not header:
                backend,solver,benchmark,instance,status,result,real,time,user,system,memory,status800,par10,par2 = line.strip().split(",")
                
                try:
                    records[instance][solver]=line.strip()
                except:
                    records[instance]={solver:line.strip()}
            else:
                header=False
    return records


def read_extracted_features(filename):
    # return a dataset of row where the first column is the instance name and the others are the features
    dataset=[]
    with open(filename,"r") as f:
        for line in f:
            row = line.strip().split(",")
            row = [float(row[i]) if i>=3 and i <=8 else int(row[i]) if i>0 else row[i] for i in range(len(row))]
            dataset.append(row)
    return dataset

def predict(execution_filename,features_filename,mode):

    execution_data = read_execution_data(execution_filename)
    features       = read_extracted_features(features_filename)
    
    loaded_model   = joblib.load("models/uniquely_random-forest.joblib")
    print(loaded_model.get_params())
    predictions={}
    backend_to_solver={
        "depqbf-blo.bash":"wf.gc.pyqasp.depqbf-blo.bash",
        "rareqs.bash":"wf.pyqasp.rareqs.bash",
        "quabs.bash":"wf.pyqasp.quabs.bash"
    }
    with open ("PyQASP-Auto-custom-solvers.csv",mode) as res_file:
        for row in features:
            instance_name=row[0]
            row=row[1:]
            backend = loaded_model.predict([row])[0]
            try:
                predictions[backend]+=1
            except:
                predictions[backend]=1
            
            try:
                res_file.write(execution_data[instance_name][backend_to_solver[backend]]+"\n")
            except:
                print("Unable to find exec data for",instance_name)
                break

    print(predictions)

predict("Exec_Data_ICLP23.csv","extracted_features.csv","w")
predict("Exec_Data_2qbf_iclp23.csv","features.2qbf.csv","a")
