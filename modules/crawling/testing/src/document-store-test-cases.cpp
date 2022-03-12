#include <catch2/catch_test_macros.hpp>
#include "doc_store.hpp"

TEST_CASE("Document Store can", "[store]") {
    std::string storePath = CRAWLER_TESTING_ROOT_DIR "/testing-store.json";
    std::string crawledDocsPath = CRAWLER_TESTING_ROOT_DIR "/testing-crawler-found-docs.json";
    std::string repoPath = CRAWLER_TESTING_ROOT_DIR "/repository.json";

    std::set<docmeta::CrawlerDocMeta> documents = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
    nlohmann::json j = documents;

    std::ofstream docStore(storePath);
    docStore << j;
    docStore.close();

    std::ofstream crawledDocs(crawledDocsPath);
    crawledDocs << j;
    crawledDocs.close();

    SECTION("retrieve documents in the store.") {
        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);
        std::set<docmeta::CrawlerDocMeta> currentStore = store.loadDocuments(store.store_path);

        REQUIRE( documents == currentStore );
    }

    SECTION("retrieve documents found by crawler.") {
        DocStore store = DocStore(crawledDocsPath, crawledDocsPath, repoPath);
        std::set<docmeta::CrawlerDocMeta> doc_found_by_crawler = store.loadDocuments(store.crawled_docs_path);

        REQUIRE( documents == doc_found_by_crawler );
    }

    SECTION("write the store to file.") {
        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);
        store.writeStoreToFile(documents);

        nlohmann::json expectedOutput = documents;

        std::ifstream file(storePath);
        std::string resultOutput((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();

        REQUIRE( expectedOutput.dump(4) == resultOutput );
    }

    SECTION("check for new documents.") {
        std::set<docmeta::CrawlerDocMeta> resultingStore;
        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);

        std::vector expectedResult(documents.begin(), documents.end());
        std::vector<docmeta::CrawlerDocMeta> foundNewDocuments = store.checkForChanges(&resultingStore, documents).first;

        REQUIRE( expectedResult == foundNewDocuments );
    }

    SECTION("check for updated documents.") {
        std::set<docmeta::CrawlerDocMeta> resultingStore = documents;
        std::set<docmeta::CrawlerDocMeta> toUpdateDocument = { docmeta::CrawlerDocMeta(1, "Hello, World! How are you?", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);

        std::vector expectedResult(toUpdateDocument.begin(), toUpdateDocument.end());
        std::vector<docmeta::CrawlerDocMeta> foundNewDocuments = store.checkForChanges(&resultingStore, toUpdateDocument).second;

        REQUIRE( expectedResult == foundNewDocuments );
    }

    SECTION("add new documments to the store.") {
        std::set<docmeta::CrawlerDocMeta> expectedStore = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        std::set<docmeta::CrawlerDocMeta> resultingStore;
        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);

        std::vector newDocumment = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        store.addNewDocumentsToStore(&resultingStore, newDocumment);

        REQUIRE( expectedStore == resultingStore );
    }

    SECTION("update documments in the store.") {
        std::set<docmeta::CrawlerDocMeta> expectedStore = { docmeta::CrawlerDocMeta(1, "Hello, World! How are you?", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };

        std::set<docmeta::CrawlerDocMeta> resultingStore = { docmeta::CrawlerDocMeta(1, "Hello, World!", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);

        std::vector updatedDocument = { docmeta::CrawlerDocMeta(1, "Hello, World! How are you?", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        store.updateDocumentsInStore(&resultingStore, updatedDocument);

        REQUIRE( expectedStore == resultingStore );
    }

    SECTION("write changes to repository.") {
        std::vector newAndChangedDocuments = { docmeta::CrawlerDocMeta(1, "Hello, World! How are you?", CRAWLER_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        DocStore store = DocStore(storePath, crawledDocsPath, repoPath);
        store.writeChangesToRepository(newAndChangedDocuments, store.repo_path);

        nlohmann::json expectedOutput = newAndChangedDocuments;

        std::ifstream file(repoPath);
        std::string resultOutput((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();

        REQUIRE( expectedOutput.dump(4) == resultOutput );
    }
}
