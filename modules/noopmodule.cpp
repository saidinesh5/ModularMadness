#include "noopmodule.h"

NoopModule::NoopModule()
{
}

bool NoopModule::acceptInput(const string& input)
{
    m_data = input;
    return true;
}

const string NoopModule::output()
{
    return m_data;
}
