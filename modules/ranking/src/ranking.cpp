#ifndef RANKING_CPP
#define RANKING_CPP

#include "ranking.hpp"

namespace docmeta {
    void to_json(nlohmann::json& j, const RankingDocumentMeta& doc) {
        j = nlohmann::json{ {"id", doc.id}, {"content", doc.content}, {"path", doc.path} };
    }

    void from_json(const nlohmann::json& j, RankingDocumentMeta& doc) {
        j.at("id").get_to(doc.id);
        j.at("content").get_to(doc.content);
        j.at("path").get_to(doc.path);
    }
}

namespace tokenmeta {
    void to_json(nlohmann::json& j, const RankingTokenMeta& token) {
        j = nlohmann::json{ {"doc_id", token.document_id}, {"num_appearances", token.num_appearances}, {"positions", token.positions} };
    }

    void from_json(const nlohmann::json& j, RankingTokenMeta& token) {
        j.at("doc_id").get_to(token.document_id);
        j.at("num_appearances").get_to(token.num_appearances);
        j.at("positions").get_to(token.positions);
    }
}

std::vector<docmeta::RankingDocumentMeta> Ranking::searchFor(std::string query) {
    stringhelper::trim(query);
    std::transform(query.begin(), query.end(), query.begin(), [](unsigned char c){ return std::tolower(c); });
    std::set<tokenmeta::RankingTokenMeta> tokens = getTokens(query);
    return collectDocuments(tokens);
}

std::set<tokenmeta::RankingTokenMeta> Ranking::getTokens(std::string query) {
    std::ifstream ifs("index.json");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json index;

    try {
        index = nlohmann::json::parse(content);
        try {
            std::set<tokenmeta::RankingTokenMeta> tokens = index.at(query);
            return tokens;
        }
        catch (nlohmann::json::out_of_range& e) {
            std::cout << e.what() << '\n';
            return {};
        }
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }
    return {};
}

std::vector<docmeta::RankingDocumentMeta> Ranking::collectDocuments(std::set<tokenmeta::RankingTokenMeta> tokens) {
    std::ifstream ifs("store.json");
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    nlohmann::json store;
    std::vector<docmeta::RankingDocumentMeta> documents;

    try {
        store = nlohmann::json::parse(content);
        // std::cout << std::setw(4) << store << std::endl;

        for (auto& doc: tokens) {
            try {
                documents.push_back(store.at(doc.document_id-1));
            }
            catch (nlohmann::json::out_of_range& e) {
                std::cout << e.what() << '\n';
            }
        }
    }
    catch (nlohmann::json::parse_error& ex) {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }
    return documents;
}

#endif