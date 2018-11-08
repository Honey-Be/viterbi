#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <vector>
#include <string>

using namespace std;

int listFilePaths (string dir, vector<string> &files);
int listAllInputPaths (vector<string> &files);

#endif