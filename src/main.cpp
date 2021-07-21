#include "../include/logistic_sgd.h"
#include "../include/properties.h"
#include "mpi.h"
#include <iostream>
#include <vector>

using namespace std;

void Origin_SGD(int argc, char **argv) {
  int myId, proNum;
  int maxDim;
  char fileName[100];
  double start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  MPI_Comm_size(MPI_COMM_WORLD, &proNum);

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);
  printf("processor %s, rank %d out of %d processors\n", processor_name, myId,
         proNum);

  Properties properties(argc, argv);

  int batch_size = properties.GetInt("batchSize");
  int maxIteration = properties.GetInt(
      "maxIteration"); //最后一个节点的数据量不一定和其他节点相同
  std::string train_data_path = properties.GetString("train_data_path");
  std::string test_data_path = properties.GetString("test_data_path");

  //训练数据
  sprintf(fileName, train_data_path.c_str(), proNum, myId);
  string dataname(fileName);

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

  problem *test_prob = new problem(test_data_path.c_str());
  log_sgd.testprob = test_prob;

  if (myId == 0)
    cout << "max dim：" << maxDim << endl;

  start_time = MPI_Wtime();
  log_sgd.train(batch_size, maxIteration, 0, 0);
  end_time = MPI_Wtime();

  MPI_Finalize();

  if (myId == 0) {
    cout << "run_time:" << end_time - start_time << " "
         << "com_time:" << log_sgd.comm_time << " "
         << "cal_time:" << log_sgd.cal_time << " " << endl;
  }
}

int main(int argc, char **argv) { Origin_SGD(argc, argv); }
