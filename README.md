# Search Engine

Beginning of a fully modular search engine.

## Compiling and Running

In the root directory, execute the following to create the CMake files:

`cmake -S . -B bin`

Afterward, go into the `bin` directory you execute the `make` command, which will compile the project.

Now you can run the search engine by simply running:

`./searchengine`

```
cmake -S . -B bin
cd bin
make
./searchengine
```

## Testing
The tests are placed in every module (`crawling`, `indexing`, `ranking`) in the `testing` directory. The name of the testing executable has the format of `[module-name]_tests`.
To run a test, you have to go into the `bin` directory and then into the `testing` directory of the desired module you want to test.
Example:
```
cd bin/modules/crawling/testing
./crawling_tests
```

## Architecture
### Crawler
#### Document Crawler
The document crawler collects text documents from a specified directory. The crawler goes through every sub-directory and registers a found document. 

#### Web Crawler
The web crawler retrieves the HTML document from a given origin path and extracts all URLs that link to a different page and not to a section on the page. 

#### Document Store
The document store receives the found documents from the crawler and checks if a document is not in the store yet or got updated. This means that the document store will ignore unchanged documents. Changes and additions are saved in the document store and additionally stored in the repository, where the indexer can pick these documents up.

### Indexer
The indexer splits every document into individual tokens. Next, every token is checked for special characters (`?`, `!`, `<`, `>`, `-`, `_`, etc.) and if the token is a stopword (also called function word). After unnecessary tokens and characters are removed, the indexer collects the position and the number of appearances of each token in the document. Finally, the indexer stores each token together with its meta-information in the index.

### Ranker
Currently, the ranker is very basic in the way that it searches for a direct match for the query in the index. Before the search, the ranker removes all possible whitespace around the query and transforms it to lower case. Next, it searches for a perfect match in the index. If a match is found, it collects all the documents in which the found token appears. Before returning the documents to the user, the ranker sorts the documents by the number of appearances of the token.
