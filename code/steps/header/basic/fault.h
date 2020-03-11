#ifndef FAULT_H
#define FAULT_H

#include "header/basic/steps_enum.h"
#include <complex>

using namespace std;

class FAULT
{
    public:
        FAULT();
        virtual ~FAULT();

        void set_fault_type(FAULT_TYPE fault_type);

        void set_fault_shunt_in_pu(const complex<double>& y);

        FAULT_TYPE get_fault_type() const;
        string get_fault_type_string() const;

        complex<double> get_fault_shunt_in_pu() const;

        bool is_faulted() const;
        void clear();

        virtual FAULT& operator=(const FAULT& fault);

    private:
        FAULT_TYPE fault_type;
        complex<double> fault_shunt_in_pu;
};

#endif // FAULT_H
