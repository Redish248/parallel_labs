num_threads=(1 3 4 5 8 10 20 30 100)
program_name=("static1" "static3" "static4" "static5" "dynamic1" "dynamic3" "dynamic4" "dynamic5" "guided1" "guided3" "guided4" "guided5")
n1=
n2=

i=0

make clean
make static
make dynamic
make guided

for m_thread in ${num_threads[@]}; do
  for name in ${program_name[@]}; do
    echo "-------------------"
    echo "N now: " n1
    echo "program_name now: " name
    ./$name $n1 $m_thread
    echo "-------------------"
    echo "N now: " n2
    echo "program_name now: " name
    ./$name $n1 $m_thread
  done
done