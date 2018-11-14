#ifndef VITERBI_H_
#define VITERBI_H_

#include <iostream>
#include "transition.h"
#include "observation.h"

#define MAX_TIME_LENGTH 1000

struct valueType {
    bool isAssigned;
    double prob;
    int prevVoca;
    int prevPhone;
    int prevState;
    bool wordChanged;
};

extern valueType values[MAX_TIME_LENGTH][N_VOCA][MAX_PHONES][N_STATE];

void runViterbi(size_t length, double spectrogram[][N_DIMENSION], vector<string> &result);

#endif