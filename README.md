"Frequency Counter" project
===========================

Command line tool "fq": outputs the frequency table of the given input.

    $ fq README.md	# Counted only the first 20 chars as example
    > Final frequency table
	Symbol    Frequency   Pos
	-------------------------
	'e' 65            3     0
	' ' 20            2     1
	'"' 22            2     2
	'n' 6E            2     3
	'r' 72            2     4
	'u' 75            2     5
	'C' 43            1     6
	'F' 46            1     7
	'c' 63            1     8
	'o' 6F            1     9
	'q' 71            1     A
	't' 74            1     B
	'y' 79            1     C
	-------------------------
    Size: 20 - Number of symbols: 13
    
    $ echo "Hello world" | fq
    > Final frequency table
	Symbol    Frequency   Pos
	-------------------------
	'l' 6C            3     0
	'o' 6F            2     1
	'.' 0A            1     2
	' ' 20            1     3
	'H' 48            1     4
	'd' 64            1     5
	'e' 65            1     6
	'r' 72            1     7
	'w' 77            1     8
	-------------------------
    Size: 12 - Number of symbols: 9

The symbols are sorted from the highest to the lowest
frequencies, and in case two bytes have the same
frequency they are sorted "alphabetically".


### Verbose mode

Executing it with the `-v` (verbose) argument,
it iterates the file byte by byte and shows the
frequencies each time:

	$ echo 'banana!!' | fq -v
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            1     0 *
	'b' 62            1     1
	-------------------------
	Size: 2 - Number of symbols: 2
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            1     0
	'b' 62            1     1
	'n' 6E            1     2 *
	-------------------------
	Size: 3 - Number of symbols: 3
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            2     0 <
	'b' 62            1     1
	'n' 6E            1     2
	-------------------------
	Size: 4 - Number of symbols: 3
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            2     0
	'n' 6E            2     1 ^ (+1)
	'b' 62            1     2
	-------------------------
	Size: 5 - Number of symbols: 3
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            3     0 <
	'n' 6E            2     1
	'b' 62            1     2
	-------------------------
	Size: 6 - Number of symbols: 3
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            3     0
	'n' 6E            2     1
	'!' 21            1     2 *
	'b' 62            1     3
	-------------------------
	Size: 7 - Number of symbols: 4
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            3     0
	'!' 21            2     1 ^ (+1)
	'n' 6E            2     2
	'b' 62            1     3
	-------------------------
	Size: 8 - Number of symbols: 4
	
	
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            3     0
	'!' 21            2     1
	'n' 6E            2     2
	'.' 0A            1     3 *
	'b' 62            1     4
	-------------------------
	Size: 9 - Number of symbols: 5
	
	
	> Final frequency table
	Symbol    Frequency   Pos
	-------------------------
	'a' 61            3     0
	'!' 21            2     1
	'n' 6E            2     2
	'.' 0A            1     3
	'b' 62            1     4
	-------------------------
	Size: 9 - Number of symbols: 5


And indicator at the right marks what is the current
symbol found, with `*` when the symbol is found for the first
time, `<` when isn't the first time and the position in the
table didn't change, and `^ (+NUM)` when not the first time
but the position in the table has changed _NUM_ positions.


Build and execute
-----------------

The project uses *CMakes* to build the executable.

1. From the source directory, generate the "Makefile":

       $ cmake .

2. *Make* the project and generate the executable "fq".

       $ make

3. Then execute with (use with `-h` to see available options):

       $ out/fq

4. To install the executable `fq` into `/usr/local/bin`:

       $ sudo make install

   To install into another folder, when executing the step 1
   overwrite the `CMAKE_INSTALL_PREFIX` variable, eg.
   to install into the `/usr/bin` folder:

       $ cmake -DCMAKE_INSTALL_PREFIX=/usr .

To clean the compiled binaries and compiled modules:

    $ make clean

To clean all the compiled binaries and the "Makefiles" files:

    $ make clean-all


### Tests

Tests are built with CHEAT <https://github.com/Tuplanolla/cheat>,
a unit testing framework for C/C++, and can be executed
with CMake CTest:

    $ make test

To run the tests and see all the output in the console (specially
if the tests fail), first build the project (`make`) and then execute:

    $ out/test_fq


About
-----

Source: https://github.com/mrsarm/fq

Authors: (2015-2021) Mariano Ruiz <mrsarm@gmail.cm>

License: LGPL-3
