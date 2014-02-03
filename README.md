ModularMadness
==============

Yet another interview question.


build:
------

    mkdir build
    cd build
    cmake ..
    make

Notes:
------

The Program comprises of two parts:
1) Engine
The actual Engine that does all the processing.
Internally it builds a graph of all the instantiated modules where,

  o) Instance of a Module is a Vertex
  o) Connection between two modules is an Edge.
  o) The dependencies between modules are keept track of with an adjacency list.

The engine currently supports the following commands to build and manipulate the module graph:

  a) "module <name> <type>"
  b) "connect <module1> <module2>"
  c) "process <string1> <string2> <string3> ..."

For the engine, everything is a module, including input and output.
 
 2) Modules
 All the modules are derived from the base class Module. Each module must at least implement Module::acceptInput(const string&) and Module::output()
 Each module does one specific task it is designed for. Currently available modules are:
 
   InputModule   -> Outputs the input strings one at a time. hasUnprocessedData() returns true until all the data is consumed.
   OutputModule  -> Outputs the given input to console. If the number of output strings >= number of input strings, it won't accept any more input.
   DelayModule   -> Outputs the previous input string. The first output string is "hello".
   EchoModule    -> Outputs the input string concatinated with itself.
   NoopModule    -> Outputs the input string as is.
   ReverseModule -> Outputs the input string reversed.

Limitations/Assumptions:
------------------------

  1) Currently the program only works with inputs,which are Directed Acyclic Graphs. 
   i.e There should be no cycles/circular dependencies between the modules.
   One possible way to fix this is when trying to add a connection between two modules,
   if a cycle is detected, then the connection could be made to a previous cached value.
   
  2) The code needs a C++11 compatible compiler, for the usage of shared pointers, auto, range based for loops.