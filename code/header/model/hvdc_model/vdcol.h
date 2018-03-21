#ifndef VDCOL_H
#define VDCOL_H

#include <cstdlib>
#include <vector>

using namespace std;

class VDCOL
{
    public:
        VDCOL();
        VDCOL(const VDCOL& limiter);
        ~VDCOL();

        void append_vdcol_point_in_kV_kA(double V_in_kV, double I_in_kA);

        size_t get_vdcol_point_count() const;
        double get_vdcol_voltage_of_point_in_kV(size_t index) const;
        double get_vdcol_current_of_point_in_kA(size_t index) const;

        double get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(double V_in_kV) const;

        VDCOL& operator=(const VDCOL& limiter);

        void clear();



    private:
        void copy_from_const_vdcol(const VDCOL& limiter);

        double get_vdcol_voltage_of_last_point_in_kV() const;
        double get_vdcol_current_of_last_point_in_kA() const;

        vector< vector<double> > vdcol_parameters;
};

#endif // VDCOL_H
