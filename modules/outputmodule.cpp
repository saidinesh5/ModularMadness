#include <iostream>
#include "outputmodule.h"

using namespace std;

OutputModule::OutputModule()
{
}

bool OutputModule::initialize(const vector<string> &args)
{
    m_input = args;
    m_output.clear();
    return true;
}

void OutputModule::finalize()
{
    cout << endl;
}

bool OutputModule::acceptInput(const string &data)
{
    // Output should be limited to 16 times the size of the input
    if( m_output.size() < 16* m_input.size() )
    {
        if(!m_output.empty())
            cout << " ";
        cout << data;
        m_output.push_back( data );
        return true;
    }

    return false;
}

const string OutputModule::output()
{
    string result = "";
    bool firstTime = true;
    for (auto& str : m_output)
    {
        if(!firstTime)
            result = result + " ";
        result = result  + str;
    }
    return result;
}

void OutputModule::reset()
{
    m_input.clear();
    m_output.clear();
}
