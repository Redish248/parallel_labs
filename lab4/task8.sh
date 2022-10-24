make clean
make lab4_task8

m=4 # 2 3 4 5 6
n1=1801 # или N/2, N - это такое значение, при котором накладные расходы на распараллеливание превышают выигрыш от распараллеливания.
n2=629554
i=1801 # или N/2, N - это такое значение, при котором накладные расходы на распараллеливание превышают выигрыш от распараллеливания.
programs=("lab4_task8")
result_file="lab4_8_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+=_$m
  full_result_file+='.csv'
  echo "program; n; t,ms" >$full_result_file
#  while [ $i -lt $n2 ]; do
    i=629554
    start=$(($(date +%s%N) / 1000000))
    echo $start
    r=$(./$program_name $i $m)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 62775))
    end=$(($(date +%s%N) / 1000000))
    echo $end
#  done
  full_result_file=''
#  i=$n1
done
