#include "prob.h"
#include  <cmath>
#include "../include/neighbors.h"
class logistic_sgd
{
private:
    problem *prob;

    double *w;
    double *gradient;
    double *average_w;
    double bias; 
    int dim;
    int num;
    int proNum;
    int myId;
public:
    problem *testprob;
    double comm_time;
    double cal_time;
    int Comm_method;
    int Sparse_comm;
    int nodesOfGroup;
    neighbors nears;
    logistic_sgd(problem *prob, int proNum, int myId,int nodes);
    void train(int batch_size, int epoch_num, double ratio, double learning_rate = 0.01 );
    void new_train(int batch_size, int epoch_num, double ratio, double learning_rate = 0.01 );
    void predict(problem *prob);
    void resort();
    double vector_mul_vector(feature_node **x, int index);
    
    ~logistic_sgd();
};


