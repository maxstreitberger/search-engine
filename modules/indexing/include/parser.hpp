#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <nlohmann/json.hpp>

namespace docmeta {
    struct DocumentMeta {
        DocumentMeta() {};
        DocumentMeta(int document_id, std::string text, std::filesystem::path path) : id{document_id}, content{text}, path{path} {};
    
        const bool operator < ( const DocumentMeta &r ) const {
            return id < r.id;
        }
        
        int id;
        std::string content;
        std::filesystem::path path;

    };

    extern void from_json(const nlohmann::json& j, DocumentMeta& doc);
    extern void to_json(nlohmann::json& j, const DocumentMeta& doc);
}

namespace tokenmeta {
    struct TokenMeta {
        TokenMeta() : document_id{0}, num_appearances{0}, doc_ptr{nullptr} {};
        TokenMeta(int doc_id) : document_id{doc_id}, doc_ptr{nullptr} {};
        TokenMeta(int doc_id, int num_appearances, std::vector<int> positions, docmeta::DocumentMeta* ptr) : document_id{doc_id}, num_appearances{num_appearances}, positions{positions}, doc_ptr{ptr}{};
        TokenMeta(int doc_id , int position, docmeta::DocumentMeta* ptr) : document_id{doc_id}, num_appearances{1}, positions{position}, doc_ptr{ptr} {};

        friend std::ostream& operator<<(std::ostream& os, const TokenMeta& meta) {
            os << " { document_id: " << meta.document_id << ',' << " num_appearances: " << meta.num_appearances << ',' << " positions: [ "; 
            for (int position: meta.positions) {
                os << position << ", ";
            }
            os << " ], ";
            os << "doc_ptr: " << meta.doc_ptr << " }";
            return os << std::endl;
        }

        bool operator< ( const TokenMeta &r ) const {
            return document_id < r.document_id;
        }

        bool operator==(const TokenMeta& rhs) const {
            return document_id == rhs.document_id;
        }

        int document_id;
        docmeta::DocumentMeta* doc_ptr;
        std::vector<int> positions;
        int num_appearances;
    };

    extern void from_json(const nlohmann::json& j, TokenMeta& doc);
    extern void to_json(nlohmann::json& j, const TokenMeta& doc);
}

struct Parser {
    Parser(std::string_view special_chars_path);

    void parse(std::set<docmeta::DocumentMeta>::iterator doc_it);
    std::set<std::string> loadStopWords(const std::string_view path);
    bool checkStopword(std::string_view element);
    void loadCrawlerDocuments();

    std::set<docmeta::DocumentMeta> documents;
    std::set<std::string> stopwords;

    private:
    std::vector<char> specialchars;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    std::vector<char> readFile(const std::string_view path);
};

#endif