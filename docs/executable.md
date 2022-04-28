<a id="top"></a>
# Executable

There are two executables available: **OnTheFlyWeb** and **OnTheFlyDocumentSearch**. Both are used in the same way; the only difference is where the search engine searches. The `Web` ending indicates that the search engine searches through a web page, whereas the `Document` ending shows that the search engine searches through a particular folder.

## Setup

To run the executables, you have first to download and extract the package. Afterward, you have two possibilities: <br>
- **Option 1 _(Preferred option)_** <br>
Run the executable from inside the `bin` directory of the extracted folder. You have to specify the full path to the executable you want to run. So if you are inside the directory, you can simply add `./` before the executable name.
- **Option 2** <br>
Add the bin directory from the extracted folder to the `PATH` environment variable.
  * Guide for [Windows](https://medium.com/@kevinmarkvi/how-to-add-executables-to-your-path-in-windows-5ffa4ce61a53), [Linux](https://opensource.com/article/17/6/set-path-linux), and [MacOS](https://www.architectryan.com/2012/10/02/add-to-the-path-on-mac-os-x-mountain-lion/).

> **Note:** &nbsp; The executables are also installed when the library is installed if you want to know how to install the library, click [here](library.md#library-installation).

## Usage

```
$ OnTheFlyWebSearch -p [url] -s [search_term]
```

> Learn more about the available command line options [here](command-line.md#top).