#include "suspect.h"

using namespace std;

Suspect::Suspect( string name, int realCat, int catCt, int w, int h ){
    ENTER;
    this->name = name;
    hist = new SOMHistogram( w, h );
    this->realCat = realCat;
    this->catCt = catCt;
    votes = vector<double>( catCt, 0.0 );
    RETURN;
}

Suspect::~Suspect(){
    ENTER;
    delete hist;
    RETURN;
}

void Suspect::setANNVectors( double* input, int inputW, double* output, int outputW ){
    ENTER;
    ASSERT_MSG( output == NULL || outputW == catCt, "Class count missmatch" );
    for( int i=0; i<inputW; i++ )
        input[i] = hist->getBin( i );
    if( output==NULL ){
        RETURN;
    }
    for( int i=0; i<outputW; i++ )
        output[i] = i==realCat ? 1.0 : -1.0;
    RETURN;
}

void Suspect::setCatVotes( double* output, int outputW ){
    ENTER;
    ASSERT_MSG( outputW == catCt, "Class count mismatch" );
    for( int i=0; i<outputW; i++ ){
        votes[i] = output[i];
    }
    RETURN;
}

vector<double> Suspect::getCatVotes(){
    ENTER;
    RETURN votes;
}

int Suspect::getRealCat(){
    ENTER;
    RETURN realCat;
}

int Suspect::getPredCat(){
    ENTER;
    int maxVote = 0;
    double maxVal  = votes[0];
    for( int i=1; i<catCt; i++ ){
        if( votes[i] >= maxVal ){                                                                                       /// @note that this will favor a higher category if two or more votes are equivalent
            maxVote = i;
            maxVal = votes[i];
        }
    }
    RETURN maxVote;
}

void Suspect::incrementHistogram( int index ){
    ENTER;
    hist->increment( index );
    RETURN;
}

void Suspect::incrementHistogram( int x, int y ){
    ENTER;
    hist->increment( x, y );
    RETURN;
}

void Suspect::normalizeHistogram(){
    ENTER;
    hist->normalize();
    RETURN;
}

void Suspect::showHistogram( string msg ){
    ENTER;
    hist->showHistogram( msg );
    RETURN;
}

string Suspect::getName(){
    ENTER;
    RETURN name;
}
