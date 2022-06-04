<a id="top"></a>
# Components
<p align="center">
  <img src="https://user-images.githubusercontent.com/27073329/165269221-42a8d92d-58d7-4dd9-b3af-7116ceaba706.png" width="750">
</p>

<a id="components-crawler"></a>
## Crawler

|     Type    |               |
| ------------- | ------------- |
| **Web**       | The web crawler retrieves the HTML document from a given origin path and extracts all URLs that link to a different page and not to a section on the page.  |
| **Document**  | The document crawler collects text documents from a specified directory. The crawler goes through every sub-directory and registers a found document.  |

<a id="components-document-store"></a>
## Document Store

The document store receives the found documents from the crawler and checks if a document is not in the store yet or got updated. This means that the document store will ignore unchanged documents. Changes and additions are saved in the document store and in the repository, where the indexer can pick the documents up.

<a id="components-indexer"></a>
## Indexer

The indexer splits every document into individual tokens. Next, every token is checked for special characters (`?`, `!`, `<`, `>`, `-`, `_`, etc.) and if the token is a [stop word](https://en.wikipedia.org/wiki/Stop_word). After unnecessary tokens and characters are removed, the indexer collects the position and the number of appearances of each token in the document — finally, the indexer stores each token together with its meta-information in the index.

<a id="components-ranker"></a>
## Ranker

Currently, the ranker is very basic because it only searches for a direct match for the query in the index. Before the search, the ranker removes all possible whitespace around the query and transforms it to lower case. Next, it searches for a perfect match in the index. If a match is found, it collects all the documents in which the found token appears. Before returning the documents to the user, the ranker sorts the documents by the number of appearances of the token.



# ADD IMAGE
