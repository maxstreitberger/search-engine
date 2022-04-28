<a id="top"></a>
# Library

Besides using the executables you also have the possibility to integrate the search engine into your C++ application. 

<a id="library-requirements"></a>
## Requirements

- [gumbo](https://github.com/google/gumbo-parser)
- [glog](https://github.com/google/glog)
- [libcurl](https://curl.se/libcurl/)
> **NOTE:** &nbsp; _gumbo_ and _libcurl_ are only required for web searching.

<a id="library-installation"></a>
## Installation

1. Clone the repository: 
   ```
   git clone https://github.com/maxstreitberger/search-engine.git
   ```
2. Execute the `.install` script

<a id="library-usage"></a>
## Usage

```
find_package(Seeker CONFIG REQUIRED)
add_executable(your_project ...)
target_link_libraries(your_project PRIVATE Seeker::Seeker)
```

```
#include <iostream>
#include <map>
#include <glog/logging.h>
#include <seeker/doc_crawler.hpp>
#include <seeker/doc_store.hpp>
#include <seeker/indexer.hpp>
#include <seeker/ranker.hpp>
#include <seeker/engine.hpp>

int main(int argc, char const *argv[]) {
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]);

    std::string searchTerm = "lorem";
    std::string path = "/path/to/dummy-text";
    int maxReturnDocumentCount = 0;

    std::string specialCharsPath = "/path/to/special.txt";
    std::string stopwordsPath = "/path/to/stopwords.txt";

    std::set<docmeta::DocumentMeta> crawler_found_documents;
    std::set<docmeta::DocumentMeta> documents_in_store;
    std::vector<docmeta::DocumentMeta> repository;
    std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

    DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
    DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, path);
    Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
    Ranker ranker = Ranker(&documents_in_store, &index, maxReturnDocumentCount);

    std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, searchTerm);

    if (foundDocuments.empty()) {
        std::cout << "No document(s) containing '" << searchTerm << "' found." << std::endl;
    } else {
        std::cout << "Found " << foundDocuments.size() << " document(s):" << std::endl;

        for (auto& doc: foundDocuments) {
            std::cout << doc << std::endl;
        }
    }

    return 0;
}
```

If the search engine has found documents that contain the search term, it returns a list of all documents. For example, `foundDocuments` could look like this:
```
[{ document_id: 1, content: 'Hello, world, path: test.txt' }]
```
