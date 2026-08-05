# clipstore

clipstore is a fast, lightweight, terminal-based snippet manager written in C. It allows you to store, list, retrieve, and delete text snippets directly from your CLI, storing them safely in ~/.clipstore.

## Features

* Lightweight & Fast: Zero external dependencies outside standard C libraries.
* Isolated Storage: Automatically creates and uses a dedicated directory (~/.clipstore).
* Simple Interface: Easy-to-remember flags for common snippet management operations.

## Installation

Prerequisites:

* GCC or Clang
* Make

Building and Installing:

1. Clone the repository:
git clone [https://github.com/TechCore3/clipstore.git](https://www.google.com/search?q=https://github.com/TechCore3/clipstore.git)
2. Navigate to directory:
cd clipstore
3. Build and install:
make
sudo make install

This builds the binary and places it in /usr/local/bin.

To uninstall:
sudo make uninstall

To clean build artifacts:
make clean

## Usage

clipstore  [file] [contents]

Options:
-s    Store snippet contents into a file
-S             Show contents of a stored snippet
-l                   List all saved snippet files
-r             Remove a stored snippet
-h, --help           Display usage information

## Examples

1. Save a snippet:
clipstore -s quicknote "Remember to run updates on server"
2. List all saved snippets:
clipstore -l
3. Show a snippet's contents:
clipstore -S quicknote
4. Delete a snippet:
clipstore -r quicknote

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for de
