lab_names=("lpar-1" "lpar-3" "lpar-4" "lpar-5")
n1_arr=(1801 1740 1722 1482)
n2_arr=(666522 640059 635057 629554)
i=0

rm *.csv
make clean
make par
for program_name in ${lab_names[@]}; do
  echo "-------------------"
  n1=${n1_arr[$i]}
  n2=${n2_arr[$i]}
  result_file_name=${program_name}.csv
  ((delta = ($n2 - $n1) / 10))

  echo "n; ms" >$result_file_name
  counter=$n1
  while [ $counter -lt $n2 ]; do
    ./$program_name $counter >>$result_file_name
    ((counter += delta))
  done
  ./$program_name $counter >>$result_file_name
  (i+=1)

  echo $program_name OK
done
