echo "TEST #1"
cat stdlib.frt test1.frt | ./forto4ka | sed 's/ *~*> *//g'
echo "TEST FROM FIRST STEP"
cat stdlib.frt test_first_step.frt - | ./forto4ka  | sed 's/_MSG:No such word//' | sed 's/~>//g'  

