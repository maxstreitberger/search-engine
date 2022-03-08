#ifndef RANKING_HPP
#define RANKING_HPP

#include <iostream>
#include <set>
#include <fstream>
#include <nlohmann/json.hpp>

namespace stringhelper {
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }  
} 

namespace docmeta {
    struct RankingDocumentMeta {
        RankingDocumentMeta() {};
        RankingDocumentMeta(int document_id, std::string text, std::filesystem::path path) : id{document_id}, content{text}, path{path} {};
    
        const bool operator < ( const RankingDocumentMeta &r ) const {
            return id < r.id;
        }

        friend std::ostream& operator << (std::ostream& os, const RankingDocumentMeta &doc) {
            os << "{ document_id: " << doc.id << ", content: '" << doc.content << "' }" << std::endl;
            return os;
        }
        
        int id;
        std::string content;
        std::filesystem::path path;

    };

    extern void from_json(const nlohmann::json& j, RankingDocumentMeta& doc);
    extern void to_json(nlohmann::json& j, const RankingDocumentMeta& doc);
}

namespace tokenmeta {
    struct RankingTokenMeta {
        RankingTokenMeta() : document_id{0}, num_appearances{0} {};
        RankingTokenMeta(int doc_id) : document_id{doc_id} {};
        RankingTokenMeta(int doc_id, int num_appearances, std::vector<int> positions) : document_id{doc_id}, num_appearances{num_appearances}, positions{positions} {};
        RankingTokenMeta(int doc_id , int position) : document_id{doc_id}, num_appearances{1}, positions{position} {};

        friend std::ostream& operator<<(std::ostream& os, const RankingTokenMeta& meta) {
            os << " { document_id: " << meta.document_id << ',' << " num_appearances: " << meta.num_appearances << ',' << " positions: [ "; 
            for (int position: meta.positions) {
                os << position << ", ";
            }
            os << " ] }";
            return os << std::endl;
        }

        bool operator< ( const RankingTokenMeta &r ) const {
            return num_appearances > r.num_appearances;
        }

        bool operator==(const RankingTokenMeta& rhs) const {
            return document_id == rhs.document_id;
        }

        int document_id;
        std::vector<int> positions;
        int num_appearances;
    };

    extern void from_json(const nlohmann::json& j, RankingTokenMeta& doc);
    extern void to_json(nlohmann::json& j, const RankingTokenMeta& doc);
}

struct Ranking {
    Ranking() {};
    
    std::vector<docmeta::RankingDocumentMeta> searchFor(std::string query);
    std::set<tokenmeta::RankingTokenMeta> getTokens(std::string query);
    std::vector<docmeta::RankingDocumentMeta> collectDocuments(std::set<tokenmeta::RankingTokenMeta> tokens);
};

#endif