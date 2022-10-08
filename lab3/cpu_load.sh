echo "time p1 p2 p3 p4" >load_log.csv
while [ 1 ]; do
  load=$(mpstat -P ALL | tail -4 | awk '{print $3}')
  t=$(($(date +%s%N) / 1000000))
  echo $t
  echo $t $load >>load_log.csv
done
