#include <catch2/catch_test_macros.hpp>
#include "doc_crawler.hpp"

TEST_CASE("Document crawler end-to-end test.", "[crawler][e2e]") {
    std::set<docmeta::DocumentMeta> expected_store = {
        docmeta::DocumentMeta(1, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis interdum malesuada nisl, in posuere ligula aliquam et. Vivamus dapibus sapien a maximus suscipit. Maecenas id viverra mauris. Phasellus et lorem quis massa pretium elementum. Donec feugiat diam ut lorem pellentesque, eget eleifend elit iaculis. Mauris vitae libero pulvinar, egestas erat.", SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc1.txt"),
        docmeta::DocumentMeta(2, "Curabitur eleifend vehicula ultrices. Praesent consectetur aliquam in sit cursus. Nullam sed tellus hendrerit, porttitor massa at, congue lorem. Phasellus euismod et.", SEARCHENGINE_TESTING_DIR "/resources/test-documents/doc2.txt"),
        docmeta::DocumentMeta(3, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse vitae ligula imperdiet, auctor lacus non, tristique justo. Quisque imperdiet lacus elit, amet euismod ipsum venenatis a. Suspendisse finibus lectus lacus, id semper turpis efficitur eget. Nam mattis lorem ipsum, varius pulvinar neque aliquam in. Sed ut ultricies sem. Curabitur.", SEARCHENGINE_TESTING_DIR "/resources/test-documents/folder1/doc3.txt")
    };

    std::vector<docmeta::DocumentMeta> expected_repository(expected_store.begin(), expected_store.end());

    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);                                              // <---- Injecting Doc_Store
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, SEARCHENGINE_TESTING_DIR "/resources/test-documents");
    crawler.start();

    REQUIRE( documents_in_store == expected_store );
    REQUIRE( repository == expected_repository );
}
