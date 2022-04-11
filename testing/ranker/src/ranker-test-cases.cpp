#include <catch2/catch_test_macros.hpp>
#include "ranker.hpp"

std::vector<docmeta::DocumentMeta> store = { 
    docmeta::DocumentMeta(1, "Hello, World! How are you world?", SEARCHENGINE_TESTING_DIR "/test-documents/doc1.txt"),
    docmeta::DocumentMeta(2, "Around the World in Eighty Days. Eighty miles", SEARCHENGINE_TESTING_DIR "/test-documents/doc2.txt")
};

TEST_CASE("Ranker can transfrom search query (remove whitespace and to lowercase).", "[ranker]") {
    std::string searchQuery = "  HELLO  ";
    std::string expectedQuery = "hello";

    Ranker ranker = Ranker();
    std::string returnedQuery = ranker.transformQuery(searchQuery);

    REQUIRE( returnedQuery == expectedQuery );
}

TEST_CASE("Ranker can retrieve token meta information.", "[ranker]") {
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    index["hello"] = { tokenmeta::TokenMeta(1, 1, { 1 }, &store[0]) };
    index["world"] =  { tokenmeta::TokenMeta(1, 2, { 2, 4 }, &store[0]), tokenmeta::TokenMeta(2, 1, { 2 }, &store[1]) };
    index["you"] = { tokenmeta::TokenMeta(1, 1, { 3 }, &store[0]) };
    index["around"] =  { tokenmeta::TokenMeta(2, 1, { 1 }, &store[1]) };
    index["eighty"] = { tokenmeta::TokenMeta(2, 2, { 3, 5 }, &store[1]) };
    index["days"] = { tokenmeta::TokenMeta(2, 1, { 4 }, &store[1]) };
    index["miles"] = { tokenmeta::TokenMeta(2, 1, { 6 }, &store[1]) };

    std::vector<tokenmeta::TokenMeta> expectedInfo = {
        tokenmeta::TokenMeta(1, 2, { 2, 4 }, &store[0]), 
        tokenmeta::TokenMeta(2, 1, { 2 }, &store[1])
    };
    
    std::string query = "world";

    Ranker ranker = Ranker();
    std::vector<tokenmeta::TokenMeta> returnedInfo = ranker.retrieveMetaInformations(&index, query);

    REQUIRE( returnedInfo == expectedInfo );
}

TEST_CASE("Ranker can collect document ids.", "[ranker]") {
    std::vector<tokenmeta::TokenMeta> tokenMeta = { 
        tokenmeta::TokenMeta(1, 1, { 1 }), 
        tokenmeta::TokenMeta(2, 1, { 1 }) 
    }; 
    std::vector<const docmeta::DocumentMeta*> expectedResult = { tokenMeta[0].doc_ptr, tokenMeta[1].doc_ptr };

    Ranker ranker = Ranker();
    std::vector<const docmeta::DocumentMeta*> doc_ptrs = ranker.filterDocPtrs(tokenMeta);

    REQUIRE( doc_ptrs == expectedResult );
}

TEST_CASE("Ranker can return documents.", "[ranker]") {
    std::vector<docmeta::DocumentMeta> expectedDocuments = {
        docmeta::DocumentMeta(2, "Around the World in Eighty Days. Eighty miles", SEARCHENGINE_TESTING_DIR "/test-documents/doc2.txt"),
    };

    std::vector<const docmeta::DocumentMeta*> ptrs = { &store[1] };

    Ranker ranker = Ranker();
    std::vector<docmeta::DocumentMeta> returnedDocuments = ranker.collectDocuments(ptrs);

    REQUIRE( returnedDocuments == expectedDocuments );
}

TEST_CASE("Ranker can return the specified amount of documents. (test if one document got returned)", "[ranker]") {
    std::vector<docmeta::DocumentMeta> expectedDocuments = {
        docmeta::DocumentMeta(1, "Hello, World! How are you world?", SEARCHENGINE_TESTING_DIR "/test-documents/doc1.txt"),
    };

    std::vector<const docmeta::DocumentMeta*> ptrs = { &store[0], &store[1] };

    Ranker ranker = Ranker(1);
    std::vector<docmeta::DocumentMeta> returnedDocuments = ranker.collectDocuments(ptrs);

    REQUIRE( returnedDocuments == expectedDocuments );
}
