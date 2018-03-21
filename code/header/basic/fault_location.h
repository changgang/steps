#ifndef FAULT_LOCATION_H
#define FAULT_LOCATION_H

#include "header/basic/constants.h"
#include <complex>

using namespace std;

class FAULT_LOCATION
{
    public:
        FAULT_LOCATION();

        void set_fault_location_in_pu(int location);
        int get_fault_location_in_pu() const;

        bool operator<(const FAULT_LOCATION& fault_location) const;
        bool operator==(const FAULT_LOCATION& fault_location) const;

    private:
        int location_in_pu;
};

#endif // FAULT_LOCATION_H
