//
// Created by cluster on 2021/1/4.
//

#ifndef LOGISTIC_SGD_UTILS_H
#define LOGISTIC_SGD_UTILS_H

#include <vector>
using namespace std;

class utils {
public:
    int repeatIter;
    utils(int iternums);
    vector<int> exchangeElement(vector<int> data,int GroupNum,int Group1,int Group2,int part);
    vector<vector<int>> divideGroup(vector<int> nodes,int groupNums);
    int position(double *vec,int size,int index);
    vector<int> findFastNodes(double * time,vector<int> group,int node,int numsofGrup,int size);
    void changeGroup(vector<vector<int>> &data,int node,vector<int> fastVec,int numsOfgroup,int iter);
    void MasterNodes(int procnum,int nodesOfGroup,int DynamicGroup,int maxIteration);
};


#endif //LOGISTIC_SGD_UTILS_H
