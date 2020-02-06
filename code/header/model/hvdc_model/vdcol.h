#ifndef VDCOL_H
#define VDCOL_H

#include <cstdlib>
#include <vector>

#include "header/basic/base.h"
#include "header/basic/constants.h"
using namespace std;

class VDCOL
{
    public:
        VDCOL();
        VDCOL(const VDCOL& limiter);
        ~VDCOL();

        void append_vdcol_point_in_kV_kA(double V_in_kV, double I_in_kA);

        unsigned int get_vdcol_point_count() const;
        double get_vdcol_voltage_of_point_in_kV(unsigned int index) const;
        double get_vdcol_current_of_point_in_kA(unsigned int index) const;

        double get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(double V_in_kV) const;

        VDCOL& operator=(const VDCOL& limiter);

        void clear();
    private:
        void copy_from_const_vdcol(const VDCOL& limiter);

        double get_vdcol_voltage_of_last_point_in_kV() const;
        double get_vdcol_current_of_last_point_in_kA() const;

        bool is_valid() const;
        void check();

        float vdcol_parameters[STEPS_MAX_VDCOL_TABLE_SIZE][2];
};

#endif // VDCOL_H
