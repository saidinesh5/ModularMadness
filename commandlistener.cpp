#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>

#include "commandlistener.h"


CommandListener::CommandListener()
{

}

bool CommandListener::processCommand( const string &command )
{
    if( command.empty() )
    {
        LOGME( "Cannot process an empty command" << endl );
        return false;
    }

    // Tokenizes the given input line and calls the appropriate function based on the input
    stringstream stream(command);
    istream_iterator<string> it(stream);
    istream_iterator<string> end;
    vector<string> tokens(it,end);

    if( tokens[0] == "module" && tokens.size() == 3)
        return m_engine.addModule( tokens[1], tokens[2] );

    if( tokens[0] == "connect" && tokens.size() == 3)
        return m_engine.connectModules( tokens[1], tokens[2] );

    if( tokens[0] == "process" && tokens.size() > 1)
        return m_engine.processData( vector<string>( tokens.begin() + 1 , tokens.end() ) );

    return false;
}
