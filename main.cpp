#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fileutil.h"
#include "transition.h"
#include "viterbi.h"

using namespace std;

vector<string> inputPaths;

size_t length;
double spectrogram[MAX_TIME_LENGTH][N_DIMENSION];

void readTestInput(string path) {
    ifstream inFile(path, ios::in);
    size_t t, d, dimension;

    inFile >> length >> dimension;

    for (t = 0; t < length; t++) {
        for (d = 0; d < dimension; d++) {
            inFile >> spectrogram[t][d];
        }
    }
}

void getRecName(string s, string &out) {
    string::size_type i = s.rfind(".txt", s.length());
    if (i != string::npos) {
        out = "\"" + s.replace(i, 4, ".rec") + "\"";
    }
}

void runAllTests() {
    ofstream outFile("recognized.txt");
    
    outFile << "#!MLF!#" << endl;

    vector<string>::iterator inputPath;
    size_t count = 0;
    double percent;
    string outputPath;

    for (inputPath = inputPaths.begin(); inputPath != inputPaths.end(); inputPath++) {
        count++;
        if(count % 60 == 0) {
            percent = (double)count / (double)inputPaths.size() * 100;
            cout << percent << "%.." << endl;
        }
        readTestInput(*inputPath);

        getRecName(*inputPath, outputPath);
        outFile << outputPath << endl;

        vector<string> result;

        runViterbi(length, spectrogram, result);

        vector<string>::iterator word;
        for (word = result.begin(); word != result.end(); word++) {
            if (*word == "<s>") {
                continue;
            }
            outFile << *word << endl;
        }
        outFile << "." << endl;
    }

    cout << "100%" << endl;
}

int main() {
    initAllTransitions();
    listAllInputPaths(inputPaths);
    runAllTests();
    return 0;
}