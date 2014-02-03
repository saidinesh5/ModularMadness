#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>

#define M_UNUSED(arg) (void)arg;

using namespace std;

/**
 * @brief The Module class
 *
 * This is the Abstract Base class for each functional module.
 * Every module which is subclassed from this needs to implement at least the following methods:
 *
 * bool acceptInput( const string& data);
 * const string output();
 *
 * The typical sequence of :
 *
 * 1) new Module();
 *
 * 2) (optionally) initialize( data );
 *
 * 3) if( module.hasUnprocessedData() );
 *    processData();
 *
 * 4) (optionally) acceptInput( input );
 *
 * 5) output();
 *
 * 6) finalize();
 *
 * 7) (optionally) reset();
 *
 */

class Module
{

public:
    Module();
    virtual ~Module();

    /**
     * @brief initialize
     *        Initialize the module state, with any constraints etc..
     *        for eg. the output module should be initialized with the input strings,
     *        so that it can apply the constraint that it would accept input only if the
     *        (number of outputstrings) < 16*(number of inputstrings)
     * @param args (similar to sys.argv), all the arguments to the module are passed in this
     * @return true if the data is accepted.
     */
    virtual bool initialize( const vector<string>& args );
    virtual void finalize();

    /**
     * @brief acceptInput
     *        Will be called whenever the module needs to be passed input,
     *        usually,the input output of another module, that this module depends upon
     * @param input - is the data to be processed
     * @return true if the data is accepted.
     */
    virtual bool acceptInput( const string& input ) = 0;

    /**
     * @brief output
     *        Will be called once per connection.
     *        This is usually fed into the input of the modules that depend on this module.
     * @return The output of the module (possibly after processing the data)
     */
    virtual const string output() = 0;

    /**
     * @brief hasUnprocessedData
     *        The engine uses this method to see if the module has any data to be processed
     * @return true, if this module needs one or more cycles to finish processing the data
     */
    virtual bool hasUnprocessedData();

    /**
     * @brief processData
     *        The engine calls this method if hasUnprocessedData returns true.
     *        Do any processing that is needed here.
     * @return false, to stop the engine's execution if any error has occured while processing the data.
     */
    virtual bool processData();

    /**
     * @brief reset
     *        called to reset the internal state of the module
     */
    virtual void reset();
};

#endif // MODULE_H
