#include "module.h"

Module::Module()
{
}

Module::~Module()
{
}

bool Module::initialize( const vector<string> &args )
{
    M_UNUSED(args)
    return true;
}

void Module::finalize()
{
}

bool Module::hasUnprocessedData()
{
    return false;
}

bool Module::processData()
{
    return true;
}

void Module::reset()
{
}
