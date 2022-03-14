#ifndef INDEXER_HPP
#define INDEXER_HPP

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

        friend std::ostream& operator << (std::ostream& os, const DocumentMeta &doc) {
            os << "{ document_id: " << doc.id << ", content: '" << doc.content << ", path: " << doc.path << "' }" << std::endl;
            return os;
        }

        const bool operator == (const DocumentMeta &r) const {
            return id == r.id && content == r.content && path == r.path;
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
        TokenMeta(int doc_id , int position, docmeta::DocumentMeta* ptr) : document_id{doc_id}, num_appearances{1}, positions{position}, doc_ptr{ptr} {};
        TokenMeta(int doc_id, int num_appearances, std::vector<int> positions, docmeta::DocumentMeta* ptr) : document_id{doc_id}, num_appearances{num_appearances}, positions{positions}, doc_ptr{ptr}{};

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
            return document_id == rhs.document_id && doc_ptr == rhs.doc_ptr && positions == rhs.positions && num_appearances == rhs.num_appearances;
        }

        int document_id;
        docmeta::DocumentMeta* doc_ptr;
        std::vector<int> positions;
        int num_appearances;
    };

    extern void from_json(const nlohmann::json& j, TokenMeta& doc);
    extern void to_json(nlohmann::json& j, const TokenMeta& doc);
}

struct Indexer {
    Indexer() {}
    Indexer(std::string specialCharsPath, std::string stopwordPath, std::string repoPath, std::string indexPath) : special_chars_path{specialCharsPath}, stopword_path{stopwordPath}, repo_path{repoPath}, index_path{indexPath} {}

    void generateIndex();
    std::vector<docmeta::DocumentMeta> loadRepository(std::string path);
    std::set<std::string> loadList(std::string path);
    std::vector<std::string> splitTextIntoList(std::string text);
    std::vector<std::string> removeSpecialChars(std::vector<std::string> tokens, std::set<std::string> specialChars);
    std::vector<std::string> removeStopwords(std::vector<std::string> tokens, std::set<std::string> stopwords);
    std::vector<std::string> createTokens(std::string text, std::set<std::string> specialChars, std::set<std::string> stopwords);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> createIndexForDocument(docmeta::DocumentMeta* doc, std::vector<std::string> tokens);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> joinIndexes(std::map<std::string, std::set<tokenmeta::TokenMeta>> targetIndex, std::map<std::string, std::set<tokenmeta::TokenMeta>> sourceIndex);
    void writeIndexToFile(std::map<std::string, std::set<tokenmeta::TokenMeta>> index, std::string path);

    std::string special_chars_path;
    std::string stopword_path;
    std::string repo_path;
    std::string index_path;
};

#endif