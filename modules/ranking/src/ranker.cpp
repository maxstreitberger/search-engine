#ifndef RANKING_CPP
#define RANKING_CPP

#include "ranker.hpp"

std::vector<docmeta::DocumentMeta> Ranker::searchFor(std::string query) {
    std::string searchQuery = transformQuery(query);
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index = loadIndex(index_path);
    std::set<docmeta::DocumentMeta> store = loadStore(store_path);
    std::vector<tokenmeta::TokenMeta> metaInfo = retrieveMetaInformations(&index, searchQuery);
    std::unordered_set<int> documents = filterDocIds(metaInfo);
    return collectDocuments(&store, documents);
}

std::string Ranker::transformQuery(std::string query) {
    stringhelper::trim(query);
    stringhelper::toLower(query);
    return query;
}

std::map<std::string, std::set<tokenmeta::TokenMeta>> Ranker::loadIndex(std::string path) {
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json index_json;

    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    try {
        index_json = nlohmann::json::parse(content);
        index = index_json.get<std::map<std::string, std::set<tokenmeta::TokenMeta>>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    return index;
}

std::set<docmeta::DocumentMeta> Ranker::loadStore(std::string storePath) {
    std::ifstream ifs(storePath);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json store_json;

    std::set<docmeta::DocumentMeta> store;

    try {
        store_json = nlohmann::json::parse(content);
        store = store_json.get<std::set<docmeta::DocumentMeta>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    return store;
}

std::vector<tokenmeta::TokenMeta> Ranker::retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, std::string query) {
    auto it = index->find(query);
    std::vector<tokenmeta::TokenMeta> metaInformation;

    if (it != index->end()) {
        std::set<tokenmeta::TokenMeta> meta = (*it).second;
        metaInformation.insert(metaInformation.end(), meta.begin(), meta.end());
    }

    std::sort(metaInformation.begin(), metaInformation.end(), [](tokenmeta::TokenMeta ld, tokenmeta::TokenMeta rd) { 
        return ld.num_appearances > rd.num_appearances; 
    });

    return metaInformation;
}

std::unordered_set<int> Ranker::filterDocIds(std::vector<tokenmeta::TokenMeta> tokensMetaInfo) {
    std::unordered_set<int> doc_ids;

    for (auto& tokenMeta: tokensMetaInfo) {
        doc_ids.insert(tokenMeta.document_id);
    }

    return doc_ids;
}

std::vector<docmeta::DocumentMeta> Ranker::collectDocuments(std::set<docmeta::DocumentMeta>* docs, std::unordered_set<int> doc_ids) {
    std::vector<docmeta::DocumentMeta> documents;
    
    for (const int& doc_id: doc_ids) {
        auto it = docs->find(doc_id);
        if (it != docs->end()) {
            docmeta::DocumentMeta doc = *it;
            documents.push_back(doc);
        }
    }

    return documents;
}

#endif