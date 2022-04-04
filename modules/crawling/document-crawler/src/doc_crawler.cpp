#ifndef DOCUMENT_CRAWLER_CPP
#define DOCUMENT_CRAWLER_CPP

#include "doc_crawler.hpp"
#include "doc_store.hpp"

void DocumentCrawler::start() {
    LOG(INFO) << "Start document crawler";
    std::vector<std::string> foundDocuments = getDocumentPaths();
    for (auto& document_path: foundDocuments) {
        registerDocument(document_path);
    }

    DocStore store = DocStore(documents, doc_store, repository);
    store.processDocuments();
}

std::vector<std::string> DocumentCrawler::getDocumentPaths() {
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

void DocumentCrawler::registerDocument(std::string path) {
    int new_id = documents.size() + 1;
    std::string text = helpers::loadFile(path);
    docmeta::DocumentMeta document = docmeta::DocumentMeta(new_id, text, path);
    LOG(INFO) << "Document to register: " << document;

    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(documents.begin(), documents.end(), [&document](const docmeta::DocumentMeta doc) { 
        LOG(INFO) << "Document with path = " << document.path << " was already crawled"; 
        return doc.path == document.path;
    });

    if (it != documents.end()) {
        if ((it->content) != (document.content)) {
            document.id = it->id;
            documents.erase(it);
        }
    }

    documents.insert(document);
    LOG(INFO) << "Successfully registered document: " << document;
}

#endif