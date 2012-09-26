Course: CPSC 457
Instructor: Prof. Mea Wang

This directory contains source code of a "Hello World!" program, as well as
the system call trace of this program on Solaris, Linux, and Mac OS X.

To compile the program a executable with the name "hello":
> g++ hello.cpp -o hello

The file hello.strace contains the output of the following command in a Linux system.
> strace ./hello

The file hello.dtruss contains the output of the following command in a terminal on a Mac OS X system.
> dtruss ./hello

The file hello.truss contains the output of the following command in a Solaris system.
> truss ./hello

You may also get the system of a built-in program.  For example, to get the
system call trace of the "date" program, simple type the following command
in a Linux system.
> strace date
