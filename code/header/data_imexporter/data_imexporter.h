#ifndef DATA_IMEXPORTER_H
#define DATA_IMEXPORTER_H

#include "header/power_system_database.h"
#include "header/basic/base.h"
#include <string>
#include <fstream>

using namespace std;
class DATA_IMEXPORTER : public BASE
{
    public:
        DATA_IMEXPORTER();
        virtual ~DATA_IMEXPORTER();

        void set_base_frequency_in_Hz(double fbase);
        void set_export_zero_impedance_line_logic(bool logic);
        void set_export_out_of_service_bus_logic(bool logic);
        double get_base_frequency_in_Hz() const;
        bool get_export_zero_impedance_line_logic() const;
        bool get_export_out_of_service_bus_logic() const;

        void export_shadowed_bus_pair(string file) const;

        virtual void load_powerflow_data(string pf_source) = 0;
        virtual void load_dynamic_data(string dy_source) = 0;
        virtual void load_sequence_data(string sq_source) = 0;

        virtual void export_powerflow_data(string file, bool export_zero_impedance_line=true) = 0;
        virtual void export_dynamic_data(string file) = 0;
        virtual void export_sequence_data(string file) = 0;
    private:
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        double base_frequency_in_Hz;
        bool export_zero_impedance_line;
        bool export_out_of_service_bus;
};

#endif // DATA_IMEXPORTER_H
