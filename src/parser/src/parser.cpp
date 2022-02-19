#include "parser.hpp"

Parser::Parser(std::string_view special_chars_path) {
    specialchars = readFile(special_chars_path);
}

std::vector<char> Parser::readFile(const std::string_view path) {
    std::vector<char> chars;
    std::string line;
    std::ifstream specialfile(path);
    if (specialfile.is_open()) {
        while (std::getline(specialfile,line)) {
            chars.push_back(line[0]);
        }
        specialfile.close();
    } else {
        std::cout << "Unable to open special file" << std::endl;
    } 

    return chars;
}

std::string Parser::loadText(const std::string_view path) {
    std::ifstream ifs(path);
    std::cout << path << std::endl;
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}

std::vector<std::string> Parser::parse(const std::string_view path) {
    std::string document_text = loadText(path);
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(document_text);
    
    while (std::getline(tokenStream, token, ' ')) {
        for (const char& specialchar: specialchars) {
            while (token.find(specialchar)!=std::string::npos) {
                token.pop_back();
            }
        }
        std::transform(token.begin(), token.end(), token.begin(), [](unsigned char c){ return std::tolower(c); });
        tokens.push_back( token);
    }
    return tokens;
}