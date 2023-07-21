# PyQASP

## Required python packages
* joblib
* scikit-learn
* pyinstaller
## Build Executable from source
Executable file is generated using pyinstaller 
```
 sudo pip install pyinstaller
 sudo pip install joblib
 sudo pip install scikit-learn==1.2.2
 ```
 From repository root run
 ```
 ./clean-install.bash pyqasp
 ```
 It cleans and build a new executable file, named **pyqasp**, that is stored by into **dist** folder

## Running PyQASP Solver

To run pyqasp using default setting 
```
./dist/pyqasp problem.aspq
```

Use -h option to get available options
```
./dist/pyqasp -h 
```
