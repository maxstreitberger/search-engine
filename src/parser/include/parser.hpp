#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

struct Parser {
    Parser(std::string_view special_chars_path);

    std::vector<std::string> parse(const std::string_view path);
    
    private:
    std::vector<char> specialchars;
    std::vector<char> readFile(const std::string_view path);
    std::string loadText(const std::string_view path);
};