#!/bin/bash

for i in 1 2 4 8 16 24 32
do
	qsub -l h_rt=03:00:00 -l mem=2G -q \*@@dwarves -pe single $i ./pthreads_qsub.sh
done
