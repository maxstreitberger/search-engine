#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "ranker.hpp"

TEST_CASE("Ranker can", "[ranker]") {
    std::vector<docmeta::DocumentMeta> store = { 
        docmeta::DocumentMeta(1, "Hello, World! How are you world?", RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
        docmeta::DocumentMeta(2, "Around the World in Eighty Days. Eighty miles", RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt")
    };

    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    index["hello"] = { tokenmeta::TokenMeta(1, 1, { 1 }, &store[0]) };
    index["world"] =  { tokenmeta::TokenMeta(1, 2, { 2, 4 }, &store[0]), tokenmeta::TokenMeta(2, 1, { 2 }, &store[1]) };
    index["you"] = { tokenmeta::TokenMeta(1, 1, { 3 }, &store[0]) };
    index["around"] =  { tokenmeta::TokenMeta(2, 1, { 1 }, &store[1]) };
    index["eighty"] = { tokenmeta::TokenMeta(2, 2, { 3, 5 }, &store[1]) };
    index["days"] = { tokenmeta::TokenMeta(2, 1, { 4 }, &store[1]) };
    index["miles"] = { tokenmeta::TokenMeta(2, 1, { 6 }, &store[1]) };

    SECTION("transfrom search query (remove whitespace and to lowercase).") {
        std::string searchQuery = "  HELLO  ";
        std::string expectedQuery = "hello";

        Ranker ranker = Ranker();
        std::string returnedQuery = ranker.transformQuery(searchQuery);

        REQUIRE( returnedQuery == expectedQuery );
    }

    SECTION("can retrieve token meta information.") {
        std::vector<tokenmeta::TokenMeta> expectedInfo = {
            tokenmeta::TokenMeta(1, 2, { 2, 4 }, &store[0]), 
            tokenmeta::TokenMeta(2, 1, { 2 }, &store[1])
        };
        
        std::string query = "world";

        Ranker ranker = Ranker();
        std::vector<tokenmeta::TokenMeta> returnedInfo = ranker.retrieveMetaInformations(&index, query);

        REQUIRE( returnedInfo == expectedInfo );
    }

    SECTION("can collect document ids.") {
        std::vector<tokenmeta::TokenMeta> tokenMeta = { 
            tokenmeta::TokenMeta(1, 1, { 1 }), 
            tokenmeta::TokenMeta(2, 1, { 1 }) 
        }; 
        std::unordered_set<const docmeta::DocumentMeta*> expectedResult = { tokenMeta[0].doc_ptr, tokenMeta[1].doc_ptr };

        Ranker ranker = Ranker();
        std::unordered_set<const docmeta::DocumentMeta*> doc_ptrs = ranker.filterDocPtrs(tokenMeta);

        REQUIRE( doc_ptrs == expectedResult );
    }

    SECTION("can return documents.") {
        std::vector<docmeta::DocumentMeta> expectedDocuments = {
            docmeta::DocumentMeta(2, "Around the World in Eighty Days. Eighty miles", RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt"),
        };

        std::unordered_set<const docmeta::DocumentMeta*> ptrs = { &store[1] };

        Ranker ranker = Ranker();
        std::vector<docmeta::DocumentMeta> returnedDocuments = ranker.collectDocuments(ptrs);

        REQUIRE( returnedDocuments == expectedDocuments );
    }
}

