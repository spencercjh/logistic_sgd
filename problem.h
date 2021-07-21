//
// Created by guozheng on 19-8-27.
//

#ifndef ADMM_TEST_PROBLEM_H
#define ADMM_TEST_PROBLEM_H

#include <cstdio>

struct FeatureNode{
    int index;
    double value;
};
class Problem{
public:
    Problem(const char *filename);
    int l, n;
    double *y;
    double bias;
    char *line;
    int max_line_len;
    struct FeatureNode **x;
    struct FeatureNode *x_space;
    void readProblem(const char *filename);
    char *readLine(FILE *input);
    void exitInputError(int line_num);
};
#endif //ADMM_TEST_PROBLEM_H
