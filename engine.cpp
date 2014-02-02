#include <iostream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <sstream>

#include "engine.h"

#include "inputmodule.h"
#include "outputmodule.h"

#include "delaymodule.h"
#include "echomodule.h"
#include "noopmodule.h"
#include "reversemodule.h"

using namespace std;

Engine::Engine():
    m_rebuildModuleTraversalPath(true)
{
    // Modules 0 and 1 are the input and output modules. #TheClassicUnixWay :D
    m_moduleIds["input"] = m_modules.size();
    m_modules.push_back( ModulePtr( new InputModule() ) );
    m_moduleDependencies.push_back( vector<ModuleId>() );

    m_moduleIds["output"] = m_modules.size();
    m_modules.push_back( ModulePtr( new OutputModule() ) );
    m_moduleDependencies.push_back( vector<ModuleId>() );
}

bool Engine::processCommand( const string &command )
{
    if( command.empty() )
    {
        cerr << "Cannot process an empty command" << endl;
        return false;
    }

    // Tokenizes the given input line and calls the appropriate function based on the input
    stringstream stream(command);
    istream_iterator<string> it(stream);
    istream_iterator<string> end;
    vector<string> tokens(it,end);

    if( tokens[0] == "module" && tokens.size() == 3)
        return addModule( tokens[1], tokens[2] );

    if( tokens[0] == "connect" && tokens.size() == 3)
        return connectModules( tokens[1], tokens[2] );

    if( tokens[0] == "process" && tokens.size() > 1)
        return processData( vector<string>( tokens.begin() + 1 , tokens.end() ) );

    return false;
}


const string Engine::moduleName(ModuleId id)
{
    if( id >= m_moduleIds.size() )
        return "-";

    for( auto d: m_moduleIds )
    {
        if( d.second == id )
            return d.first;
    }

    return "-";
}


bool Engine::addDependancy( const unsigned long module1, const unsigned long module2 )
{
    // Adds module2 to the list of modules that module1 depends on for calculation
    if( !( module1 < m_modules.size() && module2 < m_modules.size() ) )
        return false;

    // Check if the module already exists as a dependency
    for( auto d : m_moduleDependencies[module1] )
        if( d == module2 )
            return false;

    // Check if the module2 is reachable from module1
    vector<bool> visited( m_modules.size(), false );
    ModuleId start = module2;
    ModuleId end = module1;

    vector< ModuleId > stack { start };
    visited[start] = true;

    while( !stack.empty() )
    {
        auto currentModuleId = stack.back();
        auto adjacentNodes = m_moduleDependencies[currentModuleId];

        if( currentModuleId == end )
            return false;

        stack.pop_back();

        for( auto nextModuleId : adjacentNodes )
        {

            if( !visited[nextModuleId] )
            {
                visited[nextModuleId] = true;
                stack.push_back( nextModuleId );
            }
        }
    }


    m_moduleDependencies[module1].push_back( module2 );
    return true;
}


bool Engine::removeDependancy( const unsigned long module1, const unsigned long module2 )
{
    // Removes module2 from the modules that module1 depends on
    if( !( module1 < m_moduleDependencies.size() && module2 < m_moduleDependencies.size() ) )
    {
        cerr << " Error while trying to add dependency: " << module1 << " ---> " << module2 << endl;
        cerr << " Number of modules = " << m_moduleDependencies.size() << endl;
        return false;
    }

    for( auto d = m_moduleDependencies[module1].begin(); d != m_moduleDependencies[module1].end(); d++ )
    {
        if( *d == module2 )
        {
            m_moduleDependencies[module1].erase( d );
            return true;
        }
    }

    return false;
}


bool Engine::setDependancy( const unsigned long module1, const unsigned long module2 )
{
    // Makes module2 the only dependency of module1
    if( !( module1 < m_moduleDependencies.size() && module2 < m_moduleDependencies.size() ) )
    {
        cerr << " Error while trying to set dependency: " << module1 << " ---> " << module2 << endl;
        cerr << " Number of modules = " << m_moduleDependencies.size() << endl;
        return false;
    }

    m_moduleDependencies[module1].clear();
    return addDependancy(module1, module2);
}

const string Engine::collectInputs(ModuleId id)
{
    string result = "";

    if( id >= m_modules.size() )
    {
        return result;
    }

    for ( auto m : m_moduleDependencies[id] )
    {
        result += m_modules[m]->output();
    }

    return result;
}


