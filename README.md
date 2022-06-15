# PyQASP

## Running PyQASP Solver
Download the executable file located into dist folder and run it
```
pyqasp -d -s quabs myprogram.qasp
```
use -h option to get available options
```
pyqasp -h 
```
## Build Executable from source
Executable file is generated using pyinstaller 
```
 pip install pyinstaller
 ```
 From repository root run
 ```
 ./clean-install.bash pyqasp
 ```
 It cleans and build a new executable file that is stored by default into dist folder
