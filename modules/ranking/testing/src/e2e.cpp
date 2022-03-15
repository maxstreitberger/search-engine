#include <catch2/catch_test_macros.hpp>
#include "ranker.hpp"

TEST_CASE("Ranker returns correct documents", "[ranker]") {
    std::string indexPath = RANKING_TESTING_ROOT_DIR "/index.json";
    std::string storePath = RANKING_TESTING_ROOT_DIR "/store.json";

    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    index["lorem"] = { tokenmeta::TokenMeta(1, 1, { 1 }), tokenmeta::TokenMeta(2, 1, { 10 }) };
    index["ipsum"] = { tokenmeta::TokenMeta(1, 1, { 2 }) };
    index["dolor"] = { tokenmeta::TokenMeta(1, 1, { 3 }), tokenmeta::TokenMeta(3, 1, { 3 }) };
    index["sit"] = { tokenmeta::TokenMeta(1, 2, { 4, 7 }), tokenmeta::TokenMeta(3, 1, { 5 }) };
    index["amet"] = { tokenmeta::TokenMeta(1, 1, { 5 }), tokenmeta::TokenMeta(2, 1, { 8 }) };
    index["elit"] = { tokenmeta::TokenMeta(1, 1, { 8 }), tokenmeta::TokenMeta(3, 2, { 4, 10 }) };

    nlohmann::json indexJson = index;

    std::ofstream file(indexPath);
    file << indexJson.dump(4);
    file.close();

    std::set<docmeta::DocumentMeta> store = { 
        docmeta::DocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        ),
        docmeta::DocumentMeta(
            2,
            "Ut bibendum leo ac neque vehicula fermentum. Amet vitae lorem.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt"
        ),
        docmeta::DocumentMeta(
            3, 
            "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo3.txt"
        )
    };

    nlohmann::json j = store;

    std::ofstream docStore(storePath);
    docStore << j;
    docStore.close();

    std::vector<docmeta::DocumentMeta> expectedDocs1 = { 
        docmeta::DocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        ),
        docmeta::DocumentMeta(
            2,
            "Ut bibendum leo ac neque vehicula fermentum. Amet vitae lorem.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo2.txt"
        ),
    };
    std::vector<docmeta::DocumentMeta> expectedDocs2 = { 
        docmeta::DocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        ),
        docmeta::DocumentMeta(
            3, 
            "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo3.txt"
        )
    };
    std::vector<docmeta::DocumentMeta> expectedDocs3 = { 
        docmeta::DocumentMeta(
            3, 
            "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo3.txt"
        ),
        docmeta::DocumentMeta(
            1, 
            "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", 
            RANKING_TESTING_ROOT_DIR "/testing-documents/demo.txt"
        )
    };
    
    Ranker ranker = Ranker(indexPath, storePath);
    std::vector<docmeta::DocumentMeta> returnedDocs1 = ranker.searchFor("lorem");
    std::vector<docmeta::DocumentMeta> returnedDocs2 = ranker.searchFor("sit");
    std::vector<docmeta::DocumentMeta> returnedDocs3 = ranker.searchFor("elit");

    CHECK( returnedDocs1 == expectedDocs1 );
    CHECK( returnedDocs2 == expectedDocs2 );
    CHECK( returnedDocs3 == expectedDocs3 );
}