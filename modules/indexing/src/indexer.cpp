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

namespace tokenmeta {
    void to_json(nlohmann::json& j, const TokenMeta& token) {
        // j = nlohmann::json{ {"doc_id", token.document_id}, {"doc_ptr", token.doc_ptr}, {"num_appearances", token.num_appearances}, {"positions", token.positions} };
        j = nlohmann::json{ {"doc_id", token.document_id}, {"num_appearances", token.num_appearances}, {"positions", token.positions} };
    }

    void from_json(const nlohmann::json& j, TokenMeta& token) {
        j.at("doc_id").get_to(token.document_id);
        // j.at("doc_ptr").get_to(token.doc_ptr);
        j.at("num_appearances").get_to(token.num_appearances);
        j.at("positions").get_to(token.positions);
    }
}

Indexer::Indexer(std::string_view special_chars_path) {
    specialchars = readFile(special_chars_path);
    stopwords = loadStopWords(SEARCHENGINE_ROOT_DIR "/modules/indexing/documents/stopwords.txt");
}

std::vector<char> Indexer::readFile(const std::string_view path) {
    std::vector<char> chars;
    std::string line;
    std::ifstream specialfile(path);
    if (specialfile.is_open()) {
        while (std::getline(specialfile,line)) {
            chars.push_back(line[0]);
        }
        specialfile.close();
    } else {
        std::cout << "Unable to open special file" << std::endl;
    } 

    return chars;
}

std::set<std::string> Indexer::loadStopWords(const std::string_view path) {
    std::string line;
    std::set<std::string> stopwords;
    std::ifstream stopwordsFile(path);
    if (stopwordsFile.is_open()) {
        while (getline(stopwordsFile,line)) {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            stopwords.insert(line);
        }
        stopwordsFile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
    return stopwords;
}

std::ostream & operator <<(std::ostream &os, const std::map<std::string, std::set<tokenmeta::TokenMeta>> &m) {
    for (const auto &p : m) {
        os << p.first << ": ";
        for (auto x : p.second) os << x << ' ';
        os << std::endl;
    }

    return os;
}

void Indexer::loadCrawlerDocuments() {
    std::ifstream ifs("repository.json");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json crawler_docs;

    try {
        crawler_docs = nlohmann::json::parse(content);
        documents = crawler_docs.get<std::set<docmeta::DocumentMeta>>();
        // std::cout << std::setw(4) << crawler_docs << std::endl;

    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }
}

void Indexer::parse(std::set<docmeta::DocumentMeta>::iterator doc_it) {
    docmeta::DocumentMeta* doc = (docmeta::DocumentMeta*)&(*doc_it);

    nlohmann::json indexJson;
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(doc->content);

    while (std::getline(tokenStream, token, ' ')) {
        bool whiteSpacesOnly = std::all_of(token.begin(), token.end(), isspace);
        if(!whiteSpacesOnly) {        
            for (const char& specialchar: specialchars) {
                while (token.find(specialchar)!=std::string::npos) {
                    token.pop_back();
                }
            }
            token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
            std::transform(token.begin(), token.end(), token.begin(), [](unsigned char c){ return std::tolower(c); });
            tokens.push_back(token);
        }
    }

    std::vector<std::string>::iterator it = tokens.begin();
    while (it != tokens.end()) {
        if (stopwords.find(*it) != stopwords.end()) {
            it = tokens.erase(it);
        }
        else ++it;
    }

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

    std::ofstream file("index.json");
    indexJson = index;
    file << std::setw(4) << indexJson;
    file.close();
}

#endif