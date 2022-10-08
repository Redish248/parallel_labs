num_threads=(1 3 4 5 8 10 20 30 100)
program_name=("static1" "static3" "static4" "static5" "dynamic1" "dynamic3" "dynamic4" "dynamic5" "guided1" "guided3" "guided4" "guided5")
n1=1801
n2=629554
n_arr=

make clean
make static
make dynamic
make guided

function get_n_arr {
  ((delta = ($n2 - $n1) / 10))

  counter=$n1
  n_arr[0]=$counter
  i=1
  while [ $counter -lt $n2 ]; do
    ((counter += $delta))
    n_arr[$i]=$counter
    ((i = $i + 1))
  done
  n_arr[$i]=$n2
}

get_n_arr

result_file=lab3_schedule.csv
echo "file; n; t,ms" >$result_file
for name in "${program_name[@]}"; do
  for m_thread in "${num_threads[@]}"; do
    for n in "${n_arr[@]}"; do
      r=$(./"$name" "$n" "$m_thread")
      echo "$name" ";" "$r" >>$result_file
      echo $name - $n - $m_thread
    done
  done
done
