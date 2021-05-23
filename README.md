"Frequency Counter" project
===========================

This project is a command line tool named "fq": outputs
the frequency table of the given input.

    $ fq README.md	# Counted only the first 20 chars as example
    > Final frequency table
    Symb.: 'e' 65   Freq.: 3        Pos.:  0
    Symb.: ' ' 20   Freq.: 2        Pos.:  1
    Symb.: '"' 22   Freq.: 2        Pos.:  2
    Symb.: 'n' 6E   Freq.: 2        Pos.:  3
    Symb.: 'r' 72   Freq.: 2        Pos.:  4
    Symb.: 'u' 75   Freq.: 2        Pos.:  5
    Symb.: 'C' 43   Freq.: 1        Pos.:  6
    Symb.: 'F' 46   Freq.: 1        Pos.:  7
    Symb.: 'c' 63   Freq.: 1        Pos.:  8
    Symb.: 'o' 6F   Freq.: 1        Pos.:  9
    Symb.: 'q' 71   Freq.: 1        Pos.:  A
    Symb.: 't' 74   Freq.: 1        Pos.:  B
    Symb.: 'y' 79   Freq.: 1        Pos.:  C
    Size: 20 - Number of symbols: 13
    
    $ echo "Hello world" | fq
    > Final frequency table
    Symb.: 'l' 6C   Freq.: 3        Pos.:  0
    Symb.: 'o' 6F   Freq.: 2        Pos.:  1
    Symb.: '.' 0A   Freq.: 1        Pos.:  2
    Symb.: ' ' 20   Freq.: 1        Pos.:  3
    Symb.: 'H' 48   Freq.: 1        Pos.:  4
    Symb.: 'd' 64   Freq.: 1        Pos.:  5
    Symb.: 'e' 65   Freq.: 1        Pos.:  6
    Symb.: 'r' 72   Freq.: 1        Pos.:  7
    Symb.: 'w' 77   Freq.: 1        Pos.:  8
    Size: 12 - Number of symbols: 9

The frequencies are sorted from the highest to
the lowest, and in case two bytes have the same
frequency they are sorted "alphabetically".

Executing it with the `-v` (verbose) argument,
it iterates the file byte by byte and shows the
frequencies each time:

	$ echo "banana" | fq -v
	Symb.: 'a' 61   Freq.: 1        Pos.:  0 *
	Symb.: 'b' 62   Freq.: 1        Pos.:  1
	Size: 2 - Number of symbols: 2
	Symb.: 'a' 61
	
	Symb.: 'a' 61   Freq.: 1        Pos.:  0
	Symb.: 'b' 62   Freq.: 1        Pos.:  1
	Symb.: 'n' 6E   Freq.: 1        Pos.:  2 *
	Size: 3 - Number of symbols: 3
	Symb.: 'n' 6E
	
	Symb.: 'a' 61   Freq.: 2        Pos.:  0 *
	Symb.: 'b' 62   Freq.: 1        Pos.:  1
	Symb.: 'n' 6E   Freq.: 1        Pos.:  2
	Size: 4 - Number of symbols: 3
	Symb.: 'a' 61
	
	Symb.: 'a' 61   Freq.: 2        Pos.:  0
	Symb.: 'n' 6E   Freq.: 2        Pos.:  1 *
	Symb.: 'b' 62   Freq.: 1        Pos.:  2
	Size: 5 - Number of symbols: 3
	Symb.: 'n' 6E
	
	Symb.: 'a' 61   Freq.: 3        Pos.:  0 *
	Symb.: 'n' 6E   Freq.: 2        Pos.:  1
	Symb.: 'b' 62   Freq.: 1        Pos.:  2
	Size: 6 - Number of symbols: 3
	Symb.: 'a' 61
	
	Symb.: 'a' 61   Freq.: 3        Pos.:  0
	Symb.: 'n' 6E   Freq.: 2        Pos.:  1
	Symb.: '.' 0A   Freq.: 1        Pos.:  2 *
	Symb.: 'b' 62   Freq.: 1        Pos.:  3
	Size: 7 - Number of symbols: 4
	Symb.: '.'  A


Build and execute
-----------------

The project uses *CMakes* to build the executable.

1. From the source directory, generate the "Makefile":

       $ cmake .

2. *Make* the project and generate the executable "fq".

       $ make

3. Then execute with:

       $ out/fq

4. To install the executable `fq` into `/usr/local/bin`:

       $ sudo make install

   To install into another folder, execute the step 1
   overwriting the `CMAKE_INSTALL_PREFIX` variable, eg.
   to install into the `/usr/bin` folder:

       $ cmake -DCMAKE_INSTALL_PREFIX=/usr .

To clean the compiled binaries and compiled modules:

    $ make clean

To clean all the compiled binaries and the "Makefiles" files:

    $ make clean-all


### Tests

Tests are built with CHEAT <http://users.jyu.fi/~sapekiis/cheat/index.html>,
an unit testing framework for C/C++ programming language.

To run the tests, first build the project and then execute:

    $ out/test_fq


About
-----

Source: https://github.com/mrsarm/fq

Authors: (2015-2021) Mariano Ruiz <mrsarm@gmail.cm>

License: LGPL-3
