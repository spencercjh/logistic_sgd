#!/bin/sh
#BSUB -e /data/home/xjsjleiyongmei/hx/work_space/logistic_sgd/bin/result/sptest.err
#BSUB -o /data/home/xjsjleiyongmei/hx/work_space/logistic_sgd/bin/result/sptest.out
#BSUB -n 16
#BSUB -q priority
#BSUB -J SGD
#BSUB -R "span[ptile=8]"

#export LD_LIBRARY_PATH=/data/home/xjsjleiyongmei/xjy/mpich/lib:$LD_LIBRARY_PATH

HOME=/data/home/xjsjleiyongmei/xjy/workspace

ncpus=`cat $LSB_DJOB_HOSTFILE | wc -l `
mpirun -machine $LSB_DJOB_HOSTFILE -np 16 /data/home/xjsjleiyongmei/hx/work_space/logistic_sgd/bin/logistic_sgd -file /data/home/xjsjleiyongmei/hx/work_space/logistic_sgd/bin/logSGD.conf