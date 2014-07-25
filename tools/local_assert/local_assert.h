#pragma once

#include <exception>
#include <string>

#if !defined(WIN32) || defined(__MINGW32__)
#define PRINT_FUNC __PRETTY_FUNCTION__
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#else
#define OS_SEP_STR "\\"
#define OS_SEP_CHAR '\\'
#define PRINT_FUNC __FUNCSIG__
#endif

/// A custom exception that is thrown when a local ASSERT fails
class LocalAssert: public std::exception
{
public:

    /// Defines the status of an assertion
    enum Status{
        Error,  ///< The assertion failed
        NoError ///< The assertion passed
        };

    /** Creates the exception with a default message */
    LocalAssert( std::string info = "Location unknown", std::string message = "Local Assertion Failed" );

    /** Destructs the exception */
    ~LocalAssert() throw();

    /** Returns the message describing the cause of the exception */
    virtual const char* what() const throw();

    /** Returns the message describing the cause of the exception */
    std::string message();

    /** Returns the location where the exception occurred */
    std::string where();

    /** Returns a backtrace of execution before the exception occurred */
    std::string trace();

    /** Returns a string describing the error in detail */
    std::string details();

    /** Fetches and resets the current status of LocalAssert */
    static LocalAssert::Status fetchStatus();

    /** Resets the state of the status flag */
    static void resetStatus();

private:
    /** Describes the message set during throw */
    std::string _message;
    std::string _info;
    std::string _trace;

    static LocalAssert::Status status;
};


/// Verifies that a condition is true, otherwise it throws an exception with a descriptive message
/// @todo: Consider adding c++0x features such as default template arguments
inline void localAssert( bool condition, std::string message="Required condition failed" )
{
    /// @note  This is simply a utility function that adds a great deal of readability ( the author's opinion )
    if( !condition )
        throw LocalAssert( "Location unknown", message );
}



/** Improves the assert() idiom by allowing it to be caught in a try block */
#define ASSERT( expr )                                                \
    if( ( expr ) == false )                                           \
        throw LocalAssert( std::string("in function ") + PRINT_FUNC + \
                           "  of file " + __FILE__ +                  \
                           " at line " + num2str(__LINE__) );

/** Allows an assertion with a failure message */
#define ASSERT_MSG( expr, msg )                                       \
    if( ( expr ) == false )                                           \
        throw LocalAssert( std::string("in function ") + PRINT_FUNC + \
                           "  of file " + __FILE__ +                  \
                           " at line " + num2str(__LINE__),           \
                           std::string( msg ) );

/** Sets a local variable to a value contingent on a logical expression */
#define ASSERT_SET( var, val, expr, msg ) \
    { ASSERT_MSG( expr, msg ); \
      var = val; } \

/** Breaks execution at this point */
#define ABORT \
    throw LocalAssert( std::string(" in function ") + PRINT_FUNC + \
                       "  of file " + __FILE__ +                   \
                       " at line " + num2str(__LINE__),            \
                      "Aborted" );

/** Breaks execution at this point */
#define ABORT_MSG( msg ) \
    throw LocalAssert( std::string(" in function ") + PRINT_FUNC + \
                       "  of file " + __FILE__ +                   \
                       " at line " + num2str(__LINE__),            \
                       std::string( msg ) );

/** Catches a Local Assert and prints details to stdout         */
#define CATCH_LASS                                                            \
    catch( LocalAssert& err )                                                 \
    {                                                                         \
        cout << "Warning:  Local Assert failed:" << endl;                     \
        cout << "    " << err.what() << endl;                                 \
        cout << "    " << err.where() << endl;                                \
    }                                                                         \
    catch( ... )                                                              \
    {                                                                         \
        cout << "Warning:  Unknown Exeption occured: Re-trhowing..." << endl; \
        throw;                                                                \
    }
