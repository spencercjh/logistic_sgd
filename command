cd /mirror/zzy/logistic_sgd/build
make clean
make
mpirun -f ../bin/hostfile -np 4 ../bin/logistic_sgd


