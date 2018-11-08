#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include "fileutil.h"

using namespace std;

int listFilePaths (string dir, vector<string> &paths) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_DIR) {
            if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) continue;
            string childDir = dir + "/" + string(dirp->d_name);
            listFilePaths(childDir, paths);
        } else {
            paths.push_back(dir + "/" + string(dirp->d_name));
        }
    }
    closedir(dp);
    return 0;
}

int listAllInputPaths(vector<string> &paths) {
    string testDir = string("tst");
    return listFilePaths(testDir, paths);
}