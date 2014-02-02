#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "module.h"

#define INPUT_MODULE_ID 0
#define OUTPUT_MODULE_ID 1

using namespace std;
typedef shared_ptr<Module> ModulePtr;
typedef unsigned long ModuleId;

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

    // Internal list of module instances: can now be accessed as m_modules[moduleId]
    vector< ModulePtr > m_modules;
    vector< vector<ModuleId> > m_moduleDependencies;

    vector< ModuleId > m_moduleTraversalPath;

    // Flag used to indicate that the graph has changed, and the traversal path needs to be recomputed
    bool m_rebuildModuleTraversalPath;

public:
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
     * @brief addDependancy - adds module2 as a dependancy to compute module1
     * @param module1
     * @param module2
     * @return false if the addition failed - like when a cycle is detected
     */
    bool addDependancy( const unsigned long module1, const unsigned long module2 );

    /**
     * @brief removeDependancy - removes module2 as a dependancy to compute module1
     * @param module1
     * @param module2
     * @return false if there is nothing to remove
     */
    bool removeDependancy( const unsigned long module1, const unsigned long module2 );

    /**
     * @brief setDependancy - reset dependancies of module1 and addDependancy(module1, module2)
     * @param module1
     * @param module2
     * @return false if something goes wrong
     */
    bool setDependancy( const unsigned long module1, const unsigned long module2 );

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
     * @brief rebuildModuleTraversalPath
     *        Topologically sorts the graph of modules, starting at the given module
     *        and populates the m_moduleTraversalOrder
     * @param start
     *        starting point for the graph traversal. If no point is specified,
     *        we start at the output module.
     * @return true if the sorting succeeds
     */
    bool rebuildModuleTraversalPath( ModuleId start = OUTPUT_MODULE_ID );

};

#endif // ENGINE_H
