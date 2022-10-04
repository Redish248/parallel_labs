num_threads=(1 3 4 5 8 10 20 30 100)
schedule_type=("static" "dynamic" "guided")
chunk_size=(1 3 4 5)
n1=
n2=

i=0

make clean
make lab3

for m_thread in ${num_threads[@]}; do
  for type in ${schedule_type[@]}; do
    for chunk in ${chunk_size[@]}; do
      echo "-------------------"
      echo "N now: " n1
      echo "threads now: " $m_thread
      echo "schedule type now: " $type
      echo "chunk size now: " chunk
      ./schedule $n1 $m_thread $type $chunk
      echo "-------------------"
      echo "N now: " n2
      echo "threads now: " $m_thread
      echo "chunk size now: " chunk
      ./schedule $n1 $m_thread $type $chunk
    done
  done
done