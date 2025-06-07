if [[ $# -ge 3 ]];then
        echo "Expected [depedencies? Y/N] executable_name"
        exit 0
elif [[ $# -le 0 ]];then
        echo "Expected [depedencies? Y/N] executable_name"
        exit 0
fi

DEP=""
exec_name=$1
if [[ $# == 2 ]];then 
        exec_name=$2
        DEP=$1
        DEP="${DEP^^}"
fi

rm -rf dist
rm -rf build
rm $exec_name".spec"

if [[ $DEP == "Y" ]];then
        pyinstaller --onefile --add-data "resources:resources" --collect-submodules "sklearn" --name $exec_name src/main.py
else
        pyinstaller --onefile --add-data "resources:resources" --name $exec_name src/main.py
fi