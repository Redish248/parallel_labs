num_threads=(1 3 4 5 8 10 20 30 100)
program_name=("static1" "static3" "static4" "static5" "dynamic1" "dynamic3" "dynamic4" "dynamic5" "guided1" "guided3" "guided4" "guided5")
n1_arr=(85698 86964 85577 83243)
n2_arr=(48505570 49508863 49557942 49666431)
n_arr=

make clean
make static
make dynamic
make guided

function get_n_arr {
  n1=${n1_arr[3]}
  n2=${n2_arr[3]}
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
