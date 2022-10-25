make clean
#make new
make lab4
make schedule

m=1 # 2 3 4 5 6
n1=1801 # или N/2, N - это такое значение, при котором накладные расходы на распараллеливание превышают выигрыш от распараллеливания.
n2=629554
i=1801 # или N/2, N - это такое значение, при котором накладные расходы на распараллеливание превышают выигрыш от распараллеливания.
programs=("lab4" "static1" "static3" "static4" "static5" "dynamic1" "dynamic3" "dynamic4" "dynamic5" "guided1" "guided3" "guided4" "guided5")

result_file="lab4_6_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+=_$m
  full_result_file+='.csv'
  echo $program_name
  echo "program; n; t,ms" >$full_result_file
  while [ $i -lt $n2 ]; do
    echo $i
    r=$(./$program_name $i $m)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 62775))
  done
  full_result_file=''
  i=$n1
done
