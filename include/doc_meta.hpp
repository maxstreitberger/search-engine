#ifndef DOC_META_HPP
#define DOC_META_HPP

#include <string>
#include <filesystem>
#include <iostream>

namespace docmeta {
    struct DocumentMeta {
        DocumentMeta() {};
        DocumentMeta(int document_id) : id{document_id} {};
        DocumentMeta(int document_id, std::string text, std::string path) : id{document_id}, content{text}, path{path} {};
    
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
        std::string path;

    };
}

#endif