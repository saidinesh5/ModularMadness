#include <iostream>

#include "commandlistener.h"

using namespace std;

int main()
{
    CommandListener commandListener;
    string input;

    while ( !cin.eof() )
    {
        getline( cin, input );
        if(!commandListener.processCommand( input ))
        {
            LOGME( "Error executing: "<< input << endl );
        }
    }
    return 0;
}

