#ifndef NOOPMODULE_H
#define NOOPMODULE_H

#include "module.h"

class NoopModule : public Module
{
    string m_data;
public:
    NoopModule();

    bool acceptInput( const string& input );
    const string output();
};

#endif // NOOPMODULE_H
