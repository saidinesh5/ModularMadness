#include "reversemodule.h"

ReverseModule::ReverseModule()
{
}

bool ReverseModule::acceptInput(const string& input)
{
    m_output =  string(input.rbegin(), input.rend());
    return true;
}

const string ReverseModule::output()
{
    return m_output;
}
