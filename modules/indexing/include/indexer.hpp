#ifndef INDEXER_HPP
#define INDEXER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "../../../include/doc_meta.hpp"
#include "../../../include/token_meta.hpp"
#include "../../../include/helper/string_helper.hpp"

struct Indexer {
    Indexer() {}
    Indexer(std::string specialCharsPath, std::string stopwordPath) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath} {}
    Indexer(std::string specialCharsPath, std::string stopwordPath, std::vector<docmeta::DocumentMeta>* repository, std::map<std::string, std::set<tokenmeta::TokenMeta>>* index) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath}, repository{repository}, index{index} {}

    void generateIndex();
    std::set<std::string> loadList(std::string path);
    std::vector<std::string> splitTextIntoList(std::string text);
    std::vector<std::string> removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars);
    std::vector<std::string> removeStopwords(std::vector<std::string> tokens, std::set<std::string> stopwords);
    std::vector<std::string> createTokens(std::string text, std::set<std::string> specialChars, std::set<std::string> stopwords);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> createIndexForDocument(docmeta::DocumentMeta* doc, std::vector<std::string> tokens);
    void updateIndex(std::map<std::string, std::set<tokenmeta::TokenMeta>>* targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex);

    std::string special_chars_path;
    std::string stopword_path;
    std::vector<docmeta::DocumentMeta>* repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>>* index;
};

#endif