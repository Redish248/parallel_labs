load_file=results/load_log.csv
echo "time p1 p2 p3 p4" >$load_file
while [ 1 ]; do
  load=$(mpstat -P ALL | tail -4 | awk '{print $3}')
  t=$(($(date +%s%N) / 1000000))
  echo $t
  echo $t $load >>$load_file
done
