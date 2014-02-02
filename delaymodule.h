#ifndef DELAYMODULE_H
#define DELAYMODULE_H

#include <queue>
#include "module.h"

class DelayModule : public Module
{
    queue<string> m_data;
    string m_output;

public:
    DelayModule();

    bool hasUnprocessedData();
    bool processData();

    bool acceptInput( const string &input );
    const string output();

};

#endif // DELAYMODULE_H
