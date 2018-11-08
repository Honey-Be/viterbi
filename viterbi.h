#ifndef VITERBI_H_
#define VITERBI_H_

#include "transition.h"
#include "observation.h"

#define MAX_TIME_LENGTH 1000

struct valueType {
    bool isAssigned;
    double prob;
    int prevVoca;
    int prevPhone;
    int prevState;
};

extern valueType values[MAX_TIME_LENGTH][N_VOCA][N_PHONE][N_STATE];

void runViterbi(int length, double spectrogram[][N_DIMENSION], vector<string> &result);

#endif