#ifndef DOCUMENT_CRAWLER_CPP
#define DOCUMENT_CRAWLER_CPP

#include "pre_computed_doc_crawler.hpp"

void PreComputedDocumentCrawler::start() {
    while (keepRunning->load()) {
        std::vector<std::string> foundDocuments = getDocumentPaths();
        for (auto& document_path: foundDocuments) {
            registerDocument(document_path);
        }
    }
}

std::vector<std::string> PreComputedDocumentCrawler::getDocumentPaths() {
    std::vector<std::string> foundDocuments;
    std::queue<std::string> directories;
    directories.push(origin_path);
    while (!directories.empty()) {
        for (const auto & entry : std::filesystem::directory_iterator(directories.front())) {
            if (std::filesystem::is_directory(entry)) {
                directories.push(entry.path().string());
            } else {
                foundDocuments.push_back(entry.path().string());
            }
        }
        directories.pop();
    }

    return foundDocuments;
}

void PreComputedDocumentCrawler::registerDocument(std::string path) {
    std::string text = helpers::loadFile(path);
    docmeta::DocumentMeta document = docmeta::DocumentMeta(-1, text, path);
    crawler_store_pipeline->push(document);
}

#endif