make clean
make lab4

m=4
n1=1801 # или N/2, N - это такое значение, при котором накладные расходы на распараллеливание превышают выигрыш от распараллеливания.
n2=629554
i=1801 # или N/2, N - это такое значение, при котором накладные расходы на распараллеливание превышают выигрыш от распараллеливания.
k= # поменять на нужное K из предыдущего эксперимента
programs=("lab4")

result_file="lab4_7_2_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+='.csv'
  echo "program; n; t,ms" >$full_result_file
  while [ $i -lt $n2 ]; do
    echo $i
    r=$(./$program_name $i $m $k)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 10000))
  done
  full_result_file=''
  i=$n1
done
