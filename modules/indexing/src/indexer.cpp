#ifndef INDEXER_CPP
#define INDEXER_CPP

#include "indexer.hpp"

namespace docmeta {
    void to_json(nlohmann::json& j, const DocumentMeta& doc) {
        j = nlohmann::json{ {"id", doc.id}, {"content", doc.content}, {"path", doc.path} };
    }

    void from_json(const nlohmann::json& j, DocumentMeta& doc) {
        j.at("id").get_to(doc.id);
        j.at("content").get_to(doc.content);
        j.at("path").get_to(doc.path);
    }
}

std::ostream & operator <<(std::ostream &os, const std::map<std::string, std::set<tokenmeta::TokenMeta>> &m) {
    for (const auto &p : m) {
        os << p.first << ": ";
        for (auto x : p.second) os << x << ' ';
        os << std::endl;
    }

    return os;
}

namespace tokenmeta {
    void to_json(nlohmann::json& j, const TokenMeta& token) {
        j = nlohmann::json{ {"doc_id", token.document_id}, {"num_appearances", token.num_appearances}, {"positions", token.positions} };
    }

    void from_json(const nlohmann::json& j, TokenMeta& token) {
        j.at("doc_id").get_to(token.document_id);
        j.at("num_appearances").get_to(token.num_appearances);
        j.at("positions").get_to(token.positions);
    }
}

void Indexer::generateIndex() {
    std::set<std::string> specialchars = loadList(special_chars_path);
    std::set<std::string> stopwords = loadList(SEARCHENGINE_ROOT_DIR "/modules/indexing/documents/stopwords.txt");
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::vector<docmeta::DocumentMeta> documents = loadRepository(repo_path);

    for (auto& document: documents) {
        std::vector<std::string> tokens = splitTextIntoList(document.content);
        std::vector<std::string> withoutSpecialChars = removeSpecialChars(tokens, specialchars);
        std::vector<std::string> finalTokens = removeStopwords(withoutSpecialChars, stopwords);

        std::map<std::string, std::set<tokenmeta::TokenMeta>> doc_index = createIndexForDocument(&document, finalTokens);
        
        index = joinIndexes(index, doc_index);  
    }

    writeIndexToFile(index, index_path);
}

std::map<std::string, std::set<tokenmeta::TokenMeta>> Indexer::joinIndexes(std::map<std::string, std::set<tokenmeta::TokenMeta>> targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex) {
    for (auto const& [key, val] : sourceIndex) {
        targetIndex[key].insert(val.begin(), val.end());
    }
    return targetIndex;
}

std::set<std::string> Indexer::loadList(std::string path) {
    std::set<std::string> list;

    std::string line;
    std::ifstream file(path);
    if (file.is_open()) {
        while (std::getline(file,line)) {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            list.insert(line);
        }
        file.close();
    } else {
        std::cout << "Unable to open file at path: " << path << std::endl;
    } 

    return list;
}

std::vector<docmeta::DocumentMeta> Indexer::loadRepository(std::string path) {
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json crawler_docs;
    std::vector<docmeta::DocumentMeta> docmentsToIndex;

    try {
        crawler_docs = nlohmann::json::parse(content);
        docmentsToIndex = crawler_docs.get<std::vector<docmeta::DocumentMeta>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    return docmentsToIndex;
}

std::vector<std::string> Indexer::splitTextIntoList(std::string text) {
    std::istringstream tokenStream(text);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(tokenStream, token, ' ')) {
        bool whiteSpacesOnly = std::all_of(token.begin(), token.end(), isspace);
        if(!whiteSpacesOnly) {
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            std::transform(token.begin(), token.end(), token.begin(), [](unsigned char c){ return std::tolower(c); });
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::vector<std::string> Indexer::removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars) {
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
    std::vector<std::string> newTokens;
    for (auto& token: tokens) {
        if (stopwords.find(token) == stopwords.end()) {
            newTokens.push_back(token);
        }
    }
    return newTokens;
}

std::vector<std::string> Indexer::createTokens(std::string text, std::set<std::string> specialChars, std::set<std::string> stopwords) {
    std::vector<std::string> splittedWords = splitTextIntoList(text);
    std::vector<std::string> withoutSpecialChars = removeSpecialChars(splittedWords, specialChars);
    return removeStopwords(withoutSpecialChars, stopwords);
}

std::map<std::string, std::set<tokenmeta::TokenMeta>> Indexer::createIndexForDocument(docmeta::DocumentMeta* doc, std::vector<std::string> tokens) {
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    int counter = 1;
    for (auto token: tokens) {
        // std::cout << token << std::endl;
        if (index.find(token) == index.end()) {
            // std::cout << "Not in index" << std::endl;
            index[token].insert(tokenmeta::TokenMeta(doc->id, counter, doc));
        } else {
            // std::cout << "Found index entry" << std::endl;
            auto it = index[token].find(doc->id);
            if (it == index[token].end()) {
                // std::cout << "No entry for doc_id: " << doc.id << std::endl;
                index[token].insert(tokenmeta::TokenMeta(doc->id, counter, doc));
            } else {
                // std::cout << "Update entry for doc_id: " << doc.id << std::endl;
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

void Indexer::writeIndexToFile(std::map<std::string, std::set<tokenmeta::TokenMeta>> index, std::string path) {
    nlohmann::json indexJson;
    std::ofstream file(path);
    indexJson = index;
    file << indexJson.dump(4);
    file.close();
}

#endif