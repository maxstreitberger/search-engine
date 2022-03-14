#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "ranker.hpp"

TEST_CASE("Ranker can", "[ranker]") {
    std::string indexPath = RANKING_TESTING_ROOT_DIR "/index.json";
    std::string storePath = RANKING_TESTING_ROOT_DIR "/store.json";

    std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> index;
    index["hello"] = { tokenmeta::RankerTokenMeta(1, 1, { 1 }) };
    index["world"] =  { tokenmeta::RankerTokenMeta(1, 2, { 2, 4 }), tokenmeta::RankerTokenMeta(2, 1, { 2 }) };
    index["you"] = { tokenmeta::RankerTokenMeta(1, 1, { 3 }) };
    index["around"] =  { tokenmeta::RankerTokenMeta(2, 1, { 1 }) };
    index["eighty"] = { tokenmeta::RankerTokenMeta(2, 2, { 3, 5 }) };
    index["days"] = { tokenmeta::RankerTokenMeta(2, 1, { 4 }) };
    index["miles"] = { tokenmeta::RankerTokenMeta(2, 1, { 6 }) };

    nlohmann::json indexJson = index;

    std::ofstream file(indexPath);
    file << indexJson.dump(4);
    file.close();

    std::set<docmeta::RankerDocumentMeta> store = { 
        docmeta::RankerDocumentMeta(1, "Hello, World! How are you world?", RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"),
        docmeta::RankerDocumentMeta(2, "Around the World in Eighty Days. Eighty miles", RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt")
    };

    nlohmann::json j = store;

    std::ofstream docStore(storePath);
    docStore << j;
    docStore.close();

    SECTION("transfrom search query (remove whitespace and to lowercase).") {
        std::string searchQuery = "  HELLO  ";
        std::string expectedQuery = "hello";

        Ranker ranker = Ranker();
        std::string returnedQuery = ranker.transformQuery(searchQuery);

        REQUIRE( returnedQuery == expectedQuery );
    }

    SECTION("can load index.") {
        Ranker ranker = Ranker();
        std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> returnedIndex = ranker.loadIndex(indexPath);

        std::set<tokenmeta::RankerTokenMeta> hello = { tokenmeta::RankerTokenMeta(1, 1, { 1 }) };
        std::set<tokenmeta::RankerTokenMeta> world = { tokenmeta::RankerTokenMeta(1, 2, { 2, 4 }), tokenmeta::RankerTokenMeta(2, 1, { 2 }) };
        std::set<tokenmeta::RankerTokenMeta> you = { tokenmeta::RankerTokenMeta(1, 1, { 3 }) };
        std::set<tokenmeta::RankerTokenMeta> around = { tokenmeta::RankerTokenMeta(2, 1, { 1 }) };
        std::set<tokenmeta::RankerTokenMeta> eighty = { tokenmeta::RankerTokenMeta(2, 2, { 3, 5 }) };
        std::set<tokenmeta::RankerTokenMeta> days = { tokenmeta::RankerTokenMeta(2, 1, { 4 }) }; 
        std::set<tokenmeta::RankerTokenMeta> miles = { tokenmeta::RankerTokenMeta(2, 1, { 6 }) }; 

        CHECK( returnedIndex.size() == 7 );
        CHECK( returnedIndex["hello"] ==  hello );
        CHECK( returnedIndex["world"] == world );
        CHECK( returnedIndex["you"] == you );
        CHECK( returnedIndex["around"] == around );
        CHECK( returnedIndex["eighty"] ==  eighty );
        CHECK( returnedIndex["days"] == days );
        CHECK( returnedIndex["miles"] == miles );
    }

    SECTION("can load store.") {
        Ranker ranker = Ranker();
        std::set<docmeta::RankerDocumentMeta> returnedStore = ranker.loadStore(storePath);

        REQUIRE( returnedStore == store );
    }

    SECTION("can retrieve token meta information.") {
        std::vector<tokenmeta::RankerTokenMeta> expectedInfo = {
            tokenmeta::RankerTokenMeta(1, 2, { 2, 4 }), 
            tokenmeta::RankerTokenMeta(2, 1, { 2 })
        };
        
        std::string query = "world";

        Ranker ranker = Ranker();
        std::vector<tokenmeta::RankerTokenMeta> returnedInfo = ranker.retrieveMetaInformations(&index, query);

        REQUIRE( returnedInfo == expectedInfo );
    }

    SECTION("can collect document ids.") {
        std::vector<tokenmeta::RankerTokenMeta> tokenMeta = { 
            tokenmeta::RankerTokenMeta(1, 1, { 1 } ), 
            tokenmeta::RankerTokenMeta(2, 1, { 1 } ) 
        }; 
        std::unordered_set<int> expectedResult = { 1, 2 };

        Ranker ranker = Ranker();
        std::unordered_set<int> doc_ids = ranker.filterDocIds(tokenMeta);

        REQUIRE( doc_ids == expectedResult );
    }

    SECTION("can return documents.") {
        std::vector<docmeta::RankerDocumentMeta> expectedDocuments = {
            docmeta::RankerDocumentMeta(2, "Around the World in Eighty Days. Eighty miles", RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt"),
        };

        Ranker ranker = Ranker();
        std::vector<docmeta::RankerDocumentMeta> returnedDocuments = ranker.collectDocuments(&store, { 2 });

        REQUIRE( returnedDocuments == expectedDocuments );
    }
}

