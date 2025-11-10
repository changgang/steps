#ifndef SV_SOURCE_H
#define SV_SOURCE_H

#include <complex>
#include <string>
#include "header/device/source.h"
#include "header/model/svs_model/svs_control_model/svs_control_model.h"

using namespace std;

class SV_SOURCE : public SOURCE
{
    public:
        SV_SOURCE(STEPS& toolkit);
        virtual ~SV_SOURCE();

        void set_sv_source_bus(unsigned int bus);
        unsigned int get_sv_source_bus() const;

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        void set_svs_control_model(SVS_CONTROL_MODEL* model);
        SVS_CONTROL_MODEL* get_svs_control_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual void report() const;
        virtual void save() const;

        virtual SV_SOURCE& operator=(const SV_SOURCE& sv_source);

        void set_sequence_parameter_import_flag(bool flag);
        bool get_sequence_parameter_import_flag() const;

    private:
        SVS_CONTROL_MODEL* svs_control_model;
    private:
        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();

        bool sequence_parameter_import_flag;

};
#endif // SV_SOURCE_H
