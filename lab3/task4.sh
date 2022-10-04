num_threads=(1 3 4 5 8 10 20 30 100)
n1=
n2=

i=0

make clean
make lab3

for m_thread in ${num_threads[@]}; do
  echo "-------------------"
  echo "N now: " n1
  echo "threads now: " $m_thread
  ./lab3 $n1 $m_thread
  echo "-------------------"
    echo "N now: " n2
    echo "threads now: " $m_thread
    ./lab3 $n2 $m_thread
done