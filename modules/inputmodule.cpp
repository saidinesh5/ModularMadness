#include "inputmodule.h"

InputModule::InputModule()
{
}

bool InputModule::initialize( const vector<string> &data )
{
    m_data = data;
    return true;
}

bool InputModule::hasUnprocessedData()
{
    return !m_data.empty();
}

bool InputModule::processData()
{
    m_output = m_data[0];
    m_data.erase( m_data.begin() );
    return true;
}

bool InputModule::acceptInput( const string &input )
{
    if( !input.empty() )
        m_data.push_back( input );

    return true;
}

const string InputModule::output()
{
    return m_output;
}
