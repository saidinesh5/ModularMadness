#ifndef REVERSEMODULE_H
#define REVERSEMODULE_H

#include "module.h"

class ReverseModule : public Module
{
    string m_output;
public:
    ReverseModule();

    bool acceptInput( const string& input );
    const string output();
};

#endif // REVERSEMODULE_H
