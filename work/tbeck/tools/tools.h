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
#include <cmath>

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


/** The EXEC_TRACE Macros allow the user to trace execution through output
  * @example
  *   #define EXEC_TRACE
  *   int my_func(){
  *     BEGIN_TRACE;
  *     ENTER;
  *     ...
  *     RETURN my_int;
  *   }
  * @note When using the EXEC_TRACE idiom, be sure that RETURNS that are dependant on conditional clauses are
  *       encompassed within a block.  Otherwise undefined behavior will occur.  The curly brackets are required for
  *       proper functionality.  EXEC_TRACE is not thread safe, so do not use it in a function called from within an
  *       omp parallel block.
  *         if( test_val ){
  *             RETURN ret_val;
  *         }
  */
//#define EXEC_TRACE
#ifdef EXEC_TRACE
extern std::deque<std::string> exec_stack;
#define ENTER  { exec_stack.push_back( std::string( __FILE__ ) + ": " + num2str( __LINE__ ) + ": " + PRINT_FUNC ); }
#define RETURN { exec_stack.pop_back(); } return
#define TRACE( code ) try{ code }                           \
                      catch( exception e ){                 \
                          int depth = 0;                    \
                          string func;                      \
                          while( !exec_stack.empty() ){     \
                              func = exec_stack.front();    \
                              depth++;                      \
                              exec_stack.pop_front();       \
                              std::cout << "|" << std::flush;         \
                              for( int d=0; d<depth; d++ )  \
                                  std::cout << "--" << std::flush;    \
                              std::cout << "> " << func << std::endl; \
                          }                                 \
                          std::cout << std::endl << e.what() << std::endl; \
                          exit(1);                          \
                      }
/*
extern int callDepth;
#define BEGIN_TRACE { callDepth = 0; }
#define ENTER  if( callDepth >= 0 ){ \
               callDepth++; \
               std::cout << "|" << std::flush; \
               for( int depth=0; depth<callDepth; depth++ ) \
                   std::cout << "==" << std::flush; \
               std::cout << "> " << PRINT_FUNC << std::endl; }
#define RETURN if( callDepth >= 0 ){ \
               std::cout << "|<" << std::flush; \
               for( int depth=0; depth<callDepth; depth++ ) \
                   std::cout << "==" << std::flush; \
               std::cout << " " << PRINT_FUNC << std::endl; \
               callDepth--; } \
               return
*/
#else
#define ENTER {}
#define RETURN return
#define TRACE( code ) code
#endif

/** The DB_REP Macros are very convenient for debugging.
  * Each macro prints a message when the program reaches it in execution
  */

/** Prints the file and line */
#define DB_REP                 { std::cout << __FILE__ << ":" << __LINE__ << ": reporting" << std::endl; }

/** Prints the file name, line and a message
  * @param  msg - The message to print
  */
#define DB_REP_MSG( msg )      { std::cout << __FILE__ ":" << __LINE__ << ": message: " << (msg) << std::endl; }

/** Prints the file, line, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_VAR( var )      { std::ostringstream op; \
                                 op << __FILE__ ":" << __LINE__ << " :"; \
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
                                 op << __FILE__ ":" << __LINE__ << " : "; \
                                 printList( list, l, #list, op ); \
                                 std::cout << op.str() << std::flush; }

/** Prints the file, line, and thread# */
#define DB_REP_OMP               _Pragma( "omp critical" ){ \
                                 std::ostringstream op; \
                                 op << __FILE__ ":" << __LINE__ << ": "; \
                                 op << "thread #" << omp_get_thread_num() << " reporting"; \
                                 std::cout << op.str() << std::endl; }

/** Prints the file, line, thread#, and a message
  * @param  msg - The message to print
  */
#define DB_REP_OMP_MSG( msg )    _Pragma( "omp critical" ){ \
                                 std::ostringstream op; \
                                 op << __FILE__ ":" << __LINE__ << ":"; \
                                 op << "thread #" << omp_get_thread_num() << ": message: " << msg; \
                                 std::cout << op.str() << std::endl; }

/** Prints the file, line, thread#, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_OMP_VAR( var )    _Pragma( "omp critical" ){ \
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
                                   op << __FILE__ ":" << __LINE__ << ": "; \
                                   op << "thread #" << omp_get_thread_num() << ": "; \
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
    std::string message(){ return _message; }
    std::string where(){ return _info; }
};

/** Improves the assert() idiom by allowing it to be caught in a try block */
#define ASSERT( expr ) \
    if( ( expr ) == false ) \
        throw LocalAssert( std::string("in function ") + PRINT_FUNC + "  of file " + __FILE__ + " at line " + num2str(__LINE__) );

/** Allows an assertion with a failure message */
#define ASSERT_MSG( expr, msg ) \
    if( ( expr ) == false ) \
        throw LocalAssert( std::string("in function ") + PRINT_FUNC + "  of file " + __FILE__ + " at line " + num2str(__LINE__), std::string( msg ) );

/** Sets a local variable to a value contingent on a logical expression */
#define ASSERT_SET( var, val, expr, msg ) \
    { ASSERT_MSG( expr, msg ); \
      var = val; } \

/** Generates a random float in the interval [ 0.0, 1.0 ] */
#define RAND_FLT ( (float)rand() / RAND_MAX )

/** Generates a random double in the interval [ 0.0, 1.0 ] */
#define RAND_DBL ( (double)rand() / RAND_MAX )

/** Generates a random double in the interval [ lo, hi ] */
#define RAND_RANGE( lo, hi ) ( (lo) + ( ( RAND_DBL ) * ( (hi) - (lo) ) ) )

/** Calculates a logarithm with a specified base */
inline double logB( double x, double b )
{
    return log( x ) / log( b );
}

#define IN_OPEN_INTERVAL( x, lo, hi )       ( (x) >  (lo) && (x) <  (hi) )
#define IN_CLOSED_INTERVAL( x, lo, hi )     ( (x) >= (lo) && (x) <= (hi) )
#define IN_HALFCLOSED_INTERVAL( x, lo, hi ) ( (x) >= (lo) && (x) <  (hi) )
#define IN_RANGE IN_HALFCLOSED_INTERVAL

#define PI 3.1415926535897931

/** Converts a number to a string
  * @param  number - The number to convert
  * @param  prec   - Floating point precision to use
  * @param  w      - Width of the string
  * @return A string representation of the number
  */
template <class T>
std::string num2str( T number, int prec=2, int w=0 ){
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
void str2num( std::string str, T &number ){
    std::istringstream ss( str );
    ss >> number;
}

/** Prints a labeled variable to an output stream
  * @param  var   - The variable to print
  * @param  label - The label for the variable
  * @param  out   - The output stream
  */
template <class T>
void printVar( T var, std::string label="variable", std::ostream &out=std::cout ){
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
void printMatrix( T matrix, int w, int h, std::string label="matrix", bool abbreviate=false, int prec=2, int width=0, std::ostream &out=std::cout ){
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

/** Converts a character to an integer
  * @param  a - The character to convert.  Must be in a 0-9 range.
  * @return The converted integer
  */
int char2Int( const char a );


/** Performs a division and modulo operation
  * @param  dividend  - The dividend   ( Numerator )
  * @param  divisor   - The divisor    ( Denominator )
  * @param  quotient  - The quotient   ( Result for division )
  * @param  remainder - The remeainder ( Result for modulo )
  */
void divmod( int dividend, int divisor, int &quotient, int &remainder );

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

