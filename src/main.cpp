#include <iostream>
#include "parser.hpp"

#ifndef MAIN_CPP
#define MAIN_CPP

int main() {
    Parser parser = Parser("./modules/indexing/documents/special.txt");
    parser.registerDocument("dummy-text/demo.txt");
    parser.registerDocument("dummy-text/demo2.txt");

    std::set<DocumentMeta>::iterator it;

    for(it = parser.documents.begin(); it != parser.documents.end(); it++) {
        parser.parse(it);
    }

    return 0;
}

#endif