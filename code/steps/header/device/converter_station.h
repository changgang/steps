#ifndef CONVERTER_STATION_H
#define CONVERTER_STATION_H

#include "header/device/nonbus_device.h"
#include "header/device/lcc.h"
#include "header/device/vsc.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"

#include <complex>
#include <string>

using namespace std;

class BUS;

class CONVERTER_STATION
{
    public:
        CONVERTER_STATION();
        virtual ~CONVERTER_STATION();
        void clear();

        void copy_from_const_converter_station(const CONVERTER_STATION& station);

        void set_toolkit(STEPS&);
        STEPS& get_toolkit() const;

        void set_name(string name);
        string get_name() const;

        void set_converter_side(CONVERTER_SIDE side);
        CONVERTER_SIDE get_converter_side() const;
        string get_converter_side_name() const;
    private:
        STEPS* toolkit;
        string name;
        CONVERTER_SIDE side;
};
#endif // CONVERTER_STATION_H
