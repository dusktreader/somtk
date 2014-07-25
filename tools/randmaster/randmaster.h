#pragma once

#include <ctime>
#include <cstdlib>
#include <climits>
#include <cfloat>
#include <cmath>

/** Convenience class used for generating random integers, and reals with uniform and gaussian distributions within
  * ranges defined when the functions are called
  *
  * @todo:  Add more distributions
  */
class RandMaster
{
private:

    /// The last random gaussian number to be generated
    double lastGauss;

    /// The seed value for the random generator.  Shared by all RandMasters in the current application
    static unsigned int seed;

public:

    /// Constructs a RandMaster object
    RandMaster();

    /// Generates a random integer from within a uniform distribution in the specified range
    int randi(
            int lo=0,  ///< The low end of the range
            int hi=100 ///< The high end of the range
            );

    /// Returns a random real value from with a uniform distribution in the specified range
    float randf(
            float lo=0.0, ///< The low end of the range
            float hi=1.0  ///< The high end of the range
            );

    /// Returns a random double with a uniform distribution in the specified range
    double randd(
            double lo=0.0, ///< The low end of the range
            double hi=1.0  ///< The high end of the range
            );

    /** Generates a random real value from within a gaussian distribution
      * @note   This algorithm uses the Box-Mueller transformation method described in Numerical Recipes 3rd Ed p364
      */
    double randg(
            double mu=0.0,   ///< The center of the distribution
            double sigma=1.0 ///< The standard deviation of the distribution
            );

    /// Generates a boolean value within a given probability of a 'true' or '1' outcome
    bool randp(
            double p ///< The probablility of an 'true' or '1' outcome
            );
};
