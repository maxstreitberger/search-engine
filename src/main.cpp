#include <iostream>
#include "parser.hpp"

int main() {
    Parser parser = Parser("./src/parser/documents/special.txt");
    std::vector<std::string> return_val = parser.parse("documents/demo.txt");

    for (std::string& str: return_val) {
        std::cout << str << std::endl;
    }

    return 0;
}
