#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <iostream>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <queue>
#include <set>
#include <nlohmann/json.hpp>

namespace docmeta {
    struct CrawlerDocMeta {
        CrawlerDocMeta() {};
        CrawlerDocMeta(int document_id, std::string text, std::filesystem::path path) : id{document_id}, content{text}, path{path} {};

        const bool operator < (const CrawlerDocMeta &r) const {
            return path < r.path;
        }

        friend std::ostream& operator << (std::ostream& os, const CrawlerDocMeta &doc) {
            os << "{ document_id: " << doc.id << ", content: '" << doc.content << ", path: " << doc.path << "' }" << std::endl;
            return os;
        }

        const bool operator == (const CrawlerDocMeta &r) const {
            return id == r.id && content == r.content && path == r.path;
        }

        int id;
        std::string content;
        std::filesystem::path path;
    };

    extern void from_json(const nlohmann::json& j, CrawlerDocMeta& doc);
    extern void to_json(nlohmann::json& j, const CrawlerDocMeta& doc);
}

struct Crawler {
    Crawler() {};
    Crawler(std::string path) : origin_path{path} {};
    
    void start();
    std::vector<std::string> getDocumentPaths();
    std::string getDocumentContents(const std::string path);
    void registerDocument(const std::filesystem::path path);
    
    void pushDocumentsToStore(const std::set<docmeta::CrawlerDocMeta> documents);
    std::string loadText(const std::string path);

    std::string origin_path;
    std::set<docmeta::CrawlerDocMeta> documents;
};


#endif