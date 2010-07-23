#pragma once

#include <deque>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <omp.h>
#include <cfloat>
#include <cmath>
#include <cstdlib>

#ifndef PI
#define PI 3.1415926535897931
#endif

#ifndef EPSILON
#define EPSILON 1e-10
#endif

/** OS specific flags and etcetera */
#ifdef WIN32
#define OS_SEP_STR "\\"
#define OS_SEP_CHAR '\\'
#define PRINT_FUNC __FUNCSIG__
#else
#define PRINT_FUNC __PRETTY_FUNCTION__
#define OS_SEP_STR "/"
#define OS_SEP_CHAR '/'
#endif

/** The DB_REP Macros are very convenient for debugging.
  * Each macro prints a message when the program reaches it in execution
  */

/** Prints the file and line */
#define DB_REP { std::cout << __FILE__ << ":" << __LINE__ << ": reporting" << std::endl; }

/** Prints the file name, line and a message
  * @param  msg - The message to print
  */
#define DB_REP_MSG( msg ) { std::cout << __FILE__ ":" << __LINE__ << ": message: " << (msg) << std::endl; }

/** Prints the file, line, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_VAR( var ) { std::ostringstream op; \
                            op << __FILE__ ":" << __LINE__ << ": "; \
                            printVar( var, #var, op ); \
                            std::cout << op.str() << std::flush; }

/** Prints the file, line, variable name, and variable value, and a message
  * @param  var - The variable to report
  */
#define DB_REP_MSGVAR( msg, var ) { std::ostringstream op; \
                                    op << __FILE__ ":" << __LINE__ << ": "; \
                                    op << (msg) << ": "; \
                                    printVar( var, #var, op ); \
                                    std::cout << op.str() << std::flush; }

/** Prints the file name, line and a message if a condition is true
  * @param  cnd - The condition to test
  * @param  msg - The message to print
  */
#define DB_REP_CND( cnd, msg ) if(cnd) { std::cout << __FILE__ ":" << __LINE__ << " : messsage:" << (msg) << std::endl; }

/** Prints the file, line, list name, and list values
  * @param  list - The list to print
  * @param  l    - The length of the list
  */
#define DB_REP_LIST( list, l ) { std::ostringstream op; \
                                 op << __FILE__ ":" << __LINE__ << " : " << std::flush; \
                                 printList( list, l, #list, op ); \
                                 std::cout << op.str() << std::flush; }

/** Prints the file, line, and thread# */
#define DB_REP_OMP _Pragma( "omp critical" ){ \
                   std::ostringstream op; \
                   op << __FILE__ ":" << __LINE__ << ": "; \
                   op << "thread #" << omp_get_thread_num() << " reporting"; \
                   std::cout << op.str() << std::endl; }

/** Prints the file, line, thread#, and a message
  * @param  msg - The message to pri
  */
#define DB_REP_OMP_MSG( msg ) _Pragma( "omp critical" ){ \
                              std::ostringstream op; \
                              op << __FILE__ ":" << __LINE__ << ":"; \
                              op << "thread #" << omp_get_thread_num() << ": message: " << msg; \
                              std::cout << op.str() << std::endl; }

/** Prints the file, line, thread#, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_OMP_VAR( var ) _Pragma( "omp critical" ){ \
                              std::ostringstream op; \
                              op << __FILE__ ":" << __LINE__ << ": "; \
                              op << "thread #" << omp_get_thread_num() << ": "; \
                              printVar( var, #var, op ); \
                              std::cout << op.str() << std::flush; }

/** Prints the file, line, thread#, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_OMP_LIST( list, l ) _Pragma( "omp critical" ){ \
                                   std::ostringstream op; \
                                   op << __FILE__ ":" << __LINE__ << ": " << std::flush; \
                                   op << "thread #" << omp_get_thread_num() << ": " << std::flush; \
                                   printList( list, l, #list, op ); \
                                   std::cout << op.str() << std::flush; }

/** A custom exception that is thrown when a local ASSERT fails */
class LocalAssert: public std::exception{
private:
    /** Describes the message set during throw */
    std::string _message;
    std::string _info;
public:
    /** Creates the exception with a default message */
    LocalAssert( std::string info = "Location unknown", std::string message = "Local Assertion Failed" ) : _message(message), _info(info){}
    /** Destructs the exception */
    ~LocalAssert() throw(){}
    /** Returns the message describing the cause of the exception */
    virtual const char* what() const throw(){ return _message.c_str(); }
    /** Returns the message describing the cause of the exception */
    std::string message(){ return _message; }
    /** Returns the location where the exception occurred */
    std::string where(){ return _info; }
};

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