bool Engine::addModule( string name, string type )
{
    // adds a new module of the given type to the list of modules
    if( m_moduleIds.count(name) > 0 )
    {
        cerr << "Error while trying to add module: " << name << "(" << type << ")" << endl;
        cerr << "modules[" << name << "] = " << m_moduleIds[name] << endl;
        return false;
    }

    if( type == "delay" || type == "echo" || type == "noop" || type == "reverse" )
    {
        int moduleId = m_modules.size();
        m_moduleIds[name] = moduleId;
        if(type == "delay")
            m_modules.push_back( ModulePtr( new DelayModule() ) );
        if(type == "echo" )
            m_modules.push_back( ModulePtr( new EchoModule() ) );
        if( type == "noop" )
            m_modules.push_back( ModulePtr( new NoopModule() ) );
        if( type == "reverse" )
            m_modules.push_back( ModulePtr( new ReverseModule() ) );

        m_moduleDependencies.push_back( vector<ModuleId>() );
        // If this is the first module, make it get the input from the input module
        if( m_modules.size() == 3 )
            setDependancy( moduleId, INPUT_MODULE_ID );

        // Set the current module as the output module
        setDependancy( OUTPUT_MODULE_ID, moduleId );
        m_rebuildModuleTraversalPath = true;
        return true;
    }

    return false;
}

bool Engine::connectModules( string module1, string module2 )
{
    // Connect modules only if they have been defined in the first place
    if( m_moduleIds.count( module1 ) > 0 && m_moduleIds.count( module2 ) > 0 )
    {
        m_rebuildModuleTraversalPath = true;
        return addDependancy( m_moduleIds[module2], m_moduleIds[module1] );
    }

    return false;
}

bool Engine::processData( const vector<string>& data )
{
    if( data.empty() )
    {
        cerr << "No data to process" << endl;
        return true;
    }

    m_modules[INPUT_MODULE_ID]->initialize( data );
    m_modules[OUTPUT_MODULE_ID]->initialize( data );

    if( m_rebuildModuleTraversalPath )
    {
        rebuildModuleTraversalPath( OUTPUT_MODULE_ID );
        m_rebuildModuleTraversalPath = false;
        if( m_moduleTraversalPath.size() <= 1 )
        {
            cerr << "Nothing to do. Add a few modules and then call me back";
            return true;
        }
    }

    // The input module always has data to process
    long firstUnprocessedNode = m_moduleTraversalPath.front();
    const vector<ModuleId> &path = m_moduleTraversalPath;
    bool processingErrors = false;

    while( firstUnprocessedNode >= 0 && !processingErrors )
    {
        for( unsigned long i = firstUnprocessedNode; i < path.size(); i++ )
        {
            ModuleId moduleId = path[i];

            //Process the first unprocessed node
            if( m_modules[moduleId]->hasUnprocessedData() )
                m_modules[moduleId]->processData();

            ModuleId nextModuleId = path[i + 1];
            if(nextModuleId >= m_modules.size())
                break;

            // And make it's dependent collect the data
            if( m_moduleDependencies[nextModuleId].size() > 0)
            {
                string nextInput = collectInputs(nextModuleId);
                if(!m_modules[nextModuleId]->acceptInput(nextInput))
                {
                    cerr << moduleName(nextModuleId) << " module did not accept input: "<< nextInput << endl;
                    processingErrors = true;
                    break;
                }
            }
        }

        // Find out which node next to process
        firstUnprocessedNode = -1;
        for ( unsigned long i = 0; i < path.size(); i++ )
        {
            ModuleId moduleId = path[i];
            if( m_modules[moduleId]->hasUnprocessedData() )
            {
                firstUnprocessedNode = i;
                break;
            }
        }
    }

    // Clean up after the event loop
    for(auto moduleId : m_moduleTraversalPath)
        m_modules[moduleId]->finalize();

    return true;
}

bool Engine::rebuildModuleTraversalPath( ModuleId start )
{
    m_moduleTraversalPath.clear();
    vector<bool> visited( m_modules.size(), false );

    vector< ModuleId > stack { start };
    visited[start] = true;

    while( !stack.empty() )
    {
        auto currentModuleId = stack.back();
        auto adjacentNodes = m_moduleDependencies[currentModuleId];

        m_moduleTraversalPath.push_back( currentModuleId );
        stack.pop_back();

        for( auto nextModuleId : adjacentNodes )
        {
            if( !visited[nextModuleId] )
            {
                visited[nextModuleId] = true;
                stack.push_back( nextModuleId );
            }
        }
    }

    reverse( m_moduleTraversalPath.begin(), m_moduleTraversalPath.end() );

//    cout << "Rebuilt traversal path: " << endl;
//    for( auto moduleId: m_moduleTraversalPath )
//    {
//        cout << moduleName(moduleId) << ",";
//    }
//    cout << endl;

    return true;
}
