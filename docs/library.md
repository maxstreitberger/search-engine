<a id="top"></a>
# Library

Besides using the executables you also have the possibility to integrate the search engine into your C++ application. 

## Requirements

- [glog](https://github.com/google/glog)
- curl ⇒ (How to add curl to cmake)

## Installation

1. Download repository
2. Execute the `.config` script
3. Execute the `.install` script

## Usage

```
find_package(Seeker CONFIG REQUIRED)
add_executable(your_project ...)
target_link_libraries(your_project PRIVATE Seeker::Seeker)
```

```
#include <seeker/doc_crawler.hpp>
#include <seeker/doc_store.hpp>
#include <seeker/indexer.hpp>
#include <seeker/ranker.hpp>

...

std::string specialCharsPath = "/path/to/specialChars.txt";
std::string stopwordsPath = "/path/to/stopwords.txt";

std::set<docmeta::DocumentMeta> crawler_found_documents;
std::set<docmeta::DocumentMeta> documents_in_store;
std::vector<docmeta::DocumentMeta> repository;
std::map<std::string, std::set<tokenmeta::TokenMeta>> index;

DocStore store = DocStore(&crawler_found_documents, &documents_in_store, &repository);
DocumentCrawler crawler = DocumentCrawler(store, &crawler_found_documents, SEARCHENGINE_ROOT_DIR "/dummy-text");
Indexer indexer = Indexer(specialCharsPath, stopwordsPath, &repository, &index);
Ranker ranker = Ranker(&documents_in_store, &index);

std::vector<docmeta::DocumentMeta> foundDocuments = engine::runSearch(crawler, indexer, ranker, searchTerm);
```

If the search engine has found documents that contain the search term, it returns a list of all documents: `[{ document_id: 1, content: 'Hello, world, path: test.txt' }]`.

On document contains an `id`, the document `content`, and the `path` to the document.