/** Converts a number to a string
  * @param  number - The number to convert
  * @param  prec   - Floating point precision to use
  * @param  w      - Width of the string
  * @return A string representation of the number
  */
template <class T>
std::string num2str( T number, int prec=2, int w=0 )
{
    std::ostringstream ss;
    ss.setf( std::ios::showpoint | std::ios::fixed );
    ss.precision( prec );
    ss.fill( ' ' );
    ss.width( w );
    ss << number;
    return ss.str();
}

/** Converts a string to a number
  * @param  str    - The string to convert
  * @param  number - The storage for the number
  */
template <class T>
void str2num( std::string str, T &number )
{
    std::istringstream ss( str );
    ss >> number;
}

template <typename R, typename I>
R convert( const I& in )
{
    std::stringstream ss;
    ss << in;
    R ret();
    ASSERT_MSG( !( ss >> ret ).fail(), "Conversion failed" );
    return ret;
}

/** Prints a labeled variable to an output stream
  * @param  var   - The variable to print
  * @param  label - The label for the variable
  * @param  out   - The output stream
  */
template <class T>
void printVar( T var, std::string label="variable", std::ostream &out=std::cout )
{
    out << label << " = " << var << std::endl;
}

/** Prints a labeled list to an output stream.  Any object overloaded for the operator[] using linear access will work
  * @param  list       - The list to print
  * @param  l          - The number of items to print.  This could overrun the list!
  * @param  label      - The label for the list
  * @param  abbreviate - Will only print the first and last three elements if true.
  * @param  prec       - Floating point precision to use
  * @param  width      - Padding width for numeric types
  * @param  out        - The output stream
  */
template <class T>
void printList( T list, int l, std::string label="items", bool abbreviate=false, int prec=2, int width=0, bool reverse=false, std::ostream &out=std::cout )
{
    out << label << ": [ " << std::flush;
    int i0, i1, di;
    if( reverse )
    {
        i0 = l - 1;
        i1 = -1;
        di = -1;
    }
    else
    {
        i0 = 0;
        i1 = l;
        di = 1;
    }
    for( int i=i0; i!=i1; i+=di )
    {
        if( abbreviate && i == i0 + di*3 && l > 10 )
        {
            out << "... " << std::flush;
            i = i1 + -di * 4;
        }
        else
            out << std::setw( width ) <<
                   std::setfill( ' ' ) <<
                   std::fixed <<
                   std::setprecision( prec ) <<
                   list[i] << " " << std::flush;
    }
    out << " ]" << std::endl;
}

/** Prints a labeled matrix to an output stream.  The matrix is assumed to be allocated serially in 1 dimension
  * @param  matrix     - The matrix to print
  * @param  w          - The width of the matrix
  * @param  h          - The height of the matrix
  * @param  label      - The label for the matrix
  * @param  abbreviate - Will only print the first and last three rows/columns if true.
  * @param  prec       - Floating point precision to use
  * @param  width      - Padding width for numeric types
  * @param  out        - The output stream
  */
template <class T>
void printMatrix( T matrix, int w, int h, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout )
{
    out << label << std::endl
        << "------------" << std::endl;
    for( int i=0; i<h; i++ ){
        if( abbreviate && i == 3 && h > 10 ){
            out << "..." << std::endl;
            i = h - 4;
        }
        printList( matrix + i * w, w, "|", abbreviate, prec, width, out );
    }
}

/** Convenience class used for generating random integers, and reals with uniform and gaussian distributions within
  * ranges defined when the functions are called
  */
class RandMaster
{
private:

    /// The last random gaussian number to be generated
    double lastGauss;

