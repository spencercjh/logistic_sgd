#!/bin/sh
current=$(date "+%Y_%m_%d_%H%M")

mpirun -n 4 ../bin/logistic_sgd -file ./logSGD.conf >log_"$current".log
