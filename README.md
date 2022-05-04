<a id="top"></a>
## What is Seeker?

Seeker is a modular search engine sandbox built in C++. The goal is to allow you to combine different implementations of each component to have the search engine you need. 

**Features**
* Web search &rarr; Search through web pages
* Document search &rarr; Search through local folder

## Limitations
Currently, Seeker only supports an on-the-fly architecture. This means that the search is done sequentially without any threads involved. The advantage is that it only runs on demand. However, this type of architecture is not suited for a large data source. Additionally, the is no support for multi-word queries, and documents are only found if they include the exact query term.
> **Note:** &nbsp; Currently, a search engine with a pre-computed index gets developed in the `pre-computed` branch.

## Requirements
- [gumbo](https://github.com/google/gumbo-parser)
- [glog](https://github.com/google/glog)
- [libcurl4-openssl-dev](https://everything.curl.dev/get)
> **NOTE:** &nbsp; It is recommended to use a package manager (for example, [vcpkg](https://vcpkg.io/en/index.html))

## How to use it
This documentation comprises these parts:

* [Library](docs/library.md#top) - how to add the library to your project
* [Executable](docs/executable.md#top) - how to run the search engine
* [Reference section](docs/README.md#top) - all the details


Copyright © 2022 Maximilian Streitberger maximilian.streitberger@code.berlin @maxstreitberger
