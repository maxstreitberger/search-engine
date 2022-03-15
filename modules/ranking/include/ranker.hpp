#ifndef RANKING_HPP
#define RANKING_HPP

#include <iostream>
#include <set>
#include <unordered_set>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../../../include/doc_meta.hpp"
#include "../../../include/token_meta.hpp"

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
} 

struct Ranker {
    Ranker() {};
    Ranker(std::string indexPath, std::string storePath): index_path{indexPath}, store_path{storePath} {};
    
    std::vector<docmeta::DocumentMeta> searchFor(std::string query);
    std::string transformQuery(std::string query);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> loadIndex(std::string indexPath);
    std::set<docmeta::DocumentMeta> loadStore(std::string storePath);
    std::vector<tokenmeta::TokenMeta> retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, std::string query);
    std::unordered_set<int> filterDocIds(std::vector<tokenmeta::TokenMeta> tokensMetaInfo);
    std::vector<docmeta::DocumentMeta> collectDocuments(std::set<docmeta::DocumentMeta>* docs, std::unordered_set<int> doc_ids);


    std::string index_path;
    std::string store_path;
};

#endif