#ifndef PRE_COMPUTED_INDEXER_HPP
#define PRE_COMPUTED_INDEXER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#include <glog/logging.h>

#include "../../../include/doc_meta.hpp"
#include "../../../include/token_meta.hpp"
#include "../../../include/helpers.hpp"
#include "../../../include/thread_queue.hpp"

struct PreComputedIndexer {
    PreComputedIndexer() {}
    PreComputedIndexer(std::string specialCharsPath, std::string stopwordPath) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath} {}
    PreComputedIndexer(std::string specialCharsPath, std::string stopwordPath, ThreadQueue<docmeta::DocumentMeta>* pipeline, std::map<std::string, std::set<tokenmeta::TokenMeta>>* index) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath}, repository_pipeline{pipeline}, index{index} {}

    void generateIndex();
    void process(docmeta::DocumentMeta doc);
    std::set<std::string> loadList(std::string path);
    std::vector<std::string> splitTextIntoList(std::string text);
    std::vector<std::string> removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars);
    std::vector<std::string> removeStopwords(std::vector<std::string> tokens, std::set<std::string> stopwords);
    std::vector<std::string> createTokens(std::string text, std::set<std::string> specialChars, std::set<std::string> stopwords);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> createIndexForDocument(docmeta::DocumentMeta* doc, std::vector<std::string> tokens);
    void updateIndex(std::map<std::string, std::set<tokenmeta::TokenMeta>>* targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex);

    std::string special_chars_path;
    std::string stopword_path;
    std::set<std::string> specialchars;
    std::set<std::string> stopwords;

    ThreadQueue<docmeta::DocumentMeta>* repository_pipeline;
    std::map<std::string, std::set<tokenmeta::TokenMeta>>* index;
};

#endif