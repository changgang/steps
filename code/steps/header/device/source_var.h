#ifndef SOURCE_VAR_H
#define SOURCE_VAR_H

#include <complex>

using namespace std;

class SOURCE_VAR
{
    public:
        SOURCE_VAR();
    public:
        double p_generation_MW, q_generation_MVar;

        unsigned int bus_to_regulate;

        complex<double> source_Z_pu;

        double p_max_MW, p_min_MW, q_max_MVar, q_min_MVar;
        double voltage_to_regulate_pu;
};
#endif // SOURCE_VAR_H
