#include "viterbi.h"

using namespace std;

valueType values[MAX_TIME_LENGTH][N_VOCA][MAX_PHONES][N_STATE];

void resetValues(size_t length) {
	size_t t;
    int v, p, s;
    for (t = 0; t < length; t++) {
        for (v = 0; v < N_VOCA; v++) {
            for (p = 0; p < MAX_PHONES; p++) {
                for (s = 0; s < N_STATE; s++) {
                    values[t][v][p][s].isAssigned = false;
                }
            }
        }
    }
}

double observationProb[N_PHONE][N_STATE];

void memoizeObservationProbs(double spectrum[]) {
    int p, s;
    int n_state;
    for (p = 0; p < N_PHONE; p++) {
        n_state = getNumberOfPhoneState(p);
        for (s = 0; s < n_state; s++) {
            observationProb[p][s] = getObservationProb(p, s, spectrum);
        }
    }
}

valueType getMaxValue(size_t lastTimeIndex) {
    int v, p, s;
    valueType max;
    max.isAssigned = false;
    for (v = 0; v < N_VOCA; v++) {
        for (p = 0; p < vocas[v].n_phones; p++) {
            int p_index = phoneIndex[v][p];
            int n_state = getNumberOfPhoneState(p_index);
            for (s = 0; s < n_state; s++) {
                if (!values[lastTimeIndex][v][p][s].isAssigned) continue;
                if (!max.isAssigned || max.prob < values[lastTimeIndex][v][p][s].prob) {
                    max.isAssigned = true;
                    max.prob = values[lastTimeIndex][v][p][s].prob;
                    max.prevVoca = v;
                    max.prevPhone = p;
                    max.prevState = s;
                }
            }
        }
    }

    return max;
}

void backtrace(size_t t, int v, int p, int s, vector<string> &result) {
    valueType *value = &values[t][v][p][s];

    if(t > 0) {
        backtrace(t-1, value->prevVoca, value->prevPhone, value->prevState, result);
    }

    if (value->wordChanged) {
        result.push_back(vocas[value->prevVoca].name);
    }
}

void runViterbi(size_t length, double spectrogram[][N_DIMENSION], vector<string> &result) {
    size_t t;
    int v, p, s;
    vector<transitionType>::iterator trans;
    valueType * value;
    double currentProb, prevProb;
    int next_p_index;

    resetValues(length);

    memoizeObservationProbs(spectrogram[0]);
    for (trans = beginningTransitions.begin(); trans != beginningTransitions.end(); trans++) {
        value = &values[0][trans->voca][trans->phone][trans->state];
        currentProb = trans->prob + observationProb[trans->phone][trans->state];
        if(!value->isAssigned || value->prob < currentProb) {
            value->isAssigned = true;
            value->prob = currentProb;
        }
    }

    for (t = 0; t < length - 1; t++) {
        memoizeObservationProbs(spectrogram[t + 1]);
        for (v = 0; v < N_VOCA; v++) {
            for (p = 0; p < vocas[v].n_phones; p++) {
                int p_index = phoneIndex[v][p];
                int n_state = getNumberOfPhoneState(p_index);
                for (s = 0; s < n_state; s++) {
                    if (!values[t][v][p][s].isAssigned) continue;
                    prevProb = values[t][v][p][s].prob;
                    for (trans = transitions[v][p][s].begin(); trans != transitions[v][p][s].end(); trans++) {
                        value = &values[t+1][trans->voca][trans->phone][trans->state];
                        next_p_index = phoneIndex[trans->voca][trans->phone];
                        currentProb = prevProb + trans->prob + observationProb[next_p_index][trans->state];
                        if (!value->isAssigned || value->prob < currentProb) {
                            value->isAssigned = true;
                            value->prob = currentProb;

                            value->prevVoca = v;
                            value->prevPhone = p;
                            value->prevState = s;

                            value->wordChanged = trans->toNextWord;
                        }
                    }
                }
            }
        }
    }
    
    valueType max = getMaxValue(length - 1);
    backtrace(length - 1, max.prevVoca, max.prevPhone, max.prevState, result);
}
