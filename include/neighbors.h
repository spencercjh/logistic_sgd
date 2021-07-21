//
// Created by cluster on 2021/1/4.
//

#ifndef LOGISTIC_SGD_NEIGHBORS_H
#define LOGISTIC_SGD_NEIGHBORS_H

class neighbors {
public:
    int neighborsNums;
    int *neighs;
    void setNeighbours(int nums,int *set);
    void clearNeighbours();
};


#endif //LOGISTIC_SGD_NEIGHBORS_H
