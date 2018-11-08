#ifndef HMM_H_
#define HMM_H_

#define N_STATE		3
#define N_PDF		2
#define N_DIMENSION	39

#include <string>

using namespace std;

typedef struct {
  float weight;
  float mean[N_DIMENSION];
  float var[N_DIMENSION];
} pdfType;

typedef struct {
  pdfType pdf[N_PDF];
} stateType;

typedef struct {
  string name;
  float tp[N_STATE+2][N_STATE+2];
  stateType state[N_STATE];
} hmmType;

extern hmmType phones[];

#endif