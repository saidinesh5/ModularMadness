#include "echomodule.h"

EchoModule::EchoModule():
    Module()
{
}

bool EchoModule::acceptInput(const string& input)
{
    m_output = input + input;
    return true;
}

const string EchoModule::output()
{
    return m_output;
}
