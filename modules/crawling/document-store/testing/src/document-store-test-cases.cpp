#include <catch2/catch_test_macros.hpp>
#include "doc_store.hpp"

TEST_CASE("Document Store can", "[store]") {
    std::set<docmeta::DocumentMeta> crawler_docs = {
        docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt")
    };

    std::set<docmeta::DocumentMeta>* document_store;
    std::vector<docmeta::DocumentMeta>* repository;

    SECTION("check for new documents.") {
        std::set<docmeta::DocumentMeta> resultingStore;
        DocStore store = DocStore(crawler_docs, document_store, repository);

        std::vector expectedResult(crawler_docs.begin(), crawler_docs.end());
        std::vector<docmeta::DocumentMeta> foundNewDocuments = store.checkForChanges(&resultingStore, crawler_docs).first;

        REQUIRE( expectedResult == foundNewDocuments );
    }

    SECTION("check for updated documents.") {
        std::set<docmeta::DocumentMeta> resultingStore = crawler_docs;
        std::set<docmeta::DocumentMeta> toUpdateDocument = { docmeta::DocumentMeta(1, "Hello, World! How are you?", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        DocStore store = DocStore(crawler_docs, document_store, repository);

        std::vector expectedResult(toUpdateDocument.begin(), toUpdateDocument.end());
        std::vector<docmeta::DocumentMeta> foundNewDocuments = store.checkForChanges(&resultingStore, toUpdateDocument).second;

        REQUIRE( expectedResult == foundNewDocuments );
    }

    SECTION("add new documments to the store.") {
        std::set<docmeta::DocumentMeta> expectedStore = { docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        std::set<docmeta::DocumentMeta> resultingStore;
        DocStore store = DocStore(crawler_docs, document_store, repository);

        std::vector newDocumment = { docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        store.addNewDocumentsToStore(&resultingStore, newDocumment);

        REQUIRE( expectedStore == resultingStore );
    }

    SECTION("update documments in the store.") {
        std::set<docmeta::DocumentMeta> expectedStore = { docmeta::DocumentMeta(1, "Hello, World! How are you?", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        std::set<docmeta::DocumentMeta> resultingStore = { docmeta::DocumentMeta(1, "Hello, World!", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        DocStore store = DocStore(crawler_docs, document_store, repository);

        std::vector updatedDocument = { docmeta::DocumentMeta(1, "Hello, World! How are you?", DOCUMENT_CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        store.updateDocumentsInStore(&resultingStore, updatedDocument);

        REQUIRE( expectedStore == resultingStore );
    }
}
