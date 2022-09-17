timestamp() {
  date +%s
}

make clean
rm out-*
make

counter=0
execution_time=0
max_time=5
while [ $execution_time -lt $max_time ]; do
  start_time=$(timestamp)
  ./lab1-seq $counter >> out-seq.txt
  ((current_time = timestamp - $start_time))
  ((counter++))
done

echo "--------------------------------------------- FINISH SEQ ------------------------------------------"
echo "---------------------------------------------------------------------------------------------------"

counter=0
execution_time=0
while [ $execution_time -lt $max_time ]; do
  start_time=$(timestamp)
  ./lab1-par-1 $counter >> out-par1.txt
  ((current_time = timestamp - $start_time))
  ((counter++))
done

echo "--------------------------------------------- FINISH PAR-1 ------------------------------------------"
echo "---------------------------------------------------------------------------------------------------"

counter=0
execution_time=0
while [ $execution_time -lt $max_time ]; do
  start_time=$(timestamp)
  ./lab1-par-3 $counter >> out-par3.txt
  ((current_time = timestamp - $start_time))
  ((counter++))
done

echo "--------------------------------------------- FINISH PAR-3 ------------------------------------------"
echo "---------------------------------------------------------------------------------------------------"

counter=0
execution_time=0
while [ $execution_time -lt $max_time ]; do
  start_time=$(timestamp)
  ./lab1-par-4 $counter >> out-par4.txt
  ((current_time = timestamp - $start_time))
  ((counter++))
done

echo "--------------------------------------------- FINISH PAR-4 ------------------------------------------"
echo "---------------------------------------------------------------------------------------------------"

counter=0
execution_time=0
while [ $execution_time -lt $max_time ]; do
  start_time=$(timestamp)
  ./lab1-par-5 $counter >> out-par5.txt
  ((current_time = timestamp - $start_time))
  ((counter++))
done

echo "--------------------------------------------- FINISH PAR-5 ------------------------------------------"
echo "---------------------------------------------------------------------------------------------------"