#include <iostream>

using namespace std;

enum ops{ ADD, SUB, MLT };

template <class T>
class Matrix{
private:
    T* mat;
    int rows;
    int cols;
    int sz;

    void initMat( int rows, int cols ){
        if( rows <= 0 || cols <= 0 ){
            mat = NULL;
            return;
        }
        this->rows = rows;
        this->cols = cols;
        sz = rows * cols;
        mat = new T[sz];
    }


public:
    Matrix(){
        mat = NULL;
    }

    Matrix( int rows, int cols ){
        initMat( rows, cols );
    }

    Matrix( int rows, int cols, T initVal ){
        initMat( rows, cols );
        for( int i=0; i<sz; i++ )
            mat[i] = initVal;
    }

    Matrix( const Matrix& other ){
        initMat( other.rows, other.cols );
        memcpy( mat, other.mat, sz * sizeof(T) );
    }

    ~Matrix(){
        delete [] mat;
    }

    T& operator()( int row, int col ) const{
        return mat[ cols * row + col ];
    }

    Matrix operator+( const Matrix& other ) const{
        return arithmeticOperator( other, ADD );
    }

    Matrix operator-( const Matrix& other ) const{
        return arithmeticOperator( other, SUB );
    }

    Matrix operator*( const Matrix& other ) const{
        return arithmeticOperator( other, MLT );
    }

    Matrix arithmeticOperator( const Matrix& other, ops op ) const{
        if( mat == NULL || other.mat == NULL || other.rows != rows || other.cols != cols )
            return Matrix();
        Matrix neo( rows, cols );
        switch( op ){
            case ADD: for( int i=0; i<sz; i++ ) neo.mat[i] =  mat[i] + other.mat[i]; break;
            case SUB: for( int i=0; i<sz; i++ ) neo.mat[i] =  mat[i] - other.mat[i]; break;
            case MLT: for( int i=0; i<sz; i++ ) neo.mat[i] =  mat[i] * other.mat[i]; break;
        }
        return neo;
    }

    friend ostream& operator<<( ostream& out, const Matrix& self ){
        if( self.mat == NULL ){
            out << "<Invalid Matrix>" << endl;
            return out;
        }
        for( int row=0, idx=0; row<self.rows; row++ ){
            out << "|   ";
            for( int col=0; col<self.cols; col++ )
                out << self.mat[idx++] << "   ";
            out << "|" << endl;
        }
        return out;
    }

};

int main(int argc, char *argv[]){
    Matrix<int> A( 10, 10, 10 );
    A( 5, 5 ) = 80;
    Matrix<int> B = A;
    B( 5, 5 ) += 19;
    cout << A << endl << endl;
    cout << B << endl;
    Matrix<double> C( 5, 5, 3.14 );
    C( 1, 1 ) = 1.5;
    Matrix<double> D = C * C;
    cout << D << endl;
    Matrix<char> E;
    cout << E << endl;
    Matrix<char> F( 0, 2 );
    cout << F << endl;
}
