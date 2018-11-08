#include <math.h>
#include "transition.h"

using namespace std;

vector<transitionType> beginningTransitions;
vector<transitionType> transitions[N_VOCA][MAX_PHONES][N_STATE];

// 해당 phone에서 state로 시작할 확률
double getBeginningProb(int phoneIndex, int state) {
    return phones[phoneIndex].tp[0][state + 1];
}

// 해당 phone state에서 끝날 확률
double getEndingProb(int phoneIndex, int state) {
    int n_state = getNumberOfPhoneState(phoneIndex);
    return phones[phoneIndex].tp[state + 1][n_state + 1];
}

// 해당 phone에서 state s -> state d transition 확률
double getTransProb(int phoneIndex, int s, int d) {
    return phones[phoneIndex].tp[s + 1][d + 1];
}

void applyLogScale() {
    int i, v, p, s;

    for (i = 0; i < beginningTransitions.size(); i++) {
        beginningTransitions[i].prob = log(beginningTransitions[i].prob);
    }

    for (v = 0; v < N_VOCA; v++) {
        for (p = 0; p < MAX_PHONES; p++) {
            for (s = 0; s < N_STATE; s++) {
                for (i = 0; i < transitions[v][p][s].size(); i++) {
                    transitions[v][p][s][i].prob = log(transitions[v][p][s][i].prob);
                }
            }
        }
    }
}

void initAllTransitions() {
    transitionType t;

    // read and initialize unigram, bigram, indexes of vocabulary phones.
    readUnigram();
    readBigram();
    initPhoneIndex();

    t.toNextWord = false;
    // initialize beginning transitions
    for (int v = 0; v < N_VOCA; v++) {
        int p_index = phoneIndex[v][0];
        int n_state = getNumberOfPhoneState(p_index);
        for (int s = 0; s < n_state; s++) {
            t.voca = v;
            t.phone = 0;
            t.state = s;
            t.prob = unigram[v] * getBeginningProb(p_index, s);
            if (t.prob > 0) beginningTransitions.push_back(t);
        }
    }

    // initialize transitions from each states

    // case 0. every transitions of each phone hmm
    for (int v = 0; v < N_VOCA; v++) {
        for (int p = 0; p < vocas[v].n_phones; p++) {
            int p_index = phoneIndex[v][p];
            int n_state = getNumberOfPhoneState(p_index);
            for (int s = 0; s < n_state; s++) {
                for (int d = 0; d < n_state; d++) {
                    t.voca = v;
                    t.phone = p;
                    t.state = d;
                    t.prob = getTransProb(p_index, s, d);
                    if (t.prob > 0) transitions[v][p][s].push_back(t);
                }
            }
        }
    }

    // case 1. phone to next phone in same voca
    for (int v = 0; v < N_VOCA; v++) {
        for (int p = 0; p < vocas[v].n_phones - 1; p++) {
           int p_index = phoneIndex[v][p];
           int p_n_state = getNumberOfPhoneState(p_index);

           int next_p_index = phoneIndex[v][p+1];
           int next_p_n_state = getNumberOfPhoneState(next_p_index);
           t.voca = v;
           t.phone = p + 1;
           for (int s = 0; s < p_n_state; s++) {
               for (int next_s = 0; next_s < next_p_n_state; next_s++) {
                   t.state = next_s;
                   t.prob = getEndingProb(p_index, s) * getBeginningProb(next_p_index, next_s);
                   if (t.prob > 0) transitions[v][p][s].push_back(t);
               }
           }
        }
    }

    t.toNextWord = true;
    // case 2. last phone to next voca
    for (int v = 0; v < N_VOCA; v++) {
        int p = vocas[v].n_phones - 1;

        int p_index = phoneIndex[v][p];
        int p_n_state = getNumberOfPhoneState(p_index);

        // add transitions from current voca's last phone to next voca's first phone
        for (int nextv = 0; nextv < N_VOCA; nextv++) {
            t.voca = nextv;
            t.phone = 0;

            int next_p_index = phoneIndex[nextv][0];
            int next_p_n_state = getNumberOfPhoneState(next_p_index);

            for (int s = 0; s < p_n_state; s++) {
                for (int next_s = 0; next_s < next_p_n_state; next_s++) {
                    t.state = next_s;
                    t.prob = getEndingProb(p_index, s) * bigram[v][nextv] * getBeginningProb(next_p_index, next_s);
                    if (t.prob > 0) transitions[v][p][s].push_back(t);
                }
            }
        }

        if (vocas[v].phones[p] == "sp" && p > 0) {
            // If the last phone is "sp", then we can skip it
            int sp_index = phoneIndex[v][p];
            double skipProb = phones[sp_index].tp[0][2];

            p--;

            p_index = phoneIndex[v][p];
            p_n_state = getNumberOfPhoneState(p_index);

            for (int nextv = 0; nextv < N_VOCA; nextv++) {
                t.voca = nextv;
                t.phone = 0;

                int next_p_index = phoneIndex[nextv][0];
                int next_p_n_state = getNumberOfPhoneState(next_p_index);

                for (int s = 0; s < p_n_state; s++) {
                    for (int next_s = 0; next_s < next_p_n_state; next_s++) {
                        t.state = next_s;
                        t.prob = getEndingProb(p_index, s) * skipProb * bigram[v][nextv] * getBeginningProb(next_p_index, next_s);
                        if (t.prob > 0) transitions[v][p][s].push_back(t);
                    }
                }
            }
        }
    }

    applyLogScale();
}
