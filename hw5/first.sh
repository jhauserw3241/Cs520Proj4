#
# My first shell script
#
clear
echo "Start"

echo "Knowledge is Power"

echo "Time,Number of Cores,Number of Threads,Memory Usage(RAM),Network Speed,Source Size,Programming Style" >> log.csv
for i in {1..5}
do
	echo "$i,$(($i * 5))" >> log.csv
done

# Add call to script in the for loop
#for i in {2..64..2}
#do
#	echo $i
#done

echo "End"
