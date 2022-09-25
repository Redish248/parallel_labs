lab_names=("lpar-1" "lpar-3" "lpar-4" "lpar-5")
n1_arr=(85698 86964 85577 83243)
n2_arr=(48505570 49508863 49557942 49666431)
i=0

make clean
make par
for program_name in ${lab_names[@]}; do
  echo "-------------------"
  n1=${n1_arr[$i]}
  n2=${n2_arr[$i]}
  i=$((i + 1))
  result_file_name=${program_name}.csv
  ((delta = ($n2 - $n1) / 10))

  echo "start; end; n; ms" >$result_file_name
  counter=$n1
  while [ $counter -lt $n2 ]; do
    start=$(($(date +%s%N) / 1000000))
    r=$(./$program_name $counter)
    end=$(($(date +%s%N) / 1000000))
    echo $start ";" $end ";" $r >>$result_file_name
    ((counter += delta))
  done
  start=$(($(date +%s%N) / 1000000))
  r=$(./$program_name $counter)
  end=$(($(date +%s%N) / 1000000))
  echo $start ";" $end ";" $r >>$result_file_name

  echo $program_name OK
done
