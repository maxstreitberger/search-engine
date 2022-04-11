#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "doc_store.hpp"

std::set<docmeta::DocumentMeta> crawler_docs = {
    docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1"),
    docmeta::DocumentMeta(2, "Test!", "Some Example Path #2"),
};

std::set<docmeta::DocumentMeta>* document_store;
std::vector<docmeta::DocumentMeta>* repository;

TEST_CASE("Document Store can check for new documents.", "[store]") {
    std::set<docmeta::DocumentMeta> resultingStore;
    DocStore store = DocStore(&crawler_docs, document_store, repository);

    std::vector expectedResult(crawler_docs.begin(), crawler_docs.end());
    std::vector<docmeta::DocumentMeta> foundNewDocuments = store.checkForChanges(&resultingStore, &crawler_docs).first;

    REQUIRE( expectedResult == foundNewDocuments );

    BENCHMARK("Document Store can check for new documents.") {
        return store.checkForChanges(&resultingStore, &crawler_docs).first;
    };
}

TEST_CASE("Document Store can check for updated documents.", "[store]") {
    std::set<docmeta::DocumentMeta> toUpdateDocument = { docmeta::DocumentMeta(1, "Hello, World! How are you?", "Some Example Path #1") };
    std::vector<docmeta::DocumentMeta> expectedResult = {
        docmeta::DocumentMeta(1, "Hello, World! How are you?", "Some Example Path #1")
    };

    DocStore store = DocStore(&crawler_docs, document_store, repository);
    std::vector<docmeta::DocumentMeta> resultingUpdates = store.checkForChanges(&crawler_docs, &toUpdateDocument).second;

    REQUIRE( expectedResult == resultingUpdates );

    BENCHMARK("Document Store can check for updated documents.") {
        return store.checkForChanges(&crawler_docs, &toUpdateDocument).second;
    };
}

TEST_CASE("Document Store can add new documments to the store.", "[store]") {
    std::set<docmeta::DocumentMeta> expectedStore = { docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1") };

    std::set<docmeta::DocumentMeta> resultingStore;
    DocStore store = DocStore(&crawler_docs, document_store, repository);

    std::vector newDocumment = { docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1") };
    store.addNewDocumentsToStore(&resultingStore, newDocumment);

    REQUIRE( expectedStore == resultingStore );

    resultingStore = {};
    BENCHMARK("Document Store can check for updated documents.") {
        return store.addNewDocumentsToStore(&resultingStore, newDocumment);
    };
}

TEST_CASE("Document Store can update documments in the store.", "[store]") {
    std::set<docmeta::DocumentMeta> expectedStore = { docmeta::DocumentMeta(1, "Hello, World! How are you?", "Some Example Path #1") };

    std::set<docmeta::DocumentMeta> resultingStore = { docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1") };
    DocStore store = DocStore(&crawler_docs, document_store, repository);

    std::vector updatedDocument = { docmeta::DocumentMeta(1, "Hello, World! How are you?", "Some Example Path #1") };
    store.updateDocumentsInStore(&resultingStore, updatedDocument);

    REQUIRE( expectedStore == resultingStore );

    resultingStore = { docmeta::DocumentMeta(1, "Hello, World!", "Some Example Path #1") };
    BENCHMARK("Document Store can update documments in the store.") {
        return store.updateDocumentsInStore(&resultingStore, updatedDocument);
    };
}
