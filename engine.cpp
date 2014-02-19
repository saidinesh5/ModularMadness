#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>

#include "engine.h"

#include "inputmodule.h"
#include "outputmodule.h"

#include "delaymodule.h"
#include "echomodule.h"
#include "noopmodule.h"
#include "reversemodule.h"

using namespace std;

Engine::Engine()
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
        LOGME( "Cannot process an empty command" << endl );
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


const string Engine::moduleName( ModuleId id )
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

bool Engine::isReachable( ModuleId source, ModuleId destination )
{
    //Implements a simple depth first search starting from source
    vector< bool > visited( m_modules.size(), false );
    visited[source] = true;

    vector< ModuleId > stack;
    stack.push_back( source );

    while( !stack.empty() )
    {
        auto currentModuleId = stack.back();
        auto adjacentNodes = m_moduleDependencies[currentModuleId];

        if( currentModuleId == destination )
            return true;

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

    return false;
}

bool Engine::addDependancy( const ModuleId module1, const ModuleId module2 )
{
    // Adds module2 to the list of modules that module1 depends on for calculation
    if( !( module1 < m_modules.size() && module2 < m_modules.size() ) )
        return false;

    // Check if the module already exists as a dependency
    for( auto d : m_moduleDependencies[module1] )
        if( d == module2 )
            return false;

    // Check if the module2 is reachable from module1 to prevent formation of cycles.
    if( isReachable( module2, module1 ) )
        return false;

    m_moduleDependencies[module1].push_back( module2 );
    return true;
}


bool Engine::removeDependancy( const ModuleId module1, const ModuleId module2 )
{
    // Removes module2 from the modules that module1 depends on
    if( !( module1 < m_moduleDependencies.size() && module2 < m_moduleDependencies.size() ) )
    {
        LOGME( " Error while trying to add dependency: " << module1 << " ---> " << module2 << endl );
        LOGME( " Number of modules = " << m_moduleDependencies.size() << endl );
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


bool Engine::setDependancy( const ModuleId module1, const ModuleId module2 )
{
    // Makes module2 the only dependency of module1
    if( !( module1 < m_moduleDependencies.size() && module2 < m_moduleDependencies.size() ) )
    {
        LOGME( " Error while trying to set dependency: " << module1 << " ---> " << module2 << endl );
        LOGME( " Number of modules = " << m_moduleDependencies.size() << endl );
        return false;
    }

    m_moduleDependencies[module1].clear();
    return addDependancy(module1, module2);
}


const string Engine::collectInputs( ModuleId id )
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
        LOGME( "Error while trying to add module: " << name << "(" << type << ")" << endl );
        LOGME( "modules[" << name << "] = " << m_moduleIds[name] << endl );
        return false;
    }

    if( type == "delay" || type == "echo" || type == "noop" || type == "reverse" )
    {
        ModuleId moduleId = m_modules.size();
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
        return true;
    }

    return false;
}


bool Engine::connectModules( string module1, string module2 )
{
    // Connect modules only if they have been defined in the first place
    if( m_moduleIds.count( module1 ) > 0 && m_moduleIds.count( module2 ) > 0 )
        return addDependancy( m_moduleIds[module2], m_moduleIds[module1] );

    return false;
}

bool Engine::processData( const vector<string>& data )
{
    if( data.empty() )
    {
        LOGME( "No data to process" << endl );
        return true;
    }

    m_modules[INPUT_MODULE_ID]->initialize( data );
    m_modules[OUTPUT_MODULE_ID]->initialize( data );

    // Assume that all modules have something to say
    m_needsAnotherTick = vector<bool>( m_modules.size(), true );

    bool needsAnotherTick = true;
    while( needsAnotherTick )
    {
        // Get the flags ready for recurssion
        m_visited = vector<bool>( m_modules.size(), false );
        needsAnotherTick = tick( OUTPUT_MODULE_ID );
    }

    // Clean up after the event loop
    for(auto module : m_modules)
        module->finalize();

    return true;
}

bool Engine::tick( ModuleId id )
{
    bool needsAnotherTick = false;
    auto adjacentNodes = m_moduleDependencies[id];

    for( auto nextModuleId : adjacentNodes )
    {
        // First process all it's dependencies, if they need processing.
        // That way the data flows from their dependents to them.
        if( !m_visited[nextModuleId] )
        {
            m_visited[nextModuleId] = true;

            // This module needs another tick if it's dependencies need another nick
            if( m_needsAnotherTick[nextModuleId] )
                needsAnotherTick = needsAnotherTick || tick( nextModuleId );

            // Then processData() data on them, so if they have any unprocessed data from previous step,
            // it shall be processed.
            if( m_modules[nextModuleId]->hasUnprocessedData() )
            {
                if( !m_modules[nextModuleId]->processData() )
                    return false;
            }

            // This module needs another tick also if it's dependancies have unprocessed data
            needsAnotherTick = needsAnotherTick || m_modules[nextModuleId]->hasUnprocessedData();
        }
    }

    if( !m_moduleDependencies[id].empty() )
    {
        const string input = collectInputs( id );
        if( !m_modules[id]->acceptInput( input ) )
        {
            return false;
        }
    }

    // A module needs another "tick()", only if any of the modules that it depends on needs another processData()
    m_needsAnotherTick[id] = needsAnotherTick;

    return m_needsAnotherTick[id];
}
