#ifndef RANKING_HPP
#define RANKING_HPP

#include <iostream>
#include <set>
#include <unordered_set>
#include <fstream>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "../../../include/doc_meta.hpp"
#include "../../../include/token_meta.hpp"
#include "../../../include/helpers.hpp"

struct Ranker {
    Ranker() {};
    Ranker(std::set<docmeta::DocumentMeta>* document_store, std::map<std::string, std::set<tokenmeta::TokenMeta>>* index) : doc_store{document_store}, index{index} {};
    
    std::vector<docmeta::DocumentMeta> searchFor(std::string query);
    std::string transformQuery(std::string query);
    std::vector<tokenmeta::TokenMeta> retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, std::string query);
    std::unordered_set<const docmeta::DocumentMeta*> filterDocPtrs(std::vector<tokenmeta::TokenMeta> tokensMetaInfo);
    std::vector<docmeta::DocumentMeta> collectDocuments(std::unordered_set<const docmeta::DocumentMeta*> doc_ptrs);

    std::set<docmeta::DocumentMeta>* doc_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>>* index;
};

#endif