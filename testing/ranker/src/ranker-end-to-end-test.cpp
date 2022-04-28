#include <catch2/catch_test_macros.hpp>
#include "ranker.hpp"

TEST_CASE("Ranker returns correct documents", "[ranker]") {
    std::set<docmeta::DocumentMeta> store = { 
        docmeta::DocumentMeta(1, "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", "Some Example Path #1"),
        docmeta::DocumentMeta(2, "Ut bibendum leo ac neque vehicula fermentum. Amet vitae lorem.", "Some Example Path #2"),
        docmeta::DocumentMeta(3, "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", "Some Example Path #3")
    };

    std::set<docmeta::DocumentMeta>::iterator it = store.begin();
    const docmeta::DocumentMeta* first_doc = &(*it);
    std::advance(it, 1);
    const docmeta::DocumentMeta* second_doc = &(*it);
    std::advance(it, 1);
    const docmeta::DocumentMeta* third_doc = &(*it);
    
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;
    index["lorem"] = { tokenmeta::TokenMeta(1, 1, { 1 }, first_doc), tokenmeta::TokenMeta(2, 1, { 10 }, second_doc) };
    index["ipsum"] = { tokenmeta::TokenMeta(1, 1, { 2 }, first_doc) };
    index["dolor"] = { tokenmeta::TokenMeta(1, 1, { 3 }, first_doc), tokenmeta::TokenMeta(3, 1, { 3 }, third_doc) };
    index["sit"] = { tokenmeta::TokenMeta(1, 2, { 4, 7 }, first_doc), tokenmeta::TokenMeta(3, 1, { 5 }, third_doc) };
    index["amet"] = { tokenmeta::TokenMeta(1, 1, { 5 }, first_doc), tokenmeta::TokenMeta(2, 1, { 8 }, second_doc) };
    index["elit"] = { tokenmeta::TokenMeta(1, 1, { 8 }, first_doc), tokenmeta::TokenMeta(3, 2, { 4, 10 }, third_doc) };

    std::vector<docmeta::DocumentMeta> expectedDocs1 = { 
        docmeta::DocumentMeta(1, "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", "Some Example Path #1"),
        docmeta::DocumentMeta(2, "Ut bibendum leo ac neque vehicula fermentum. Amet vitae lorem.", "Some Example Path #2"),
    };
    
    std::vector<docmeta::DocumentMeta> expectedDocs2 = { 
        docmeta::DocumentMeta(1, "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", "Some Example Path #1"),
        docmeta::DocumentMeta(3, "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", "Some Example Path #3")
    };

    std::vector<docmeta::DocumentMeta> expectedDocs3 = { 
        docmeta::DocumentMeta(3, "Vestibulum accumsan dolor elit sit iaculis leo. Praesent et elit.", "Some Example Path #3"),
        docmeta::DocumentMeta(1, "Lorem ipsum dolor sit amet, consectetur sit elit. Vestibulum bibendum.", "Some Example Path #1")
    };
    
    Ranker ranker = Ranker(&store, &index);
    std::vector<docmeta::DocumentMeta> returnedDocs1 = ranker.searchFor("lorem");
    std::vector<docmeta::DocumentMeta> returnedDocs2 = ranker.searchFor("sit");
    std::vector<docmeta::DocumentMeta> returnedDocs3 = ranker.searchFor("elit");

    CHECK( returnedDocs1 == expectedDocs1 );
    CHECK( returnedDocs2 == expectedDocs2 );
    CHECK( returnedDocs3 == expectedDocs3 );
}