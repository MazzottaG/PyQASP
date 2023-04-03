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
 sudo pip install scikit-learn
 ```
 From repository root run
 ```
 ./clean-install.bash pyqasp
 ```
 It cleans and build a new executable file that is stored by default into dist folder

## Running PyQASP Solver
Download the executable file located into dist folder and run it
```
pyqasp myprogram.qasp
```
Use -h option to get available options
```
pyqasp -h 
```
