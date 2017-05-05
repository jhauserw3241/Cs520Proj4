clear

echo "Dummy, Job type, Number of cores, Time" >> log.csv
grep -i "DATA" pthreads_qsub.sh.o* >> log.csv
grep -i "DATA" pthreads_qsub2.sh.o* >> log.csv
