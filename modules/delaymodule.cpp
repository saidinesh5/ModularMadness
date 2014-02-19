#include "delaymodule.h"

DelayModule::DelayModule():
    Module()
{
    m_data.push("hello");
}

bool DelayModule::hasUnprocessedData()
{
    return !m_data.empty();
}

bool DelayModule::processData()
{
    m_output = m_data.front();
    m_data.pop();
    return true;
}

bool DelayModule::acceptInput( const string &input )
{
    m_data.push( input );
    return true;
}

const string DelayModule::output()
{
    return m_output;
}
