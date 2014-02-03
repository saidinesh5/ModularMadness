#include <iostream>

#include "engine.h"

using namespace std;

int main()
{
    Engine engine;
    string input;

    while ( !cin.eof() )
    {
        getline( cin, input );
        if(!engine.processCommand( input ))
        {
            LOGME( "Error executing: "<< input << endl );
        }
    }
    return 0;
}

