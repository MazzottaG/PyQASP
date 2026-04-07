# PyQASP

## Required python packages
* joblib
* scikit-learn
## Build Executable from source
pyqasp can be installed as a python module 
```
 sudo pip install joblib
 sudo pip install scikit-learn==1.2.2
 ```
 From repository root run
 ```
 pip install .
 ```
 It cleans and installs **pyqasp_opt**

## Running PyQASP Solver

To run pyqasp use the following command 
```
pyqasp_opt -s quabs -g gringo --no-wf -w global_weak.lp problem.aspq
```

Use -h option to get available options
```
pyqasp_opt -h 
```
