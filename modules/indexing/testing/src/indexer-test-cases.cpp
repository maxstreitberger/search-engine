#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "indexer.hpp"

TEST_CASE("Indexer can", "[indexer]") {

    SECTION("load stopwords.") {
        std::vector<std::string> someExpectedStopwords = {"about", "was", "com"};
        
        Indexer indexer = Indexer("", INDEXING_ROOT_DIR "/documents/stopwords.txt");
        std::set<std::string> resultingList = indexer.loadList(indexer.stopword_path);

        CHECK( resultingList.find(someExpectedStopwords[0]) != resultingList.end() );
        CHECK( resultingList.find(someExpectedStopwords[1]) != resultingList.end() );
        CHECK( resultingList.find(someExpectedStopwords[2]) != resultingList.end() );
    }

    SECTION("load special characters.") {
        std::vector<std::string> someExpectedChars = {"!", ",", "."};
        
        Indexer indexer = Indexer(INDEXING_ROOT_DIR "/documents/special.txt", "");
        std::set<std::string> resultingList = indexer.loadList(indexer.special_chars_path);

        CHECK( resultingList.find(someExpectedChars[0]) != resultingList.end() );
        CHECK( resultingList.find(someExpectedChars[1]) != resultingList.end() );
        CHECK( resultingList.find(someExpectedChars[2]) != resultingList.end() );
    }

    SECTION("split text into list by whitespace.") {
        std::vector<std::string> expectedList = { "hello,", "world!" };
        std::string text = "Hello, World!";

        Indexer indexer = Indexer();
        std::vector<std::string> returnedList = indexer.splitTextIntoList(text);

        REQUIRE( expectedList == returnedList );
    }

    SECTION("remove stopwords.") {
        std::set<std::string> stopwords = { "i", "be", "to", "from", "will" };
        std::vector<std::string> tokens = { "from", "be", "earth", "to", "jupiter", "i", "will" };
        std::vector<std::string> expectedTokens = { "earth", "jupiter" };

        Indexer indexer = Indexer();
        std::vector<std::string> resultTokens = indexer.removeStopwords(tokens, stopwords);

        REQUIRE( expectedTokens == resultTokens );
    }

    SECTION("remove special characters.") {
        std::set<std::string> specialChars = { "+", "?", "!", "\"", "^", "<", ">", ";", ",", ":", ".", "_" };
        std::vector<std::string> tokens = { "mercury,", "venus!", "<earth>", "_mars", "jupiter:" };
        std::vector<std::string> expectedTokens = { "mercury", "venus", "earth", "mars", "jupiter" };

        Indexer indexer = Indexer();
        std::vector<std::string> resultTokens = indexer.removeSpecialChars(tokens, specialChars);

        REQUIRE( expectedTokens == resultTokens );
    }

    SECTION("create index for one document.") {
        docmeta::DocumentMeta document = docmeta::DocumentMeta(1, "Hello, World! How are you world?", "DOCUMENT PATH");
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
    }

    // SECTION("update index.") {
    //     REQUIRE( 0 );
    // } 
}