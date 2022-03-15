#ifndef CRAWLER_CPP
#define CRAWLER_CPP

#include "crawler.hpp"
#include "doc_store.hpp"

void Crawler::start() {
    std::cout << "Crawling through " << origin_path << std::endl;
    std::vector<std::string> foundDocuments = getDocumentPaths();
    for (auto& document_path: foundDocuments) {
        registerDocument(document_path);
    }

    pushDocumentsToStore(documents);
    
    DocStore store = DocStore("store.json", "crawler-found-docs.json", "repository.json");
    store.processDocuments();
}

std::vector<std::string> Crawler::getDocumentPaths() {
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

std::string Crawler::getDocumentContents(const std::string path) {
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}

void Crawler::registerDocument(const std::filesystem::path path) {
    int new_id = documents.size() + 1;
    std::string text = getDocumentContents(path);
    docmeta::DocumentMeta document = docmeta::DocumentMeta(new_id, text, path.string());

    std::set<docmeta::DocumentMeta>::iterator it = std::find_if(documents.begin(), documents.end(), [&document](const docmeta::DocumentMeta doc) { 
        return doc.path == document.path;
    });

    if (it != documents.end()) {
        if ((it->content) != (document.content)) {
            document.id = it->id;
            documents.erase(it);
        }
    }

    documents.insert(document);
}

void Crawler::pushDocumentsToStore(const std::set<docmeta::DocumentMeta> documents) {
    std::ofstream file("crawler-found-docs.json");

    nlohmann::json doc_json = documents;

    file << std::setw(4) << doc_json;

    file.close();
}

#endif