echo "Enter name"
read name
echo "Enter from"
read from
echo "Enter to"
read to

counter=$from
while [ $counter -lt $to ]; do
  ./$name $counter
  ((counter++))
done