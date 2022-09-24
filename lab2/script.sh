lab_names=("lseq" "lpar-1" "lpar-3" "lpar-4" "lpar-5")
n1_arr=(1801 1740 1722 1482)
n2_arr=(666522 640059 635057 629554)
th_arr=(1 3 4 5)
program_name="lab2"

rm *.csv
make clean
make
for i in 0 1 2 3; do
  echo "-------------------"
  n1=${n1_arr[$i]}
  n2=${n2_arr[$i]}
  m=${th_arr[$i]}
  result_file_name=${program_name}_$m.csv
  ((delta = ($n2 - $n1) / 10))

  echo "n; ms" >$result_file_name
  counter=$n1
  while [ $counter -lt $n2 ]; do
    ./$program_name $counter $m >>$result_file_name
    ((counter += delta))
  done
  ./$program_name $counter $m >>$result_file_name

  echo $result_file_name OK
done
