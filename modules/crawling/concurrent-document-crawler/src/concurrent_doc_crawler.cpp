#ifndef DOCUMENT_CRAWLER_CPP
#define DOCUMENT_CRAWLER_CPP

#include "concurrent_doc_crawler.hpp"

void ConcurrentDocumentCrawler::start() {
    std::vector<std::string> foundDocuments;
    std::queue<std::string> directories;
    directories.push(origin_path);
    while (!directories.empty()) {
        for (const auto & entry : std::filesystem::directory_iterator(directories.front())) {
            if (std::filesystem::is_directory(entry)) {
                directories.push(entry.path().string());
            } else {
                registerDocument(entry.path().string());
            }
        }
        directories.pop();
    }
    crawler_flag->store(false);
}

void ConcurrentDocumentCrawler::registerDocument(std::string path) {
    std::string text = helpers::loadFile(path);
    docmeta::DocumentMeta document = docmeta::DocumentMeta(-1, text, path);
    crawler_store_pipeline->push(document);
}

#endif