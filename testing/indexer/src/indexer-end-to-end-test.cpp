#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "indexer.hpp"

TEST_CASE("Indexer end-to-end test.", "[indexer, e2e]") {
    std::string stopwordsPath = SEARCHENGINE_TESTING_DIR "/resources/stopwords.txt";
    std::string specialCharsPath = SEARCHENGINE_TESTING_DIR "/resources/special.txt";
    
    std::vector<docmeta::DocumentMeta> repository = {
        docmeta::DocumentMeta(1, "Hello, World! How are you world?", "Some Example Path #1"),
        docmeta::DocumentMeta(2, "Quisque gravida erat at turpis sodales euismod ut vitae justo.", "Some Example Path #2"),
    };

    std::map<std::string, std::set<tokenmeta::TokenMeta>> expectedIndex;
    expectedIndex["erat"].insert(tokenmeta::TokenMeta(2, 1, { 3 }, &repository[1]));
    expectedIndex["euismod"].insert(tokenmeta::TokenMeta(2, 1, { 6 }, &repository[1]));
    expectedIndex["gravida"].insert(tokenmeta::TokenMeta(2, 1, { 2 }, &repository[1]));
    expectedIndex["hello"].insert(tokenmeta::TokenMeta(1, 1, { 1 }, &repository[0]));
    expectedIndex["justo"].insert(tokenmeta::TokenMeta(2, 1, { 9 }, &repository[1]));
    expectedIndex["quisque"].insert(tokenmeta::TokenMeta(2, 1, { 1 }, &repository[1]));
    expectedIndex["sodales"].insert(tokenmeta::TokenMeta(2, 1, { 5 }, &repository[1]));
    expectedIndex["turpis"].insert(tokenmeta::TokenMeta(2, 1, { 4 }, &repository[1]));
    expectedIndex["ut"].insert(tokenmeta::TokenMeta(2, 1, { 7 }, &repository[1]));
    expectedIndex["vitae"].insert(tokenmeta::TokenMeta(2, 1, { 8 }, &repository[1]));
    expectedIndex["world"].insert(tokenmeta::TokenMeta(1, 2, { 2, 4 }, &repository[0]));
    expectedIndex["you"].insert(tokenmeta::TokenMeta(1, 1, { 3 }, &repository[0]));

    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    indexer.generateIndex();

    REQUIRE( index.size() == expectedIndex.size() );
    CHECK( index["erat"] == expectedIndex["erat"] );
    CHECK( index["euismod"] == expectedIndex["euismod"] );
    CHECK( index["gravida"] == expectedIndex["gravida"] );
    CHECK( index["hello"] == expectedIndex["hello"] );
    CHECK( index["justo"] == expectedIndex["justo"] );
    CHECK( index["quisque"] == expectedIndex["quisque"] );
    CHECK( index["sodales"] == expectedIndex["sodales"] );
    CHECK( index["turpis"] == expectedIndex["turpis"] );
    CHECK( index["ut"] == expectedIndex["ut"] );
    CHECK( index["vitae"] == expectedIndex["vitae"] );
    CHECK( index["world"] == expectedIndex["world"] );
    CHECK( index["you"] == expectedIndex["you"] );

    index = {};
    BENCHMARK("Indexer end-to-end test.") {
        return indexer.generateIndex();
    };
}