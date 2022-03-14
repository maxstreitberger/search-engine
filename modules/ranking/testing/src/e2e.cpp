#include <catch2/catch_test_macros.hpp>
#include "ranker.hpp"

TEST_CASE("Ranker returns correct documents", "[ranker]") {
    std::string indexPath = RANKING_TESTING_ROOT_DIR "/index.json";
    std::string storePath = RANKING_TESTING_ROOT_DIR "/store.json";

    std::map<std::string, std::set<tokenmeta::RankerTokenMeta>> index;
    index["lorem"] = { tokenmeta::RankerTokenMeta(1, 1, { 1 }), tokenmeta::RankerTokenMeta(2, 1, { 10 }) };
    index["ipsum"] = { tokenmeta::RankerTokenMeta(1, 1, { 2 }) };
    index["dolor"] = { tokenmeta::RankerTokenMeta(1, 1, { 3 }), tokenmeta::RankerTokenMeta(3, 1, { 3 }) };
    index["sit"] = { tokenmeta::RankerTokenMeta(1, 2, { 4, 7 }), tokenmeta::RankerTokenMeta(3, 1, { 5 }) };
    index["amet"] = { tokenmeta::RankerTokenMeta(1, 1, { 5 }), tokenmeta::RankerTokenMeta(2, 1, { 8 }) };
    index["elit"] = { tokenmeta::RankerTokenMeta(1, 1, { 8 }), tokenmeta::RankerTokenMeta(3, 2, { 4, 10 }) };

    nlohmann::json indexJson = index;

    std::ofstream file(indexPath);
    file << indexJson.dump(4);
    file.close();

    std::set<docmeta::RankerDocumentMeta> store = { 
        docmeta::RankerDocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        ),
        docmeta::RankerDocumentMeta(
            2,
            "Ut bibendum leo ac neque vehicula fermentum. Amet vitae lorem.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt"
        ),
        docmeta::RankerDocumentMeta(
            3, 
            "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo3.txt"
        )
    };

    nlohmann::json j = store;

    std::ofstream docStore(storePath);
    docStore << j;
    docStore.close();

    std::vector<docmeta::RankerDocumentMeta> expectedDocs1 = { 
        docmeta::RankerDocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        ),
        docmeta::RankerDocumentMeta(
            2,
            "Ut bibendum leo ac neque vehicula fermentum. Amet vitae lorem.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt"
        ),
    };
    std::vector<docmeta::RankerDocumentMeta> expectedDocs2 = { 
        docmeta::RankerDocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        ),
        docmeta::RankerDocumentMeta(
            3, 
            "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo3.txt"
        )
    };
    std::vector<docmeta::RankerDocumentMeta> expectedDocs3 = { 
        docmeta::RankerDocumentMeta(
            3, 
            "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo3.txt"
        ),
        docmeta::RankerDocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        )
    };
    
    Ranker ranker = Ranker(indexPath, storePath);
    std::vector<docmeta::RankerDocumentMeta> returnedDocs1 = ranker.searchFor("lorem");
    std::vector<docmeta::RankerDocumentMeta> returnedDocs2 = ranker.searchFor("sit");
    std::vector<docmeta::RankerDocumentMeta> returnedDocs3 = ranker.searchFor("elit");

    CHECK( returnedDocs1 == expectedDocs1 );
    CHECK( returnedDocs2 == expectedDocs2 );
    CHECK( returnedDocs3 == expectedDocs3 );
}