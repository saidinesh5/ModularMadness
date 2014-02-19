#ifndef INPUTMODULE_H
#define INPUTMODULE_H

#include <vector>

#include "../module.h"

class InputModule : public Module
{

    vector<string> m_data;
    string m_output;

public:
    InputModule();

    bool initialize( const vector<string>& input );

    bool hasUnprocessedData();
    bool processData();
    bool acceptInput( const string &input );
    const string output();
};

#endif // INPUTMODULE_H
