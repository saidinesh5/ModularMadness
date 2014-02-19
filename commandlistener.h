#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H

#include <string>
#include "engine.h"

/**
 * @brief The CommandListener class
 *
 *        This class processes the input commands one line at a time via. processCommand()
 *        and executes them using an object of the Engine class, if it is a valid command.
 */
class CommandListener
{
    Engine m_engine;

public:
    /**
     * @brief processCommand
     *        The only public method of the class. Accepts a command and does the internal processing
     *
     * @param command - The command string to be processed. Currently Accepted commands are:
     *        1) "module <name> <type>"
     *        2) "connect <module1> <module2>"
     *        3) "process <string1> <string2> <string3> ..."
     *
     * @return true, if the processing is successful.
     */
     bool processCommand( const string& command );
};

#endif // COMMANDLISTENER_H
