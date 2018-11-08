#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "fileutil.h"
#include "transition.h"
#include "viterbi.h"

using namespace std;

vector<string> inputPaths;

int length;
double spectrogram[MAX_TIME_LENGTH][N_DIMENSION];

void readTestInput(string path) {
    FILE *in = fopen(path.c_str(), "r");
    int t, d, dimension;

    fscanf(in, "%d%d", &length, &dimension);

    for (t = 0; t < length; t++) {
        for (d = 0; d < dimension; d++) {
            fscanf(in, "%lf", &spectrogram[t][d]);
        }
    }

    fclose(in);
}

string getRecName(string inputPath) {
    size_t pos = inputPath.find(".txt");
    return '\"' + inputPath.replace(pos, 4, ".rec") + '\"';
}

void runAllTests() {
    FILE *out = fopen("recognized.txt", "w");

    fprintf(out, "#!MLF!#\n");

    vector<string>::iterator inputPath;
    int count = 0;
    for (inputPath = inputPaths.begin(); inputPath != inputPaths.end(); inputPath++) {
        count++;
        if(count % 100 == 0) printf("%.2lf%%..\n", (double)count / (double)inputPaths.size() * 100);
        readTestInput(*inputPath);

        fprintf(out, "%s\n", getRecName(*inputPath).c_str());
                
        vector<string> result;
        runViterbi(length, spectrogram, result);
        
        vector<string>::iterator word;
        for (word = result.begin(); word != result.end(); word++) {
            if (*word == "<s>") continue;
            fprintf(out, "%s\n", word->c_str());
        }
        fprintf(out, ".\n");
    }

    printf("100%%\n");

    fclose(out);
}

int main()
{
    initAllTransitions();
    listAllInputPaths(inputPaths);
    runAllTests();
    
    return 0;
}
