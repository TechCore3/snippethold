# clipstore

`clipstore` is a fast, lightweight, terminal-based snippet manager written in C. It allows you to store, list, retrieve, and delete text snippets directly from your CLI, storing them safely in `~/.clipstore`.

## Features

- **Lightweight & Fast**: Zero external dependencies outside standard C libraries.
- **Isolated Storage**: Automatically creates and uses a dedicated directory (`~/.clipstore`).
- **Simple Interface**: Easy-to-remember flags for common snippet management operations.

---

## Installation

### Prerequisites

- GCC or Clang
- Make

### Building and Installing

Clone the repository and install system-wide using `make`:

```bash
git clone [https://github.com/TechCore3/clipstore.git](https://github.com/TechCore3/clipstore.git)
cd clipstore
make
sudo make install
