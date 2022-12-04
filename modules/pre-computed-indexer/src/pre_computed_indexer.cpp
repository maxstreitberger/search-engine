#ifndef PRE_COMPUTED_INDEXER_CPP
#define PRE_COMPUTED_INDEXER_CPP

#include "pre_computed_indexer.hpp"

std::ostream & operator <<(std::ostream &os, const std::map<std::string, std::set<tokenmeta::TokenMeta>> &m) {
    for (const auto &p : m) {
        os << p.first << ": ";
        for (auto x : p.second) os << x << ' ';
        os << std::endl;
    }

    return os;
}

void PreComputedIndexer::generateIndex() {
    specialchars = loadList(special_chars_path);
    stopwords = loadList(stopword_path);
    while (true) {
        const docmeta::DocumentMeta* doc = nullptr;
        repository_pipeline->wait_and_pop(doc);
        if (doc != nullptr) {
            process(doc);
        } else {
            break;
        }
    }
}

void PreComputedIndexer::process(const docmeta::DocumentMeta* doc) {
    std::vector<std::string> tokens = splitTextIntoList(doc->content);
    std::vector<std::string> withoutSpecialChars = removeSpecialChars(tokens, specialchars);
    std::vector<std::string> finalTokens = removeStopwords(withoutSpecialChars, stopwords);

    std::map<std::string, std::set<tokenmeta::TokenMeta>> doc_index = createIndexForDocument(doc, finalTokens);
    updateIndex(index, doc_index);
}

void PreComputedIndexer::updateIndex(std::map<std::string, std::set<tokenmeta::TokenMeta>>* targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex) {
    for (auto const& [key, val] : sourceIndex) {
        (*targetIndex)[key].insert(val.begin(), val.end());
    }
}

std::set<std::string> PreComputedIndexer::loadList(std::string path) {
    std::set<std::string> list;

    std::string line;
    std::ifstream file(path);
    if (file.is_open()) {
        while (std::getline(file,line)) {
            helpers::trim(line);
            list.insert(line);
        }
        file.close();
    }

    return list;
}

std::vector<std::string> PreComputedIndexer::splitTextIntoList(std::string text) {
    std::istringstream tokenStream(text);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(tokenStream, token, ' ')) {
        bool whiteSpacesOnly = std::all_of(token.begin(), token.end(), isspace);
        if(!whiteSpacesOnly) {
            helpers::trim(token);
            helpers::toLower(token);
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::vector<std::string> PreComputedIndexer::removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars) {
    std::vector<std::string> newTokens;
    for (auto& token: tokens) {
        for (const std::string& specialchar: specialChars) {
            char char_array[2];
            strcpy(char_array, specialchar.c_str());
            while (token.find(char_array[0]) != std::string::npos) {
                token.erase(remove(token.begin(), token.end(), char_array[0]), token.end());
            }
        }
        newTokens.push_back(token);
    }
    return newTokens;
}

std::vector<std::string> PreComputedIndexer::removeStopwords(std::vector<std::string> tokens, std::set<std::string> stopwords) {
    std::vector<std::string> newTokens;
    for (auto& token: tokens) {
        if (stopwords.find(token) == stopwords.end()) {
            newTokens.push_back(token);
        }
    }
    return newTokens;
}

std::map<std::string, std::set<tokenmeta::TokenMeta>> PreComputedIndexer::createIndexForDocument(const docmeta::DocumentMeta* doc, std::vector<std::string> tokens) {
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    int counter = 1;
    for (auto token: tokens) {
        std::cout << token << std::endl;
        if (index.find(token) == index.end()) {
            index[token].insert(tokenmeta::TokenMeta(doc->id, counter, doc));
        } else {
            auto it = index[token].find(doc->id);
            if (it == index[token].end()) {
                index[token].insert(tokenmeta::TokenMeta(doc->id, counter, doc));
            } else {
                std::vector<int> current_positions = it->positions;
                current_positions.push_back(counter);
                int current_num = it->num_appearances;

                tokenmeta::TokenMeta newMeta = tokenmeta::TokenMeta(doc->id, ++current_num, current_positions, doc);

                index[token].erase(it);
                index[token].insert(newMeta);
            }
        }
        counter++;
    }

    return index;
}

#endif