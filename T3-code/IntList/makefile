# make files make the compilation and link process easier

# define IntList and the files it requires to compile.  If IntList.o or
# main.o are not up to date they will be called upon to be updated before
# IntList is compiled together.  Note that tabs are required

IntList: IntList.o main.o
	g++ -o IntList IntList.o main.o

# to compile main.o the file main.cpp is checked for updates.  If main.cpp
# is not present an error will be presented.  If main.cpp has not changed
# since the last build it will not be recompiled.

main.o : main.cpp
	g++ -c main.cpp

IntList.o : IntList.cpp IntList.h
	g++ -c IntList.cpp
