#ifndef LVRT_RELAY_H
#define LVRT_RELAY_H

#include "header/model/converter_common_models/vrt_model/vrt_relay_model.h"

class LVRT_RELAY : public VRT_RELAY_MODEL
{
    public:
        LVRT_RELAY(STEPS& toolkit);
        ~LVRT_RELAY();
        LVRT_RELAY(const LVRT_RELAY& model);
        virtual LVRT_RELAY& operator=(const LVRT_RELAY& model);
        void clear();
        void copy_from_const_model(const LVRT_RELAY& model);
    public:
        void set_lvrt_voltage_threshold(double v);
        double get_lvrt_voltage_threshold() const;
        virtual void initialize_vrt_relay();
        virtual void check_vrt_relay();
    private:
        double vth_lvrt;
};

#endif // LVRT_RELAY_H
