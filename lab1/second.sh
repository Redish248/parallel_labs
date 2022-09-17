timestamp() {
  date +%s
}

lab_names=("lab1-seq" "lab1-par1" "lab1-par3" "lab1-par4" "lab1-par5")
for program_name in ${lab_names[@]}; do
  echo $program_name

  echo "-------------------"
  echo "Enter n1 for" $program_name
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
  ./program_name $n2 >>part2_seq.csv
done
