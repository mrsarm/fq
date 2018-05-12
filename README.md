"Frequency Counter" project
===========================

This project is a command line tool named "fq": outputs
the frequency table of the input file.

The project uses *CMakes* to build the executable.


Build and execute
-----------------

1. From the source directory, generate the "Makefile":

       $ cmake .

2. *Make* the project and generate the executable "fq".

       $ make

3. Then execute with:

       $ out/fq

To clean the compiled binaries and compiled modules:

    $ make clean

To clean all compiled files and generated "Makefiles":

    $ make clean-all


About
-----

Source: https://github.com/mrsarm/fq

Authors: (2015-2018) Mariano Ruiz <mrsarm@gmail.cm>

License: GPL-3
