#include <iostream>

#include "commandparser.h"

using namespace std;

int main()
{
    CommandParser commandParser;
    string input;

    while ( !cin.eof() )
    {
        getline( cin, input );
        if(!commandParser.parse( input ))
        {
            LOGME( "Error executing: "<< input << endl );
        }
    }
    return 0;
}

