#ifndef STRING_HELPER_HPP
#define STRING_HELPER_HPP

#include <string>
#include <algorithm>

namespace stringhelper {
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    static inline void toLower(std::string &s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ 
            return std::tolower(c); 
        });
    }
} 

#endif