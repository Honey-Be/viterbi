#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <vector>
#include <string>

using namespace std;

int listFiles (string dir, vector<string> &files);
int listInputFiles(vector<string> &files);

#endif