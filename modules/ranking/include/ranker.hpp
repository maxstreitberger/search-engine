#ifndef RANKING_HPP
#define RANKING_HPP

#include <iostream>
#include <set>
#include <unordered_set>
#include <fstream>
#include <nlohmann/json.hpp>

namespace stringhelper {
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }  
} 

namespace docmeta {
    struct RankerDocumentMeta {
        RankerDocumentMeta() {};
        RankerDocumentMeta(int document_id) : id{document_id} {};
        RankerDocumentMeta(int document_id, std::string text, std::filesystem::path path) : id{document_id}, content{text}, path{path} {};
    
        const bool operator < ( const RankerDocumentMeta &r ) const {
            return id < r.id;
        }

        friend std::ostream& operator << (std::ostream& os, const RankerDocumentMeta &doc) {
            os << "{ document_id: " << doc.id << ", content: '" << doc.content << "' }" << std::endl;
            return os;
        }

        bool operator==(const RankerDocumentMeta& rhs) const {
            return id == rhs.id;
        }
        
        int id;
        std::string content;
        std::filesystem::path path;

    };

    extern void from_json(const nlohmann::json& j, RankerDocumentMeta& doc);
    extern void to_json(nlohmann::json& j, const RankerDocumentMeta& doc);
}

namespace tokenmeta {
    struct RankerTokenMeta {
        RankerTokenMeta() : document_id{0}, num_appearances{0} {};
        RankerTokenMeta(int doc_id) : document_id{doc_id} {};
        RankerTokenMeta(int doc_id , int position) : document_id{doc_id}, num_appearances{1}, positions{position} {};
        RankerTokenMeta(int doc_id, int num_appearances, std::vector<int> positions) : document_id{doc_id}, num_appearances{num_appearances}, positions{positions} {};

        friend std::ostream& operator<<(std::ostream& os, const RankerTokenMeta& meta) {
            os << " { document_id: " << meta.document_id << ',' << " num_appearances: " << meta.num_appearances << ',' << " positions: [ "; 
            for (int position: meta.positions) {
                os << position << ", ";
            }
            os << " ] }";
            return os << std::endl;
        }

        bool operator< ( const RankerTokenMeta &r ) const {
            return document_id < r.document_id;
        }

        bool operator==(const RankerTokenMeta& rhs) const {
            return document_id == rhs.document_id && positions == rhs.positions && num_appearances == rhs.num_appearances;
        }

        int document_id;
        std::vector<int> positions;
        int num_appearances;
    };

    extern void from_json(const nlohmann::json& j, RankerTokenMeta& doc);
    extern void to_json(nlohmann::json& j, const RankerTokenMeta& doc);
}

struct Ranker {
    Ranker() {};
    Ranker(std::string indexPath, std::string storePath): index_path{indexPath}, store_path{storePath} {};
    
    std::vector<docmeta::RankerDocumentMeta> searchFor(std::string query);
    std::string transformQuery(std::string query);
    std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> loadIndex(std::string indexPath);
    std::set<docmeta::RankerDocumentMeta> loadStore(std::string storePath);
    std::vector<tokenmeta::RankerTokenMeta> retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::RankerTokenMeta>>* index, std::string query);
    std::unordered_set<int> filterDocIds(std::vector<tokenmeta::RankerTokenMeta> tokensMetaInfo);
    std::vector<docmeta::RankerDocumentMeta> collectDocuments(std::set<docmeta::RankerDocumentMeta>* docs, std::unordered_set<int> doc_ids);


    std::string index_path;
    std::string store_path;
};

#endif