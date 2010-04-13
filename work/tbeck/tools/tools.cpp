#include "tools.h"

using namespace std;

#ifdef EXEC_TRACE
deque<string> exec_stack;
#endif

int char2Int( char a ){
    ENTER;
    int i;
    ASSERT_SET( i, (int)(a-'0'),
                a >= '0' && a <= '9', "Character must be a numeral" );
    RETURN i;
}

void divmod( int dividend, int divisor, int &quotient, int &remainder ){
    quotient = dividend / divisor;
    remainder = dividend - divisor * quotient;
}

void coords2index( int &idx, const vector<int> &dims, const vector<int> &coords ){
    ASSERT_MSG( coords.size() == dims.size(), "Dimension missmatch" );
    for( int i=0; i<(int)coords.size(); i++ )
        ASSERT_MSG( coords[i] < dims[i], "Coorinate " + num2str(i) + " exceeds dimensional limit" );
    idx = 0;
    int t;
    for( int i=0; i<(int)dims.size(); i++ ){
        t = coords[i];
        for( int j=i+1; j<(int)dims.size(); j++ )
            t *= dims[j];
        idx += t;
    }
}

void index2coords( int idx, const vector<int> &dims, vector<int> &coords ){
    int t = idx;
    coords = vector<int>( dims.size(), 0 );
    for( int i=dims.size()-1; i>1; i-- )
        divmod( t, dims[i], t, coords[i] );
    divmod( t, dims[1], coords[0], coords[1] );
}
