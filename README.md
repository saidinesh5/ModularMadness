ModularMadness
==============

Yet another interview question.


build:
------
    make
    ./ModularMadness

Notes:
------

The Program comprises of three parts:

  1) CommandListener
  -------------------
  This is the command parser class which parses the input one line at a time
  And executes it using an instance of the Engine class.

  The CommandListener currently supports the following commands:

    a) "module <name> <type>"

    b) "connect <module1> <module2>"

    c) "process <string1> <string2> <string3> ..."

  2) Engine
  ----------
  The actual Engine that does all the processing.
  Internally it builds a graph of all the instantiated modules where,

    o) Instance of a Module is a Vertex

    o) Connection between two modules is an Edge.

    o) The dependencies between modules are keept track of with an adjacency list.


  The engine currently supports the following commands to build and manipulate the module graph:

    o) addModule( string name, string type )

    o) connectModules( string module1, string module2 )

    o) processData( const vector<string>& data )


  For the engine, everything is a module, including input and output.

 3) Modules
 ----------
  All the modules are derived from the base class Module. Each module must at least implement Module::acceptInput(const string&) and Module::output().
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