    /// The seed value for the random generator.  Shared by all RandMasters in the current application
    static unsigned int seed;

public:

    /** Constructs a rand master */
    RandMaster()
    {
        #pragma omp critical
        {
            if( seed == 0 )
            {
                seed = time(0);
                srand( seed );
            }
        }
        lastGauss = DBL_MAX;
    }

    inline int random()
    {
        #pragma omp critical
        {
            return rand();
        }
    }

    /** Returns a random int with a uniform distribution in the specified range
      * @param  lo - The low end of the range
      * @param  hi - The high end of the range
      * @return The randomly genereated int
      */
    int randi( int lo=0, int hi=100 )
    {
        return lo + random() % ( hi - lo );
    }

    /** Returns a random float with a uniform distribution in the specified range
      * @param  lo - The low end of the range
      * @param  hi - The high end of the range
      * @return The randomly genereated float
      */
    inline float randf( float lo=0.0, float hi=1.0 )
    {
        return lo + ( (float)random() / RAND_MAX )  * ( hi - lo );
    }

    /** Returns a random double with a uniform distribution in the specified range
      * @param  lo - The low end of the range
      * @param  hi - The high end of the range
      * @return The randomly genereated double
      */
    inline double randd( double lo=0.0, double hi=1.0 )
    {
        return lo + ( (double)random() / RAND_MAX ) * ( hi - lo );
    }

    /** Generates a random double with a gaussian distribution
      * @param  mu - The center of the distribution
      * @param  sigma - The standard deviation of the distribution
      * @return The random value within the gaussian distribution
      * @note   This algorithm uses the Box-Mueller transformation method described in Numerical Recipes 3rd Ed p364
      */
    double randg( double mu=0.0, double sigma=1.0 )
    {
        double v1  = 0.0,
               v2  = 0.0,
               rsq = 0.0,
               fac = 0.0;
        if( lastGauss == DBL_MAX )
        {
            while( rsq >= 1.0 || rsq == 0.0 )
            {
                v1 = randd( -1.0, 1.0 );
                v2 = randd( -1.0, 1.0 );
                rsq = v1*v1 + v2*v2;
            }
            fac = sqrt( -2.0 * log( rsq ) / rsq );
            lastGauss = v1 * fac;
            fac *= v2;
        }
        else
        {
            fac = lastGauss;
            lastGauss = DBL_MAX;
        }
        return mu + sigma * fac;
    }
};

/** Utility class used to represent a numeric interval.  The intervals can be open or closed on either end independently
  * and various tests and numbers can be generated within this interval
  */
class Interval
{
private:

    /// The low end of the interval
    double _lo;

    /// The high end of the interval
    double _hi;

    /// Specifies whether the low end is open
    bool _loOpen;

    /// Specifies whether the high end is open
    bool _hiOpen;

    /// A random number generator for producing random values from this interval
    RandMaster R;

public:
    /** Constructs a fully open interval on the range ( 0, 1 ) */
    Interval() : _lo(0), _hi(1), _loOpen(true), _hiOpen(true){}

    /** Constructs a fully open interval on the range ( lo, hi )
      * @param  lo - The low end of the interval
      * @param  hi - The high end of the interval
      */
    Interval( double lo, double hi ) : _lo(lo), _hi(hi), _loOpen(true), _hiOpen(true){}

    /** Constructs an interval with specified openness
      * @param  lo     - The low end of the interval
      * @param  hi     - The high end of the interval
      * @param  loOpen - Specifies if the low end is open or closed
      * @param  hiOpen - Specifies if the high end is open or closed
      */
    Interval( double lo, double hi, bool loOpen, bool hiOpen ) : _lo(lo), _hi(hi), _loOpen(loOpen), _hiOpen(hiOpen){}

    /** Fetches the value of the low end of the interval */
    inline double lo()
    {
        return _lo;
    }

    /** Fetches the value of the high end of the interval */
    inline double hi()
    {
        return _hi;
    }

    /** Fetches the midpoint of the interval */
    inline double mid()
    {
        return _lo + ( _hi - _lo ) / 2.0;
    }

