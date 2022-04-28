<a id="top"></a>
# Command Line

**Contents**

[Specify search term](#specify-search-term)<br>
[Set search path](#set-search-path)<br>
[Limit number of returned items](#limit-number-of-returned-items)<br>
[Show help](#show-help)<br>

Seeker supports the following command line options. Click one of the following links to take you straight to that option - or scroll on to browse the available options.

[`-s, —-search`](#specify-search-term)<br>
[`-p , —-path`](#set-search-path)<br>
[`-m , —-max`](#limit-number-of-returned-items)<br>
[`-h , —-help, -?`](#show-help)<br>

<br>

<a id="specify-search-term"></a>
## Specify search term (requiered)

```
-s, —-search <search term>
```

This allows you to specify what the search engine should search for. Without it you don't get any items back.

<a id="set-search-path"></a>
## Set search path (requiered)

```
-p , —-path <url/absolut path>
```

Use this option to specify where the search engine should search. If you are using the document crawler you need to specify the absolute path to the directory. 

<a id="limit-number-of-returned-items"></a>
## Limit number of returned items

```
-m , —-max <number>
```

If this option is set to zero, you will get all items that the search engine has found. Otherwise, the search engine will only return x amount of items.

<a id="show-help"></a>
## Help

```
-h , —-help, -?
```
Displays a usage, options, and arguments.
