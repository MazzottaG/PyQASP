# PyQASP

## Running PyQASP Solver
Download the executable file located into dist folder and run it
```
pyqasp myprogram.qasp
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
 pyinstaller --onefile --add-data 'resources:resources' src/parser.py --name pyqasp
 ```
 Produced executable is stored by default into dist folder
