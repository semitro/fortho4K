echo "building.."
./rebuild.sh
echo "done"
echo "Start testing"
cat stdlib.frt test1.frt test2.frt - | ./forto4ka  | sed 's/ *~*> *//g'

