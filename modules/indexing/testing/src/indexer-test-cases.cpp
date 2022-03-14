#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "indexer.hpp"

TEST_CASE("Indexer can", "[indexer]") {

    std::string repoPath = INDEXING_TESTING_ROOT_DIR "/repository.json";
    std::string specialCharsPath = INDEXING_TESTING_ROOT_DIR "/special.txt";
    std::string stopwordsPath = INDEXING_TESTING_ROOT_DIR "/stopwords.json";
    std::string indexPath = INDEXING_TESTING_ROOT_DIR "/index.json";

    SECTION("read repository.") {
        std::vector<docmeta::DocumentMeta> documents = { docmeta::DocumentMeta(1, "Hello, World!", INDEXING_TESTING_ROOT_DIR "/testing-documents/demo.txt") };
        nlohmann::json j = documents;

        std::ofstream repo(INDEXING_TESTING_ROOT_DIR "/repository.json");
        repo << j;
        repo.close();

        Indexer indexer = Indexer();
        std::vector<docmeta::DocumentMeta> loadedRepo = indexer.loadRepository(repoPath);

        REQUIRE( documents == loadedRepo );
    }

    SECTION("read load stopwords.") {
        std::vector<std::string> someExpectedStopwords = {"about", "was", "com"};
        
        Indexer indexer = Indexer(specialCharsPath, INDEXING_ROOT_DIR "/documents/stopwords.txt", repoPath, indexPath);
        std::set<std::string> resultingList = indexer.loadList(indexer.stopword_path);

        CHECK( resultingList.find(someExpectedStopwords[0]) != resultingList.end() );
        CHECK( resultingList.find(someExpectedStopwords[1]) != resultingList.end() );
        CHECK( resultingList.find(someExpectedStopwords[2]) != resultingList.end() );
    }

    SECTION("read load special characters.") {
        std::vector<std::string> someExpectedChars = {"!", ",", "."};
        
        Indexer indexer = Indexer(INDEXING_ROOT_DIR "/documents/special.txt", stopwordsPath, repoPath, indexPath);
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
        docmeta::DocumentMeta document = docmeta::DocumentMeta(1, "Hello, World! How are you world?", INDEXING_TESTING_ROOT_DIR "/testing-documents/demo.txt");
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

    SECTION("create join multiple indexes.") {
        docmeta::DocumentMeta document1 = docmeta::DocumentMeta(1, "Hello, World! How are you world?", INDEXING_TESTING_ROOT_DIR "/testing-documents/demo.txt");
        docmeta::DocumentMeta document2 = docmeta::DocumentMeta(2, "Around the World in Eighty Days", INDEXING_TESTING_ROOT_DIR "/testing-documents/demo2.txt");
        
        std::set<tokenmeta::TokenMeta> hello = { tokenmeta::TokenMeta(1, 1, { 1 }, &document1) };
        std::set<tokenmeta::TokenMeta> world = { tokenmeta::TokenMeta(1, 2, { 2, 4 }, &document1), tokenmeta::TokenMeta(2, 1, { 2 }, &document2) };
        std::set<tokenmeta::TokenMeta> you = { tokenmeta::TokenMeta(1, 1, { 3 }, &document1) };
        std::set<tokenmeta::TokenMeta> around = { tokenmeta::TokenMeta(2, 1, { 1 }, &document2) };
        std::set<tokenmeta::TokenMeta> eighty = { tokenmeta::TokenMeta(2, 1, { 3 }, &document2) };
        std::set<tokenmeta::TokenMeta> days = { tokenmeta::TokenMeta(2, 1, { 4 }, &document2) };


        std::map<std::string, std::set<tokenmeta::TokenMeta>> firstIndex;
        firstIndex["hello"] = { tokenmeta::TokenMeta(1, 1, { 1 }, &document1) };;
        firstIndex["world"] =  { tokenmeta::TokenMeta(1, 2, { 2, 4 }, &document1) };
        firstIndex["you"] = { tokenmeta::TokenMeta(1, 1, { 3 }, &document1) };

        std::map<std::string, std::set<tokenmeta::TokenMeta>> secondIndex;
        secondIndex["world"] = { tokenmeta::TokenMeta(2, 1, { 2 }, &document2) };
        secondIndex["around"] =  { tokenmeta::TokenMeta(2, 1, { 1 }, &document2) };
        secondIndex["eighty"] = { tokenmeta::TokenMeta(2, 1, { 3 }, &document2) };
        secondIndex["days"] = { tokenmeta::TokenMeta(2, 1, { 4 }, &document2) };


        Indexer indexer = Indexer();
        std::map<std::string, std::set<tokenmeta::TokenMeta>> returnedIndex;
        returnedIndex = indexer.joinIndexes(returnedIndex, firstIndex);
        returnedIndex = indexer.joinIndexes(returnedIndex, secondIndex);

        CHECK( returnedIndex.size() == 6 );
        CHECK( returnedIndex["hello"] ==  hello );
        CHECK( returnedIndex["world"] == world );
        CHECK( returnedIndex["you"] == you );
        CHECK( returnedIndex["around"] ==  around );
        CHECK( returnedIndex["eighty"] == eighty );
        CHECK( returnedIndex["days"] == days );
    }

    SECTION("write index to file.") {
        docmeta::DocumentMeta document1 = docmeta::DocumentMeta(1, "Hello, World! How are you world?", INDEXING_TESTING_ROOT_DIR "/testing-documents/demo.txt");
        docmeta::DocumentMeta document2 = docmeta::DocumentMeta(2, "Around the World in Eighty Days", INDEXING_TESTING_ROOT_DIR "/testing-documents/demo2.txt");

        std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
        index["hello"] = { tokenmeta::TokenMeta(1, 1, { 1 }, &document1) };;
        index["world"] =  { tokenmeta::TokenMeta(1, 2, { 2, 4 }, &document1) };
        index["you"] = { tokenmeta::TokenMeta(1, 1, { 3 }, &document1) };

        nlohmann::json expectedOutput = index;

        Indexer indexer = Indexer();
        indexer.writeIndexToFile(index, indexPath);

        std::ifstream file(indexPath);
        std::string resultOutput((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        file.close();

        REQUIRE( expectedOutput.dump(4) == resultOutput );
    }
}