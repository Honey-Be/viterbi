#ifndef PHONE_H_
#define PHONE_H_

#define N_PHONE 21
#define N_STATE		3
#define N_PDF		2
#define N_DIMENSION	39

#include <string>

using namespace std;

typedef struct {
  double weight;
  double mean[N_DIMENSION];
  double var[N_DIMENSION];
} pdfType;

typedef struct {
  pdfType pdf[N_PDF];
} stateType;

typedef struct {
  string name;
  double tp[N_STATE+2][N_STATE+2];
  stateType state[N_STATE];
} phoneType;

extern phoneType phones[N_PHONE];

int getPhoneIndex(string name);
int getNumberOfPhoneState(int index);

#endif