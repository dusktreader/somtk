#pragma once

#include <QString>
#include <exception>

namespace hsom {

class SOMError : public std::exception
{

private:

    QString _message;

public:

    SOMError( const QString& message ) :
            _message( message )
    {}

    ~SOMError() throw()
    {}

    virtual const char* what() const throw()
    {
        return _message.toAscii();
    }

    const QString& message() const
    {
        return _message;
    }

    static void requireCondition( bool condition, const QString& message="Condition failed" )
    {
        if( condition == false )
            throw SOMError( message );
    }

};

} // namespace hsom
