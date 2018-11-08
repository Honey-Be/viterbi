#ifndef VOCA_H_
#define VOCA_H_

#include <string>

#define N_VOCA 13
#define MAX_PHONES 6

 // it's a value about word penalty.
#define WORD_TRANS_WEIGHT 1

using namespace std;

struct vocaType {
    string name;
    int n_phones;
    string phones[MAX_PHONES];
};

extern vocaType vocas[N_VOCA];
extern int phoneIndex[N_VOCA][MAX_PHONES];

extern double unigram[N_VOCA];
extern double bigram[N_VOCA][N_VOCA];

void readUnigram();
void readBigram();

void initPhoneIndex();

#endif