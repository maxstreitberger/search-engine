#include "parser.hpp"

#ifndef PARSER_CPP
#define PARSER_CPP

Parser::Parser(std::string_view special_chars_path) {
    specialchars = readFile(special_chars_path);
    stopwords = loadStopWords("./src/parser/documents/stopwords.txt");
}

std::vector<char> Parser::readFile(const std::string_view path) {
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

void Parser::registerDocument(const std::string_view path) {
    int new_id = documents.size() + 1;
    std::string text = loadText(path);
    DocumentMeta document = DocumentMeta(new_id, text);
    documents.insert(document);
}

std::string Parser::loadText(const std::string_view path) {
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}

std::set<std::string> Parser::loadStopWords(const std::string_view path) {
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

std::ostream & operator <<(std::ostream &os, const std::map<std::string, std::set<TokenMeta>> &m) {
    for (const auto &p : m) {
        os << p.first << ": ";
        for (auto x : p.second) os << x << ' ';
        os << std::endl;
    }

    return os;
}

void Parser::parse(std::set<DocumentMeta>::iterator doc_it) {
    DocumentMeta* doc = (DocumentMeta*)&(*doc_it);

    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(doc->content);

    while (std::getline(tokenStream, token, ' ')) {
        for (const char& specialchar: specialchars) {
            while (token.find(specialchar)!=std::string::npos) {
                token.pop_back();
            }
        }
        std::transform(token.begin(), token.end(), token.begin(), [](unsigned char c){ return std::tolower(c); });
        tokens.push_back(token);
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
            index[token].insert(TokenMeta(doc->id, counter, doc));
        } else {
            // std::cout << "Found index entry" << std::endl;
            auto it = index[token].find(doc->id);
            if (it == index[token].end()) {
                // std::cout << "No entry for doc_id: " << doc.id << std::endl;
                index[token].insert(TokenMeta(doc->id, counter, doc));
            } else {
                // std::cout << "Update entry for doc_id: " << doc.id << std::endl;
                std::vector<int> current_positions = it->positions;
                current_positions.push_back(counter);
                int current_num = it->num_appearances;

                TokenMeta newMeta = TokenMeta(doc->id, ++current_num, current_positions, doc);

                index[token].erase(it);
                index[token].insert(newMeta);
            }
        }
        counter++;
    }



    std::ofstream file("test.txt");
    file << index;
    file.close();
}

#endif