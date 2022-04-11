#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "indexer.hpp"

TEST_CASE("Indexer can load stopwords.", "[indexer]") {
    std::vector<std::string> someExpectedStopwords = {"about", "was", "com"};
    
    Indexer indexer = Indexer("", SEARCHENGINE_TESTING_DIR "/resources/stopwords.txt");
    std::set<std::string> resultingList = indexer.loadList(indexer.stopword_path);

    CHECK( resultingList.find(someExpectedStopwords[0]) != resultingList.end() );
    CHECK( resultingList.find(someExpectedStopwords[1]) != resultingList.end() );
    CHECK( resultingList.find(someExpectedStopwords[2]) != resultingList.end() );

    BENCHMARK("Indexer can load stopwords.") {
        return indexer.loadList(indexer.stopword_path);
    };
}

TEST_CASE("Indexer can load special characters.", "[indexer]") {
    std::vector<std::string> someExpectedChars = {"!", ",", "."};
    
    Indexer indexer = Indexer(SEARCHENGINE_TESTING_DIR "/resources/special.txt", "");
    std::set<std::string> resultingList = indexer.loadList(indexer.special_chars_path);

    CHECK( resultingList.find(someExpectedChars[0]) != resultingList.end() );
    CHECK( resultingList.find(someExpectedChars[1]) != resultingList.end() );
    CHECK( resultingList.find(someExpectedChars[2]) != resultingList.end() );

    BENCHMARK("Indexer can load special characters.") {
        return indexer.loadList(indexer.special_chars_path);
    };
}

TEST_CASE("Indexer can split text into list by whitespace.", "[indexer]") {
    std::vector<std::string> expectedList = { "hello,", "world!" };
    std::string text = "Hello, World!";

    Indexer indexer = Indexer();
    std::vector<std::string> returnedList = indexer.splitTextIntoList(text);

    REQUIRE( expectedList == returnedList );

    BENCHMARK("Indexer can split text into list by whitespace.") {
        return indexer.splitTextIntoList(text);
    };
}

TEST_CASE("Indexer can remove stopwords.", "[indexer]") {
    std::set<std::string> stopwords = { "i", "be", "to", "from", "will" };
    std::vector<std::string> tokens = { "from", "be", "earth", "to", "jupiter", "i", "will" };
    std::vector<std::string> expectedTokens = { "earth", "jupiter" };

    Indexer indexer = Indexer();
    std::vector<std::string> resultTokens = indexer.removeStopwords(tokens, stopwords);

    REQUIRE( expectedTokens == resultTokens );

    BENCHMARK("Indexer can remove stopwords.") {
        return indexer.removeStopwords(tokens, stopwords);
    };
}

TEST_CASE("Indexer can remove special characters.", "[indexer]") {
    std::set<std::string> specialChars = { "+", "?", "!", "\"", "^", "<", ">", ";", ",", ":", ".", "_" };
    std::vector<std::string> tokens = { "mercury,", "venus!", "<earth>", "_mars", "jupiter:" };
    std::vector<std::string> expectedTokens = { "mercury", "venus", "earth", "mars", "jupiter" };

    Indexer indexer = Indexer();
    std::vector<std::string> resultTokens = indexer.removeSpecialChars(tokens, specialChars);

    REQUIRE( expectedTokens == resultTokens );

    BENCHMARK("Indexer can remove special characters.") {
        return indexer.removeSpecialChars(tokens, specialChars);
    };
}

TEST_CASE("Indexer can create index for one document.", "[indexer]") {
    docmeta::DocumentMeta document = docmeta::DocumentMeta(1, "Hello, World! How are you world?", "Some Example Path #1");
    std::vector<std::string> tokens = { "hello", "world", "you", "world" };

    std::set<tokenmeta::TokenMeta> hello = { tokenmeta::TokenMeta(1, 1, { 1 }, &document) };
    std::set<tokenmeta::TokenMeta> world = { tokenmeta::TokenMeta(1, 2, { 2, 4 }, &document) };
    std::set<tokenmeta::TokenMeta> you = { tokenmeta::TokenMeta(1, 1, { 3 }, &document) };        

    Indexer indexer = Indexer();
    std::map<std::string, std::set<tokenmeta::TokenMeta>> returnedIndex = indexer.createIndexForDocument(&document, tokens);

    CHECK( returnedIndex.size() == 3 );
    CHECK( returnedIndex["hello"] ==  hello);
    CHECK( returnedIndex["world"] == world );
    CHECK( returnedIndex["you"] == you );

    BENCHMARK("Indexer can create index for one document.") {
        return indexer.createIndexForDocument(&document, tokens);
    };
}
