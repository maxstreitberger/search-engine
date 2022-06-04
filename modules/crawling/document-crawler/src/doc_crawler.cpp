#ifndef DOCUMENT_CRAWLER_CPP
#define DOCUMENT_CRAWLER_CPP

#include "doc_crawler.hpp"

void DocumentCrawler::start() {
    std::vector<std::string> foundDocuments = getDocumentPaths();
    for (auto& document_path: foundDocuments) {
        registerDocument(document_path);
    }

    store.processDocuments();
}

std::vector<std::string> DocumentCrawler::getDocumentPaths() {
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

void DocumentCrawler::registerDocument(std::string path) {
    int new_id = documents->size() + 1;
    std::string text = helpers::loadFile(path);
    docmeta::DocumentMeta document = docmeta::DocumentMeta(new_id, text, path);

    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(documents->begin(), documents->end(), [&document](const docmeta::DocumentMeta doc) { 
        return doc.path == document.path;
    });

    if (it != documents->end()) {
        if ((it->content) != (document.content)) {
            document.id = it->id;
            documents->erase(it);
        }
    }

    documents->insert(document);
}

#endif