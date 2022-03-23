#ifndef PAGE_META_HPP
#define PAGE_META_HPP

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

namespace pagemeta {
    struct PageMeta {
        PageMeta() {};
        PageMeta(int page_id) : id{page_id} {};
        PageMeta(int page_id, std::string text, std::string path) : id{page_id}, content{text}, path{path} {};
    
        const bool operator < ( const PageMeta &r ) const {
            return id < r.id;
        }

        friend std::ostream& operator << (std::ostream& os, const PageMeta &doc) {
            os << "{ page_id: " << doc.id << ", content: '" << doc.content << ", path: " << doc.path << "' }" << std::endl;
            return os;
        }

        const bool operator == (const PageMeta &r) const {
            return id == r.id && content == r.content && path == r.path;
        }
        
        int id;
        std::string content;
        std::string path;

    };

    inline void to_json(nlohmann::json& j, const PageMeta& doc) {
        j = nlohmann::json{ {"id", doc.id}, {"content", doc.content}, {"path", doc.path} };
    }

    inline void from_json(const nlohmann::json& j, PageMeta& doc) {
        j.at("id").get_to(doc.id);
        j.at("content").get_to(doc.content);
        j.at("path").get_to(doc.path);
    }
}

#endif