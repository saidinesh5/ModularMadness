#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "module.h"

#ifdef ENABLE_LOGGING
#define LOGME(args) cerr << args
#else
#define LOGME(args)
#endif

using namespace std;
typedef shared_ptr<Module> ModulePtr;
typedef size_t ModuleId;

/**
 * @brief The Engine class
 *
 *        The actual Engine class that does all the processing.
 *        Internally builds a graph of all the instantiated modules where,
 *        -> Instance of a Module is a Vertex
 *        -> Connection between two modules is an Edge
 *        and keeps track of the dependencies of each module using an adjacency list.
 */
class Engine
{

    // Map maintaining a mapping from module name ---> moduleId
    // We use integer ids to happily speed up the computations done internally
    map< string, ModuleId > m_moduleIds;

    enum IOModules
    {
        InputModuleId,
        OutputModuleId
    };

    // Internal list of module instances: can now be accessed as m_modules[moduleId]
    vector< ModulePtr > m_modules;
    vector< vector<ModuleId> > m_moduleDependencies;

    vector<bool> m_visited;
    vector<bool> m_needsAnotherTick;

public:
    /**
     * @brief Engine - The Constructor of the class.
     */
    Engine();

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

private:

    /**
     * @brief moduleName
     * @param id
     * @return returns the name of the module with the given id
     */
    const string moduleName( ModuleId id );

    /**
     * @brief isReachable
     *        Implements a Depth first search starting from source to destination
     * @param source Module to start the DFS from
     * @param destination Module to search for
     * @return true if there exists a path from source to destination
     */
    bool isReachable( ModuleId source, ModuleId destination );

    /**
     * @brief addDependancy - adds module2 as a dependancy to compute module1
     * @param module1
     * @param module2
     * @return false if the addition failed - like when a cycle is detected
     */
    bool addDependancy( const ModuleId module1, const ModuleId module2 );

    /**
     * @brief removeDependancy - removes module2 as a dependancy to compute module1
     * @param module1
     * @param module2
     * @return false if there is nothing to remove
     */
    bool removeDependancy( const ModuleId module1, const ModuleId module2 );

    /**
     * @brief setDependancy - reset dependancies of module1 and addDependancy(module1, module2)
     * @param module1
     * @param module2
     * @return false if something goes wrong
     */
    bool setDependancy( const ModuleId module1, const ModuleId module2 );

    /**
     * @brief collectInputs
     * @param id
     * @return the sum of the output()s of all the modules the given module depends on.
     */
    const string collectInputs( ModuleId id );

    /**
     * @brief addModule
     *        Creates and adds a module to the internal list of modules
     * @param name - name of the module to create
     * @param type - type of the module to create: ( delay, echo, noop, reverse )
     * @return true if module creation is successful
     */
    bool addModule( string name, string type );

    /**
     * @brief connectModules
     *        Adds the module1 as a dependancy of module2.
     *        FIXME: The connection won't be added if the connection creates a cycle in the graph.
     *
     * @param module1
     * @param module2
     * @return true - if connection is successful.
     */
    bool connectModules( string module1, string module2 );

    /**
     * @brief processData
     *        The magic method of the engine that does all of the processing.
     *        It initializes the input and output modules with the needed data and starts
     *        The event loop, which runs until:
     *        1) no module has any data to be processed anymore
     *        2) any module results in error while during a processing step.
     *        (for eg. When the Output module raises an error at acceptInput,
     *         if the outputData > 16*inputData , the engine stops execution)
     *
     * @param data - an array of strings to be processed by the modules.
     *        This will directly be fed into the input and output modules
     *
     * @return true, if processing is successful
     */
    bool processData( const vector<string>& data );

    /**
     * @brief tick
     *        This method implements a Depth First Search, starting from the given module, and searches for modules with unprocessed data.
     * @param id - The module id from where to start the depth first search. By default, we start search from the output module
     * @return false if the event loop has to stop, if any module cannot be processed OR no module has any unprocessed data
     */
    bool tick( ModuleId id = OutputModuleId );
};

#endif // ENGINE_H
