#ifndef DOC_META_HPP
#define DOC_META_HPP

#include <string>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>

namespace docmeta {
    struct DocumentMeta {
        DocumentMeta() {};
        DocumentMeta(int document_id) : id{document_id} {};
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

    inline void to_json(nlohmann::json& j, const DocumentMeta& doc) {
        j = nlohmann::json{ {"id", doc.id}, {"content", doc.content}, {"path", doc.path} };
    }

    inline void from_json(const nlohmann::json& j, DocumentMeta& doc) {
        j.at("id").get_to(doc.id);
        j.at("content").get_to(doc.content);
        j.at("path").get_to(doc.path);
    }
}

#endif