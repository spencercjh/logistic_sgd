#!/bin/sh

current=$(date "+%Y_%m_%d_%H%M")

mpirun -n 8 -f hostfile ../bin/logistic_sgd -file ./logSGD.conf >>log_"$current".log
