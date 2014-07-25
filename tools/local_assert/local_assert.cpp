#include "tools/local_assert/local_assert.h"

using namespace std;

/* Methods for the LocalAssert Class */
LocalAssert::Status LocalAssert::status = LocalAssert::NoError;

LocalAssert::LocalAssert( string info, string message )
    : _message(message), _info(info)
{
    /*
    void* btBuff[1000];
    int btSize;
    char** btStrings;

    btSize = backtrace( btBuff, 1000 );
    btStrings = backtrace_symbols( btBuff, btSize );

    _trace = "";
    for( int i = 0; i < btSize; i++ )
        _trace.append( btStrings[i] ).append( "\n" );

    free( btStrings );
    */


    status = Error;
}

LocalAssert::~LocalAssert() throw()
{}

const char* LocalAssert::what() const throw()
{
    return _message.c_str();
}

string LocalAssert::message()
{
    return _message;
}

string LocalAssert::where()
{
    return _info;
}

string LocalAssert::trace()
{
    return _trace;
}

string LocalAssert::details()
{
    string msg;
    msg += "============\n";
    msg += "LocalAssert:\n";
    msg += "------------\n";
    msg += message() + "\n";
    msg += "------------\n";
    msg += where() + "\n";
    msg += "------------\n";
    msg += trace() + "\n";
    msg += "============\n";
    return msg;
}

LocalAssert::Status LocalAssert::fetchStatus()
{
    Status s = status;
    status = NoError;
    return s;
}

void LocalAssert::resetStatus()
{
    status = NoError;
}
