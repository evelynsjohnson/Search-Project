#include <iostream>
#include <string>

#include "search.h"

using namespace std;

int main() {
    string filename;
    getline(cin, filename);

    searchEngine(filename);
    return 0;
}
