# PyQASP

## Required python packages
* joblib
* scikit-learn
## Build Executable from source
Executable file is generated using pyinstaller 
```
 sudo pip install pyinstaller
 sudo pip install joblib
 sudo pip install scikit-learn==1.2.2
 ```
 From repository root run
 ```
 pip install .
 ```
 It cleans and build a new executable file, named **pyqasp**, that is stored by into **dist** folder

## Running PyQASP Solver

To run pyqasp use the following command 
```
pyqasp -s quabs -g gringo --no-wf problem.aspq
```

Use -h option to get available options
```
pyqasp -h 
```
