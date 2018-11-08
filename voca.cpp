#include <fstream>
#include <iostream>
#include <vector>
#include "voca.h"

using namespace std;

vocaType vocas[N_VOCA] = {
    {"<s>", 1, {"sil"}},
    {"eight", 3, {"ey", "t", "sp"}},
    {"five", 4, {"f", "ay", "v", "sp"}},
    {"four", 4, {"f", "ao", "r", "sp"}},
    {"nine", 4, {"f", "ay", "n", "sp"}},
    {"oh", 2, {"ow", "sp"}},
    {"one", 4, {"w", "ah", "n", "sp"}},
    {"seven", 6, {"s", "eh", "v", "ah", "n", "sp"}},
    {"six", 5, {"s", "ih", "k", "s", "sp"}},
    {"three", 4, {"th", "r", "iy", "sp"}},
    {"two", 3, {"t", "uw", "sp"}},
    {"zero", 5, {"z", "ih", "r", "ow", "sp"}},
    {"zero", 5, {"z", "iy", "r", "ow", "sp"}}
};

double unigram[N_VOCA];
double bigram[N_VOCA][N_VOCA];

void findIndexesByName(string name, vector<int> &indexes) {
    indexes.clear();
    for (int index = 0; index < N_VOCA; index++) {
        if (vocas[index].name == name) {
            indexes.push_back(index);
        }
    }
}

void readUnigram() {
    ifstream in;
    in.open("unigram.txt", ios::in);

    string voca = "";
    double prob = 0;
    vector<int> indexes;
    vector<int>::iterator index;

    while(!in.eof()) {
        prob = -1;
        in >> voca >> prob;
        if (prob < 0) break;

        findIndexesByName(voca, indexes);
        for(index = indexes.begin(); index != indexes.end(); index++) {
            unigram[*index] = prob * WORD_TRANS_WEIGHT;
        }
        
    }

    in.close();
}

void readBigram() {
    ifstream in;
    in.open("bigram.txt", ios::in);
    string voca1 = "", voca2 = "";
    double prob = 0;
    vector<int> indexes1, indexes2;
    vector<int>::iterator source, dest;

    while(!in.eof()) {
        prob = -1;
        in >> voca1 >> voca2 >> prob;
        if (prob < 0) break;

        findIndexesByName(voca1, indexes1);
        findIndexesByName(voca2, indexes2);     
        for (source = indexes1.begin(); source != indexes1.end(); source++) {
            for (dest = indexes2.begin(); dest != indexes2.end(); dest++) {
                bigram[*source][*dest] = prob * WORD_TRANS_WEIGHT;
            }
        }
    }

    in.close();
}
