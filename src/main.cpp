#include "../include/logistic_sgd.h"
#include "../include/properties.h"
#include "../utils/utils.h"
#include "mpi.h"
#include <ctime>
#include <iostream>

using namespace std;

void Orgin_SGD(int argc, char **argv) {
  int myId, proNum;
  int maxDim;
  char fileName[100];
  char testfileName[100];
  clock_t start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  MPI_Comm_size(MPI_COMM_WORLD, &proNum);
  Properties properties(argc, argv);

  int batch_size = properties.GetInt("batchSize");
  //    int epoch_num = properties.GetInt("epochNum");
  int maxIteration = properties.GetInt(
      "maxIteration"); //最后一个节点的数据量不一定和其他节点相同
  std::string train_data_path = properties.GetString("train_data_path");
  std::string test_data_path = properties.GetString("test_data_path");

  //训练数据
  sprintf(fileName, train_data_path.c_str(), proNum, myId);
  string dataname(fileName);
  //测试数据
  sprintf(testfileName, test_data_path.c_str());
  string testdataname(testfileName);

  problem *prob = new problem(fileName);
  maxDim = prob->n;
  vector<int> nodelist(proNum);
  for (int i = 0; i < proNum; i++) {
    nodelist.push_back(i);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&prob->n, &maxDim, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  prob->n = maxDim;
  logistic_sgd log_sgd(prob, proNum, myId, 4);

  problem *testprob = new problem(testfileName);
  log_sgd.testprob = testprob;

  if (myId == 0)
    cout << "max dim：" << maxDim << endl;

  start_time = clock();
  log_sgd.train(batch_size, maxIteration, 0, 0);
  end_time = clock();

  MPI_Finalize();

  if (myId == 0) {
    cout << "run_time:" << (double)(end_time - start_time) / CLOCKS_PER_SEC
         << " "
         << "com_time:" << log_sgd.comm_time << " "
         << "cal_time:" << log_sgd.cal_time << " " << endl;
  }
}

int main(int argc, char **argv) { Orgin_SGD(argc, argv); }
