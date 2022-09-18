lab_names=("lpar-icc-1" "lpar-icc-3" "lpar-icc-4" "lpar-icc-5")
for program_name in ${lab_names[@]}; do
  echo "-------------------"
  echo $program_name
  echo "-------------------"
  echo "Enter n1"
  read n1
  echo "Enter n2"
  read n2
  ((delta = ($n2 - $n1) / 10))
  echo $delta

  echo "n; ms" >$program_name.csv
  counter=$n1
  while [ $counter -lt $n2 ]; do
    ./$program_name $counter >>$program_name.csv
    ((counter += delta))
  done
  ./$program_name $n2 >>$program_name.csv
done
