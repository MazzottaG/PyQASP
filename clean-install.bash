if [[ $# -ne 1 ]];then
        echo "Expected executable name"
        exit 0
fi
exec_name=$1
rm -rf dist
rm -rf build
rm $exec_name".spec"

pyinstaller --onefile --add-data "resources:resources" --name $exec_name src/parser.py
