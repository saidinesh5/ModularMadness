#ifndef OUTPUTMODULE_H
#define OUTPUTMODULE_H

#include "module.h"

class OutputModule : public Module
{
    vector<string> m_input;
    vector<string> m_output;

public:
    OutputModule();

    bool initialize( const vector<string> &args );
    void finalize();

    bool acceptInput( const string &data );
    const string  output();

    void reset();
};

#endif // OUTPUTMODULE_H
