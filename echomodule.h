#ifndef ECHOMODULE_H
#define ECHOMODULE_H

#include "module.h"

class EchoModule : public Module
{
    string m_output;

public:
    EchoModule();

    bool acceptInput( const string& input );
    const string output();
};

#endif // ECHOMODULE_H
