#ifndef DOCUMENT_CRAWLER_CPP
#define DOCUMENT_CRAWLER_CPP

#include "pre_computed_doc_crawler.hpp"

void PreComputedDocumentCrawler::start() {
    LOG(INFO) << "Start document crawler";
    while(1) {
        std::vector<std::string> foundDocuments = getDocumentPaths();
        for (auto& document_path: foundDocuments) {
            registerDocument(document_path);
        }
    }
}

std::vector<std::string> PreComputedDocumentCrawler::getDocumentPaths() {
    LOG(INFO) << "Get document paths";
    std::vector<std::string> foundDocuments;
    std::queue<std::string> directories;
    directories.push(origin_path);

    while (!directories.empty()) {
        LOG(INFO) << "Crawl path: " << directories.front();
        for (const auto & entry : std::filesystem::directory_iterator(directories.front())) {
            if (std::filesystem::is_directory(entry)) {
                LOG(INFO) << "Found directory: " << entry.path();
                directories.push(entry.path().string());
            } else {
                LOG(INFO) << "Found file: " << entry.path();
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
    LOG(INFO) << "Successfully registered document: " << document;
}

#endif