    /** Fetches the distance between the high and low end of the interval */
    inline double dist()
    {
        return _hi - _lo;
    }

    /** Determines if a given value lies within the interval */
    inline bool in( double val )
    {
        return ( _loOpen ? val > _lo : val >= _lo ) && ( _hiOpen ? val < _hi : val <= _hi );
    }

    /** Generates a random value with uniform distribution from within the interval */
    double rand()
    {
        return R.randd( _lo, _hi );
    }

    /** Generates a random value with gaussian distribution centered on the midpoint of the interval
      * @param  sigmaSF - Determines how many standard deviations lie between the mid and low point of the interval
      * @return A random value with gaussian distribution
      */
    double randg( double sigmaSF=3.0 )
    {
        double mu = ( _hi - _lo ) / 2.0;
        double sigma = _hi - _lo / ( sigmaSF * 2.0 );
        double r = _lo - 1.0;
        while( ! in(r) )
        {
            r = R.randg( mu, sigma );
        }
        return r;
    }

    /** Clamps a value within the interval */
    double clamp( double val )
    {
        double clampedVal = val;
        if( val <= _lo )
            clampedVal = _lo + _loOpen ? DBL_EPSILON : 0.0;
        else if( val >= _hi )
            clampedVal = _hi - _hiOpen ? DBL_EPSILON : 0.0;
        return clampedVal;
    }

    Interval operator+( double val )
    {
        return Interval( _lo + val, _hi + val, _loOpen, _hiOpen );
    }

    Interval operator-( double val )
    {
        return *this + -val;
    }

    void shfit( double val )
    {
        _lo += val;
        _hi += val;
    }

    std::vector<double> subdividers( int n )
    {
        std::vector<double> divs;
        double dv = dist() / n;
        for( double v=_lo; v<=_hi; v+=dv )
            divs.push_back( v );
        return divs;
    }

    std::vector<double> subdividers( double dv=1.0 )
    {
        std::vector<double> divs;
        for( double v=_lo; v<=_hi; v+=dv )
            divs.push_back( v );
        return divs;
    }

    //think about how the subintervals would be open or closed
//    std::vector<Interval> subdivide( int n )
//    {
//
//    }

    /** Returns a string representation of the interval */
    std::string str()
    {
        return std::string( _loOpen? "( " : "[ " ) +
               num2str(_lo) + ", " + num2str(_hi) +
               std::string( _hiOpen? " )" : " ]" );
    }

    /** Overloads the ostream operator for printing */
    friend std::ostream& operator<<( std::ostream& out, Interval& i )
    {
        return out << i.str();
    }
};

/** Converts a value with units in degrees to units of radians */
inline double toRadians( double degrees )
{
    return degrees * PI / 180.0;
}

/** Converts a value with units in radians to units of degrees */
inline double toDegrees( double radians )
{
    return radians * 180.0 / PI;
}

/** Converts a numerical character value to an int */
int char2Int( char a );

/** Performs a division and modulo operation
  * @param  dividend  - The dividend   ( Numerator )
  * @param  divisor   - The divisor    ( Denominator )
  * @param  quotient  - The quotient   ( Result for division )
  * @param  remainder - The remeainder ( Result for modulo )
  */
void divmod( int dividend, int divisor, int &quotient, int &remainder );

/** Provides a mathematically correct integer modulus function
  * @param  a - The dividend
  * @param  b - The divisor
  * @return The result of the modulus operation
  */
int mod( int a, int b );

/** Converts a set of coordinates for an n-dimensional cartesian space into a linear array index
  * @param  idx    - The store reference for the calculated index
  * @param  dims   - The dimension sizes of the cartesian n-space
  * @param  coords - The cartesian coordinates to convert
  */
void coords2index( int &idx, const std::vector<int> &dims, const std::vector<int> &coords );

/** Converts a linear array index into a set of coordiantes for an n-dimensional cartesian space
  * @param  idx    - The linear array index to convert
  * @param  dims   - The dimension sizes of the cartesian n-space
  * @param  coords - The cartesian coordinates to convert
  */
void index2coords( int idx, const std::vector<int> &dims, std::vector<int> &coords );
