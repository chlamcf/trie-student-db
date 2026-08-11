# Student Record Management System

[![C++](https://img.shields.io/badge/C++-17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/build-CMake-064F8C?style=flat-square&logo=cmake&logoColor=white)](https://cmake.org/)

A student record system backed by a **custom-built Trie**, supporting fast lookup by SID, name, and major.

## Features

- **O(k) SID lookup** via a digit-indexed trie (`'0'`–`'9'`), where *k* is the key length.
- **Case-insensitive name/major search** using a letter-indexed trie (`'A'`–`'Z'`).
- **Multi-field queries**: search by given name, family name, full name, major, or list all records.
- **Safe insertion** that rejects duplicate SIDs via exception handling rather than silent overwrites.
- **Generic, reusable `Trie<V, BeginChar, EndChar>` template** decoupled from the student domain — usable for any bounded-alphabet key.


## Build & Run

Requires C++17 and CMake ≥ 3.15.

```bash
git clone https://github.com/chlamcf/student-record-system.git
cd student-record-system
mkdir build && cd build
cmake ..
make
./student_system
```

# Notes

- The content was originally written for coursework and was then further developed.
