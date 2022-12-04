#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <algorithm>
#include <fstream>

namespace helpers {
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    inline void toLower(std::string &s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ 
            return std::tolower(c); 
        });
    }

    inline std::string loadFile(std::string &path) {
        std::ifstream file(path);
        std::string text((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        return text;
    }
} 

#endif