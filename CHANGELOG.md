"Frequency Counter" changelog
=============================


2.0.0
-----
- Frequencies are better formatted as a table.
- In verbose mode add better marks about the current symbol.
- Set the input stream as source either with the `-` as
  file argument or not file argument at all.
- Fix when interrupting the program with _Ctrl+C_ the
  frequency output was unsorted.
- Fix pointer truncate bug.
- Better error handling and error messages.
- A lot of code refactor and formatting. Replace tabs
  with 4 spaces in source code files.
- Remove redundant `freqlist.freqs`.
- CTest configuration and improved tests.
- Add CI setup on Github Actions.


1.2.0
-----

- Print symbol's hexadecimal code with '0' padding
  instead of blank ' ' space.
- In verbose mode the symbol found is highlighted
  in the table.
- Minor code refactor.


1.0.3
-----

- Minor refactor.


1.0.2
-----

- Sort the list at the end of the stream except
  for verbose mode to boost speed.
- Add test case.


1.0.1
-----

- Remove unnecessary buffer to read
  from the input file.


1.0
---

First release.
