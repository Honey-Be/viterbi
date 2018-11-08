#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <vector>
#include "phone.h"
#include "voca.h"

using namespace std;

struct transitionType {
    // next state information (indexes)
    int voca;
    int phone;
    int state;

    // log transition probability
    double prob;
};

// transitions from beginning
extern vector<transitionType> beginningTransitions;

// transitions from each states
extern vector<transitionType> transitions[N_VOCA][MAX_PHONES][N_STATE];

// initialize and set beginningTransitions and transitions
void initAllTransitions();

#endif
