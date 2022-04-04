#ifndef INDEXER_CPP
#define INDEXER_CPP

#include "indexer.hpp"

std::ostream & operator <<(std::ostream &os, const std::map<std::string, std::set<tokenmeta::TokenMeta>> &m) {
    for (const auto &p : m) {
        os << p.first << ": ";
        for (auto x : p.second) os << x << ' ';
        os << std::endl;
    }

    return os;
}

void Indexer::generateIndex() {
    LOG(INFO) << "Start indexing";
    std::set<std::string> specialchars = loadList(special_chars_path);
    std::set<std::string> stopwords = loadList(SEARCHENGINE_ROOT_DIR "/modules/indexing/documents/stopwords.txt");

    for (auto& document: *repository) {
        LOG(INFO) << "Process document with id: " << document.id;
        std::vector<std::string> tokens = splitTextIntoList(document.content);
        std::vector<std::string> withoutSpecialChars = removeSpecialChars(tokens, specialchars);
        std::vector<std::string> finalTokens = removeStopwords(withoutSpecialChars, stopwords);

        std::map<std::string, std::set<tokenmeta::TokenMeta>> doc_index = createIndexForDocument(&document, finalTokens);
        updateIndex(index, doc_index);
    }
    
    LOG(INFO) << *index;
}

void Indexer::updateIndex(std::map<std::string, std::set<tokenmeta::TokenMeta>>* targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex) {
    LOG(INFO) << "Update index";
    for (auto const& [key, val] : sourceIndex) {
        (*targetIndex)[key].insert(val.begin(), val.end());
    }
}

std::set<std::string> Indexer::loadList(std::string path) {
    LOG(INFO) << "Load file from path: " << path;
    std::set<std::string> list;

    std::string line;
    std::ifstream file(path);
    if (file.is_open()) {
        while (std::getline(file,line)) {
            helpers::trim(line);
            list.insert(line);
        }
        file.close();
    } else {
        LOG(ERROR) << "Unable to open file at path: " << path;
    } 

    return list;
}

std::vector<std::string> Indexer::splitTextIntoList(std::string text) {
    LOG(INFO) << "Split text into list";
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

std::vector<std::string> Indexer::removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars) {
    LOG(INFO) << "Remove special characters";
    std::vector<std::string> newTokens;
    for (auto& token: tokens) {
        for (const std::string& specialchar: specialChars) {
            char char_array[1];
            strcpy(char_array, specialchar.c_str());
            while (token.find(char_array[0]) != std::string::npos) {
                token.erase(remove(token.begin(), token.end(), char_array[0]), token.end());
            }
        }
        newTokens.push_back(token);
    }
    return newTokens;
}

std::vector<std::string> Indexer::removeStopwords(std::vector<std::string> tokens, std::set<std::string> stopwords) {
    LOG(INFO) << "Remove stopwords";
    std::vector<std::string> newTokens;
    for (auto& token: tokens) {
        if (stopwords.find(token) == stopwords.end()) {
            newTokens.push_back(token);
        }
    }
    return newTokens;
}

std::map<std::string, std::set<tokenmeta::TokenMeta>> Indexer::createIndexForDocument(docmeta::DocumentMeta* doc, std::vector<std::string> tokens) {
    LOG(INFO) << "Create index";
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    int counter = 1;
    LOG(INFO) << "Process tokens";
    for (auto token: tokens) {
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