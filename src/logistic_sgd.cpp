#include "../include/logistic_sgd.h"
#include "mpi.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;

logistic_sgd::logistic_sgd(problem *prob, int proNum, int myId, int nodes) {
  this->prob = prob;
  this->proNum = proNum; //子问题个数
  this->myId = myId;
  dim = prob->n;       // dim为参数的纬度
  num = prob->l;       // num为单机上样本的数量
  w = new double[dim]; // w存储参数
  gradient = new double[dim]; //当前节点的梯度(batch_size个样本的梯度相加)
  average_w = new double[dim];
  bias = 0;
  cal_time = 0.0;
  comm_time = 0.0;
  nodesOfGroup = nodes;
  nears.neighborsNums = nodesOfGroup;
  nears.neighs = new int[nears.neighborsNums];
  for (int i = 0; i < dim; i++) {
    w[i] = 0;
    gradient[i] = 0;
    average_w[i] = 0;
  }
}

logistic_sgd::~logistic_sgd() {
  delete[] w;
  delete[] gradient;
  delete[] average_w;
//  cout << myId << " DELETE logistic_sgd" << endl;
}

double learning_rate_decay(int step, double optimizer_max_iter_num = 200,
                           double end_learning_rate = 1e-2,
                           double base_learning_rate = 1, double gamma = 2) {
  double ALPHA = end_learning_rate +
                 (base_learning_rate - end_learning_rate) *
                     pow(1 - 1.0 * step / optimizer_max_iter_num, gamma);
  return ALPHA;
}

void logistic_sgd::train(int batch_size, int maxiteration, double ratio,
                         double learning_rate) {
  double ex = 0;
  double ex1 = 0;
  double loss = 0;
  double Cal_start, Cal_end;
  double Comm_start, Comm_end;

  if (myId == 0)
    printf("%3s %12s %12s %12s %12s %12s \n", "#", "learning_rate", "current",
           "loss", "comm_time", "cal_time");

  //    int maxiteration=0;
  //    maxiteration=ceil(prob->l/batch_size)*epoch_num;

  for (int iter = 0; iter < maxiteration;) {
    loss = 0;
    resort();
    for (int j = 0; j < num; j += batch_size) {
      int counts = 0;
      iter++;
      Cal_start = MPI_Wtime();
      learning_rate = learning_rate_decay(iter, maxiteration);
      for (int k = j; k < batch_size + j && k < num; k++) {
        ex = exp(-1.0 * prob->y[k] * vector_mul_vector(prob->x, k));
        ex1 = ex / (1.0 + ex) * (-1.0 * prob->y[k]);
        feature_node *i = prob->x[k];
        for (; i->index != -1; i++) {
          gradient[i->index] = gradient[i->index] + ex1 * i->value;
        }
        counts++;
      }

      for (int m = 0; m < dim; m++) {
        w[m] = w[m] - (learning_rate * gradient[m] / counts);
      }
      for (int i = 0; i < dim; i++) { //每次训练将两个梯度数组置零
        gradient[i] = 0;
      }
      Cal_end = MPI_Wtime();

      Comm_start = MPI_Wtime();
      MPI_Allreduce(w, average_w, dim, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
      Comm_end = MPI_Wtime();

      for (int m = 0; m < dim; m++) {
        w[m] = average_w[m] / proNum;
      }
      if (myId == 0) {
        int counter = 0;
        loss = 0;
        for (int i = 0; i < testprob->l; i++) {
          ex = exp(-1.0 * testprob->y[i] * vector_mul_vector(testprob->x, i));
          loss += log(1.0 + ex);
        }
        for (int i = 0; i < testprob->l; ++i) {
          double temp = 1.0 / (1 + exp(-1 * vector_mul_vector(testprob->x, i)));
          if (testprob->y[i] == 1 && temp >= 0.5) {
            ++counter;
          } else if (testprob->y[i] == -1 && temp < 0.5) {
            ++counter;
          }
        }
        double epch_cal = Cal_end - Cal_start;
        cal_time += epch_cal;
        double epch_com = Comm_end - Comm_start;
        comm_time += epch_com;
        printf("%3d %12f %12f %12f %12f %12f\n", iter, learning_rate,
               counter * 100.0 / testprob->l, loss / testprob->l, epch_com,
               epch_cal);
        // cout<<epoch+1<<"  "<<learning_rate<<" "<< loss/testprob->l<<"
        // "<<counter * 100.0 / testprob->l<<" "<<endl;
      }
      if (iter > maxiteration - 1) {
        return;
      }
    }
  }
}

double logistic_sgd::vector_mul_vector(feature_node **x,
                                       int in) //求w和x两个向量的成绩
{
  feature_node *i;
  double mul_result = 0;
  for (i = x[in]; i->index != -1; i++)
    mul_result += w[i->index] * i->value;
  return mul_result + bias;
}

void logistic_sgd::resort() {
  srand((unsigned)time(NULL));
  double y_temp;
  feature_node *x_temp;
  int index;
  for (int i = 0; i < num; i++) {
    index = rand() % num;

    y_temp = prob->y[i];
    prob->y[i] = prob->y[index];
    prob->y[index] = y_temp;

    x_temp = prob->x[i];
    prob->x[i] = prob->x[index];
    prob->x[index] = x_temp;
  }
}
