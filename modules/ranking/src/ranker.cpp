#ifndef RANKING_CPP
#define RANKING_CPP

#include "ranker.hpp"

std::vector<docmeta::DocumentMeta> Ranker::searchFor(std::string query) {
    LOG(INFO) << "Search for: '" << query << "'";
    std::string searchQuery = transformQuery(query);
    std::vector<tokenmeta::TokenMeta> metaInfo = retrieveMetaInformations(index, searchQuery);
    std::unordered_set<const docmeta::DocumentMeta*> doc_ptrs = filterDocPtrs(metaInfo);
    return collectDocuments(doc_ptrs);
}

std::string Ranker::transformQuery(std::string query) {
    helpers::trim(query);
    helpers::toLower(query);
    LOG(INFO) << "Finished query transformation: '" << query << "'";
    return query;
}

std::vector<tokenmeta::TokenMeta> Ranker::retrieveMetaInformations(std::map<std::string, std::set<tokenmeta::TokenMeta>>* index, std::string query) {
    LOG(INFO) << "Retrieve meta information";
    auto it = index->find(query);
    std::vector<tokenmeta::TokenMeta> metaInformation;

    if (it != index->end()) {
        std::set<tokenmeta::TokenMeta> meta = (*it).second;
        metaInformation.insert(metaInformation.end(), meta.begin(), meta.end());
    }

    std::sort(metaInformation.begin(), metaInformation.end(), [](tokenmeta::TokenMeta ld, tokenmeta::TokenMeta rd) { 
        LOG(INFO) << "Sorting after num_appearances";
        return ld.num_appearances > rd.num_appearances; 
    });

    return metaInformation;
}

std::unordered_set<const docmeta::DocumentMeta*> Ranker::filterDocPtrs(std::vector<tokenmeta::TokenMeta> tokensMetaInfo) {
    LOG(INFO) << "Filter document ids";
    std::unordered_set<const docmeta::DocumentMeta*> ptrs;

    for (auto& tokenMeta: tokensMetaInfo) {
        ptrs.insert(tokenMeta.doc_ptr);
    }

    return ptrs;
}

std::vector<docmeta::DocumentMeta> Ranker::collectDocuments(std::unordered_set<const docmeta::DocumentMeta*> doc_ptrs) {
    LOG(INFO) << "Collect documents";
    std::vector<docmeta::DocumentMeta> documents;

    for (const docmeta::DocumentMeta* ptr: doc_ptrs) {
        docmeta::DocumentMeta doc = *(ptr);
        documents.push_back(doc);
    }

    return documents;
}

#endif