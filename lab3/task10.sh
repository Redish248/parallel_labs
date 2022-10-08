make clean
make

program_name=lab3 #insert best program name
m=3 #insert bes
n=49666431
start=$(($(date +%s%N) / 1000000))
echo $start
./$program_name $n $m 1
end=$(($(date +%s%N) / 1000000))
echo $end

