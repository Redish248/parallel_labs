n1_arr=(1801 1740 1722 1482)
n2_arr=(666522 640059 635057 629554)
th_arr=(1 3 4 5)
program_name="lab2"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lib/lib
make clean
make
for i in 0 1 2 3; do
  echo "-------------------"
  n1=${n1_arr[$i]}
  n2=${n2_arr[$i]}
  m=${th_arr[$i]}
  result_file_name=${program_name}_$m.csv
  ((delta = ($n2 - $n1) / 10))

  echo "start; end; n; ms" >$result_file_name
  counter=$n1
  while [ $counter -lt $n2 ]; do
    start=$(($(date +%s%N) / 1000000))
    r=$(./$program_name $counter $m)
    end=$(($(date +%s%N) / 1000000))
    echo $start ";" $end ";" $r >>$result_file_name
    ((counter += delta))
  done
  start=$(($(date +%s%N) / 1000000))
  r=$(./$program_name $counter $m)
  end=$(($(date +%s%N) / 1000000))
  echo $start ";" $end ";" $r >>$result_file_name

  echo $result_file_name OK
done
