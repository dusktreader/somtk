#include "randmaster.h"

unsigned int RandMaster::seed = 0;



RandMaster::RandMaster()
{
    if( seed == 0 )
        seed = time(0);
    else
        seed += 7;
    srand( seed );
    lastGauss = DBL_MAX;
}



int RandMaster::randi( int lo, int hi )
{
    return lo + rand() % ( hi - lo );
}



float RandMaster::randf( float lo, float hi )
{
    return (float)randd( lo, hi );
}



double RandMaster::randd( double lo, double hi )
{
    return lo + ( (double)rand() / RAND_MAX ) * ( hi - lo );
}



double RandMaster::randg( double mu, double sigma )
{
    /// @tod:  Document this function / algorithm

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



bool RandMaster::randp( double p )
{
    return ( (double)rand() / RAND_MAX ) < p;
}
