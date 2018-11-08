#include <math.h>
#include <iostream>
#include "phone.h"
#include "observation.h"

// get log probability for one probability density function. O(N_DIMENSION)
double probForOnePDF(int phone, int state, int pdf, int spectrum[N_DIMENSION]) {
    double sum = 0;
    double * mean = phones[phone].state[state].pdf[pdf].mean;
    double * var = phones[phone].state[state].pdf[pdf].var;

    for (int d = 0; d < N_DIMENSION; d++) {
        sum -= -log(var[d])/2.0 - (spectrum[d] - mean[d]) * (spectrum[d] - mean[d]) / (2 * var[d]);
    }

    return sum;
}

// O(N_DIMENSION * N_PDF)
double getObservationProb(int phone, int state, int spectrum[N_DIMENSION]) {
    double probs[N_PDF];

    cout << "probs: ";
    for (int pdf = 0; pdf < N_PDF; pdf++) {
        probs[pdf] = probForOnePDF(phone, state, pdf, spectrum);
        cout << probs[pdf] << " ";
    }
    cout << endl;

    cout << "divided probs: ";
    // probs divided by first prob (not log scale)
    double dividedProbs[N_PDF];
    for (int pdf = 0; pdf < N_PDF; pdf++) {
        dividedProbs[pdf] = exp(probs[pdf] - probs[0]);
        cout << dividedProbs[pdf] << " ";
    }
    cout << endl;

    double sum = 0;
    for (int pdf = 0; pdf < N_PDF; pdf++) {
        // multiply weight and add to sum
        sum += dividedProbs[pdf] * phones[phone].state[state].pdf[pdf].weight;
    }

    return probs[0] + log(sum);
}
