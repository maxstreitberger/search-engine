#ifndef CONCURRENT_INDEXER_HPP
#define CONCURRENT_INDEXER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <cstring>
#include <atomic>
#include <optional>

#include "../../../include/doc_meta.hpp"
#include "../../../include/token_meta.hpp"
#include "../../../include/helpers.hpp"
#include "../../../include/concurrent_thread_queue.hpp"

struct ConcurrentIndexer {
    ConcurrentIndexer() {}
    ConcurrentIndexer(std::string specialCharsPath, std::string stopwordPath) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath} {}
    ConcurrentIndexer(std::string specialCharsPath, std::string stopwordPath, ConcurrentThreadQueue<const docmeta::DocumentMeta*>* pipeline, std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, std::atomic<bool>* index_flag) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath}, repository_pipeline{pipeline}, index{index}, index_flag{index_flag} {}

    void generateIndex();
    void process(const docmeta::DocumentMeta* doc);
    std::set<std::string> loadList(std::string path);
    std::vector<std::string> splitTextIntoList(std::string text);
    std::vector<std::string> removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars);
    std::vector<std::string> removeStopwords(std::vector<std::string> tokens, std::set<std::string> stopwords);
    std::vector<std::string> createTokens(std::string text, std::set<std::string> specialChars, std::set<std::string> stopwords);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> createIndexForDocument(const docmeta::DocumentMeta* doc, std::vector<std::string> tokens);
    void updateIndex(std::map<std::string, std::set<tokenmeta::TokenMeta>>* targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex);

    std::string special_chars_path;
    std::string stopword_path;
    std::set<std::string> specialchars;
    std::set<std::string> stopwords;

    ConcurrentThreadQueue<const docmeta::DocumentMeta*>* repository_pipeline;
    std::map<std::string, std::set<tokenmeta::TokenMeta>>* index;
    std::atomic<bool>* index_flag;
};

#endif