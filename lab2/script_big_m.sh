n2_arr=(666522 640059 635057 629554)
program_name="lab2"

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/lib/lib
make clean
make
for i in 0 1 2 3; do
  echo "-------------------"
  n2=${n2_arr[$i]}
  result_file_name=${program_name}_big_m_$i.csv

  echo "start; end; m; n; ms" >$result_file_name

  for m in 5 6 7 8 9 10 15 20 30 50 70 100; do
    start=$(($(date +%s%N) / 1000000))
    r=$(./$program_name $n2 $m)
    end=$(($(date +%s%N) / 1000000))
    echo $start ";" $end ";" $m ";" $r >>$result_file_name
  done

  echo $result_file_name OK
done
