#ifndef FILEUTIL_H_
#define FILEUTIL_H_

#include <vector>
#include <string>

using namespace std;

int listFilePaths (string dir, vector<string> &paths);
int listAllInputPaths (vector<string> &paths);

#endif
