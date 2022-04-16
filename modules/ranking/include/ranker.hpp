#ifndef RANKING_HPP
#define RANKING_HPP

#include <iostream>
#include <set>
#include <unordered_set>
#include <fstream>
#include <map>

#include <glog/logging.h>

#include "doc_meta.hpp"
#include "token_meta.hpp"
#include "helpers.hpp"

struct Ranker {
    Ranker(int max_return_document_count = 0) : max_return_document_count{max_return_document_count} {};
    Ranker(std::set<docmeta::DocumentMeta>* document_store, std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, int max_return_document_count = 0) : doc_store{document_store}, index{index}, max_return_document_count{max_return_document_count} {};
    
    std::vector<docmeta::DocumentMeta> searchFor(std::string query);
    std::string transformQuery(std::string query);
    std::vector<tokenmeta::TokenMeta> retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, std::string query);
    std::vector<const docmeta::DocumentMeta*> filterDocPtrs(std::vector<tokenmeta::TokenMeta> tokensMetaInfo);
    std::vector<docmeta::DocumentMeta> collectDocuments(std::vector<const docmeta::DocumentMeta*> doc_ptrs);

    std::set<docmeta::DocumentMeta>* doc_store;
    std::map<std::string, std::set<tokenmeta::TokenMeta>>* index;
    int max_return_document_count;
};

#endif