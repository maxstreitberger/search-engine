#ifndef RANKING_CPP
#define RANKING_CPP

#include "ranker.hpp"

namespace docmeta {
    void to_json(nlohmann::json& j, const RankerDocumentMeta& doc) {
        j = nlohmann::json{ {"id", doc.id}, {"content", doc.content}, {"path", doc.path} };
    }

    void from_json(const nlohmann::json& j, RankerDocumentMeta& doc) {
        j.at("id").get_to(doc.id);
        j.at("content").get_to(doc.content);
        j.at("path").get_to(doc.path);
    }
}

namespace tokenmeta {
    void to_json(nlohmann::json& j, const RankerTokenMeta& token) {
        j = nlohmann::json{ {"doc_id", token.document_id}, {"num_appearances", token.num_appearances}, {"positions", token.positions} };
    }

    void from_json(const nlohmann::json& j, RankerTokenMeta& token) {
        j.at("doc_id").get_to(token.document_id);
        j.at("num_appearances").get_to(token.num_appearances);
        j.at("positions").get_to(token.positions);
    }
}

std::vector<docmeta::RankerDocumentMeta> Ranker::searchFor(std::string query) {
    std::string searchQuery = transformQuery(query);
    std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> index = loadIndex(index_path);
    std::set<docmeta::RankerDocumentMeta> store = loadStore(store_path);
    std::vector<tokenmeta::RankerTokenMeta> metaInfo = retrieveMetaInformations(&index, searchQuery);
    std::unordered_set<int> documents = filterDocIds(metaInfo);
    return collectDocuments(&store, documents);
}

std::string Ranker::transformQuery(std::string query) {
    stringhelper::trim(query);
    std::transform(query.begin(), query.end(), query.begin(), [](unsigned char c){ return std::tolower(c); });
    return query;
}

std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> Ranker::loadIndex(std::string path) {
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json index_json;

    std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> index;

    try {
        index_json = nlohmann::json::parse(content);
        index = index_json.get<std::map<std::string, std::set<tokenmeta::RankerTokenMeta>>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    return index;
}

std::set<docmeta::RankerDocumentMeta> Ranker::loadStore(std::string storePath) {
    std::ifstream ifs(storePath);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json store_json;

    std::set<docmeta::RankerDocumentMeta> store;

    try {
        store_json = nlohmann::json::parse(content);
        store = store_json.get<std::set<docmeta::RankerDocumentMeta>>();
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    return store;
}

std::vector<tokenmeta::RankerTokenMeta> Ranker::retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::RankerTokenMeta>>* index, std::string query) {
    auto it = index->find(query);
    std::vector<tokenmeta::RankerTokenMeta> metaInformation;

    if (it != index->end()) {
        std::set<tokenmeta::RankerTokenMeta> meta = (*it).second;
        metaInformation.insert(metaInformation.end(), meta.begin(), meta.end());
    }

    std::sort(metaInformation.begin(), metaInformation.end(), [](tokenmeta::RankerTokenMeta ld, tokenmeta::RankerTokenMeta rd) { 
        return ld.num_appearances > rd.num_appearances; 
    });

    return metaInformation;
}

std::unordered_set<int> Ranker::filterDocIds(std::vector<tokenmeta::RankerTokenMeta> tokensMetaInfo) {
    std::unordered_set<int> doc_ids;

    for (auto& tokenMeta: tokensMetaInfo) {
        doc_ids.insert(tokenMeta.document_id);
    }

    return doc_ids;
}

std::vector<docmeta::RankerDocumentMeta> Ranker::collectDocuments(std::set<docmeta::RankerDocumentMeta>* docs, std::unordered_set<int> doc_ids) {
    std::vector<docmeta::RankerDocumentMeta> documents;
    
    for (const int& doc_id: doc_ids) {
        auto it = docs->find(doc_id);
        if (it != docs->end()) {
            docmeta::RankerDocumentMeta doc = *it;
            documents.push_back(doc);
        }
    }

    return documents;
}

#endif