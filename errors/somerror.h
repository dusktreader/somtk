#pragma once

#include <QString>
#include <exception>

namespace somtk {

class SOMError : public std::exception
{

private:

    QString _message;

public:

    SOMError( QString message ) :
            _message( message )
    {}

    ~SOMError() throw()
    {}

    virtual const char* what() const throw()
    {
        return _message.toUtf8();
    }

    QString message() const
    {
        return _message;
    }

    static inline void requireCondition( bool condition, QString message="Condition failed" )
    {
        if( condition == false )
            throw SOMError( message );
    }

};

} // namespace hsom
