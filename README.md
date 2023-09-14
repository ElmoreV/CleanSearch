# CleanSearch

Cleansearch is [(for me)](#history) 'the' way to find a file on Windows Vista, XP, Windows 7/8/8.1/10.
Features:

- Finds complete filenames, with and without extension
- Finds partial filenames
- Basic AND, OR and grouping parser (see [usage](#usage)]
- [Speedy](#speed)
- Real-time update as soon as a file is found
- Portable (no install needed)
- Can search for approximate [file size](#file-size) (within 20%)
- Will not use any external file, just the Win32API
- Opens File Explorer on the file if double clicked.
- Determines most used drive automatically



## Speed
On an average SSD-drive with 500.000 files, it searches the entire drive within 10 seconds. The actual search query has little impact on the time it takes.

#### Bottlenecks
The slowest part of the program is the Win32API, and I had a plan to speed it up using the Master Boot File, which contains all the file records in a list, but since Windows 7 or 8, they locked it behind System Permission (which sounds smart actually).\
Another speedup can probably be found in the rendering of the file view. It takes about 30% of the search time. It would probably need to be done asynchronously.


## History
I made the program when I found that Windows Search never found the files that I wanted. 14 years later, it still doesn't. Back then, I couldn't find a GUI program that could do it on-the-fly, without creating extra files or an index beforehand. That's why I made my own, including a mini-parser (which was a challenge).\
Other great software I encountered were:

- [Everything](https://www.voidtools.com/)
- If you know a good, specific file search GUI for Windows, let me know!

# Installation

## Binary
Download the Windows binary from the GitHub repository.

## From source
What worked for me is, run gcc/mingw64 in the root (repository) directory with command line arguments:
- -DUNICODE (to allow wchar_t)
- -Wno-conversion-null (surpress warnings)
- -Wno-write-strings (surpress warnings)
- -mwindows (include the windows dll)
- Icon.res (incorporate the icon)

giving a final command of :


```
g++ src\*.cpp -fdiagnostics-color=always  -g -o CleanSearch.exe -O2 -DUNICODE -D_UNICODE -Wno-conversion-null -Wno-write-strings -mwindows Icon.res
strip CleanSearch.exe
```
with the final strip to remove Debug Symbols (and make the executable smaller).

# Usage
### Basic search
You can search for exact matches of 'MyFile', which also searches for the same file with any extension, like 'MyFile.exe' or 'MyFile.pdf' by inputting it in the top text field.\
#### Folder
You can limit the search from a specific folder downwards by specifying this second text field. The default folder is the hard drive or mounted drive with the most used disk space.\
#### Buttons
- 'Part of a whole word': You can search for a part of a filename such as 'File' to find 'MyFile.exe' and 'FileManager.exe', etc. \
- 'Case Sensitive': If checked, with only match if the capitalization of letters is identical.\
- 'Include the windows directory into the search (slower)' Also search through the Windows folder. This is an old setting, which might warrant deletion.\
- 'Fast'/'Stable': it will disable screen updates (Fast) or enable them(Stable). Saves about 30-50% of the search time, but makes the program unresponsive.\
- 'START','STOP!','Continue': With these, you can Start, pause, restart and continue the search.

### Parser
The parser allows for some neat tricks, such as looking for multiple things at once. To look for 'MyFile' and 'MyOtherFile' at the same time, do 'MyFile|MyOtherFile'.\ The parser recognizes '|' as the OR-operator, and '&' as the AND-operator. You can also use the angled brackets '<' and '>' to group different searches. This makes a query like '<A | B> & <C | D>' valid, and pretty powerful!\
Included are some special macros (using the '?' character at the end) to search for a collection of different file types. Included are:

- music?
- sounds?
- videos?
- images?
- documents?

which use the parser underneath, e.g. documents? translates into <.doc|.docx|.pdf|.xml|.txt|.htm> 

### File size
The filsize knows identifiers like:
- B for bytes
- kiB, MiB,..., PiB for kibibytes until pebibytes
- kB,MB,...,PB for kilobytes until petabytes

Example: 20 MB.

# Future plans

This project is in principle done, however, there are some improvements to be made:

- Update README.md with a nice picture.
- Continuing the search should not clear the view (unless it is full at 10_000 files).
- Rename 'Start/Stop/Continue' to make more sense.
- Update the macros to be more modern.
- Add more useful macros.
- Find the 'Home Directory' better.
- Remove the errorlog (unless a flag is set over the CLI.)
- Improve layout and UX.
- Make the screen update asynchronous.
- Add automated tests and benchmarks
- Test on different Windows installations
- Update to modern C++.
- Find a faster way to get all the files.
- Refactor large codefiles (such as Search.cpp)
- Add a MAKEFILE to the build system.
- Make the parser regex capable.
- Improve the filesize filtering.
- Apply the parser to the folder selection

For the rest, just enjoy!
