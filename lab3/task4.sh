num_threads=(1 3 4 5 8 10 20 30 50 70 100)
n1=1801
n2=629554
n_arr=

root_path=/mnt/redish/maga/parallel_labs
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
}

function print_n_arr {
  for n in "${n_arr[@]}"; do
    echo $n
  done
}

function execute_lab1 {
  cd $root_path/lab1 || return
  make clean
  make par

  echo "execute lab1"
  lab1_files=("lpar-1" "lpar-3" "lpar-4" "lpar-5")
  result_file=$root_path/results/lab1.csv
  echo "threads; N; t,ms" >$result_file
  for program_name in "${lab1_files[@]}"; do
    for n in "${n_arr[@]}"; do
      r=$(./$program_name $n)
      echo $program_name ";" $r >>$result_file
      echo $program_name "-" $n
    done
  done
}

function execute_lab2 {
  cd $root_path/lab2 || return
  make clean
  make

  echo "execute_lab2"
  result_file=$root_path/results/lab2.csv
  echo "threads; N; t,ms" >$result_file
  for m in "${num_threads[@]}"; do
    for n in "${n_arr[@]}"; do
      r=$(./lab2 $n $m)
      echo $m ";" $r >>$result_file
      echo $m "-" $n
    done
  done
}

function execute_lab3 {
  cd $root_path/lab3 || return
  make clean
  make

  echo "execute_lab3"
  result_file=$root_path/results/lab3.csv
  echo "threads; N; t,ms" >$result_file
  for m in "${num_threads[@]}"; do
    for n in "${n_arr[@]}"; do
      r=$(./lab3 $n $m)
      echo $m ";" $r >>$result_file
      echo $m "-" $n
    done
  done
}

mkdir $root_path/results
get_n_arr
print_n_arr
execute_lab1
execute_lab2
execute_lab3
