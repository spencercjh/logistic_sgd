//
// Created by guozheng on 19-8-26.
//
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "../include/problem.h"
#define Malloc(type, n) (type *)malloc((n) * sizeof(type))
using namespace std;
Problem::Problem(const char *filename) {
    readProblem(filename);
}
void Problem::readProblem(const char *filename) {
    int max_index, inst_max_index, i;
    size_t elements, j;//它是一种“整型”类型
    FILE *fp = fopen(filename, "r");
    char *endptr, *idx, *val, *label;
    if (fp == NULL) {
        fprintf(stderr, "can't open input file %s\n", filename);//fprintf是格式化输出到一个stream，通常是到文件，标准错误，stdout输出到磁盘文件，stderr在屏幕
        exit(1);
    }
    l = 0;
    elements = 0;
    max_line_len = 1024;
    line = Malloc(char, max_line_len);
    while (readLine(fp) != NULL) {
        char *p = strtok(line, "\t");// 分解line为一组字符串，"\t"为分隔符。
        while (1) {
            p = strtok(NULL, "\t");//
            if (p == NULL || *p == '\n') {
                break;
            }
            elements++;
        }
        elements++;//elements为文件中所有样本中数据个数的总和
        l++;//l为文件中的样本数量
    }
    rewind(fp);
    bias = -1;
    y = Malloc(double, l);
    x = Malloc(struct FeatureNode *, l);
    x_space = Malloc(struct FeatureNode, elements + l);
    max_index = 0;
    j = 0;
    for (i = 0; i < l; i++) {
        inst_max_index = 0;
        readLine(fp);
        x[i] = &x_space[j];
        label = strtok(line, " \t\n");
        if (label == NULL) {
            exitInputError(i + 1);
        }
        y[i] = strtod(label, &endptr);
        if (endptr == label || *endptr != '\0') {
            exitInputError(i + 1);
        }
        while (1) {
            idx = strtok(NULL, ":");
            val = strtok(NULL, "\t");
            if (val == NULL) {
                break;
            }
            errno = 0;
            x_space[j].index = (int) strtol(idx, &endptr, 10);
            if (endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index) {
                exitInputError(i + 1);
            }else{
                inst_max_index = x_space[j].index;
            }
            errno = 0;
            x_space[j].value = strtod(val, &endptr);
            if (endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr))) {
                exitInputError(i + 1);
            }
            ++j;
        }
        if (inst_max_index > max_index) {
            max_index = inst_max_index;
        }
        if (bias >= 0) {
            x_space[j++].value = bias;
        }
        x_space[j++].index = -1;
    }
    if (bias >= 0) {
        n = max_index + 1;
        for (i = 1; i < l; i++) {
            (x[i] -2)->index = n;
        }
        x_space[j - 2].index = n;
    }else{
        n = max_index;
    }
    fclose(fp);
}
char *Problem::readLine(FILE *input) {//延长行，使之可以存下每行数据。
    int len;
    if (fgets(line, max_line_len, input) == NULL) { //从指定的流input读取一行，并把它存储在line所指向的字符串内。
        // 当读取(max_line_len-1)个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止;
        return NULL;
    }
    while (strrchr(line, '\n') == NULL) {//在参数line所指向的字符串中搜索最后一次出现字符'\n'的位置。
        max_line_len *= 2;
        line = (char *) realloc(line, max_line_len);// original space is nonsufficient, copy the value of
        //original space to new space address.
        len = (int) strlen(line);//计算字符串line的长度，但不包括空结束字符。
        if (fgets(line + len, max_line_len - len, input) == NULL) {//2*max_line_len>len的话，就不用再延长行了。
            break;
        }
    }
    return line;
}
void Problem::exitInputError(int line_num) {
    fprintf(stderr, "Wrong input format at line %d\n", line_num);
    exit(1);
}
