<a id="top"></a>
# Command Line

**Contents**

[Specify search term](#specify-search-term)<br>
[Set search path](#set-search-path)<br>
[Limit search depth](#limit-search-depth)<br>
[Limit number of returned items](#limit-number-of-returned-items)<br>
[Show help](#show-help)<br>

Seeker supports the following command line options. Click one of the following links to take you straight to that option - or scroll on to browse the available options.

[`-s, —-search`](#specify-search-term)<br>
[`-p , —-path`](#set-search-path)<br>
[`-d , —-depth`](#limit-search-depth)<br>
[`-m , —-max`](#limit-number-of-returned-items)<br>
[`-h , —-help, -?`](#show-help)<br>

<br>

<a id="specify-search-term"></a>
## Specify search term

```
-s, —-search <search term>
```

This allows you to specify what the search engine should search for. Without it you don't get any items back.

<a id="set-search-path"></a>
## Set search path

```
-p , —-path <url/absolut path>
```

Use this option to specify where the search engine should search. If you are using the document crawler you need to specify the absolute path to the directory. 

<a id="limit-search-depth"></a>
## Limit search depth 

```
-d , —-depth <number>
```

This option allows you to limit how deep the search engine should search. For example, if you have a directory with 10 sub-directories in it, you can only crawl the first 5 directories.

<a id="limit-number-of-returned-items"></a>
## Limit number of returned items

```
-m , —-max <number>
```

If this option is set to a non zero number, the search engine will only return x amount of items. However, if zero is given, you will get all items that the search engine has found.

<a id="show-help"></a>
## Help

```
-h , —-help, -?
```
Displays a usage, options, and arguments.