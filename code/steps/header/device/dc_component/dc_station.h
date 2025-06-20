#ifndef DC_STATION_H
#define DC_STATION_H



#include "header/device/dc_component/hlcc.h"
#include "header/device/dc_component/hvsc.h"
#include "header/basic/steps_enum.h"

#include "header/block/integral_block.h"


#include <complex>
#include <string>

using namespace std;


class DC_STATION
{
    public:
        DC_STATION(STEPS& toolkit);
        ~DC_STATION();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void clear();

        DEVICE_ID get_device_id() const;
        //set
        void set_station_name(const string name);
        void set_station_number(const unsigned int station_number);
        void set_terminal_number(const unsigned int terminal_number);
        void set_status(const bool status);

        void set_station_structure(const DC_STATION_STRUCTURE structure);
        void set_station_mode(const DC_STATION_MODE mode);
        void set_station_cascade_lcc_number(const unsigned int n_lcc);
        void set_station_parallel_vsc_number(const unsigned int n_vsc);

        void set_vsc_station_dynamic_udc_in_kV(const double udc);
        //get
        string get_station_name() const;
        unsigned int get_station_number() const;
        unsigned int get_terminal_number() const;
        bool get_status() const;

        DC_STATION_STRUCTURE get_station_structure() const;
        DC_STATION_MODE get_station_mode() const;
        unsigned int get_station_cascade_lcc_number() const;
        unsigned int get_station_parallel_vsc_number() const;
        //vector<HLCC*> get_lcc_converters_in_dc_station_with_lcc_cascade();
        //vector<HVSC*> get_vsc_converters_in_dc_station_with_vsc_parallel();

        double get_vsc_station_dynamic_udc_in_kV();

        //solve
        void initialize_dc_station_converters_with_lcc_cascade(vector<HLCC*> lccs_station);
        void initialize_dc_station_converters_with_vsc_parallel(vector<HVSC*> vscs_station);

        //
        void set_udc_block_ceq_in_F(double C);
        void set_udc_block_input(double input);
        void set_udc_block_initial_value(double value);

        double get_udc_block_c_in_F();
        double get_udc_block_input();
        double get_udc_block_initial_value();


        void initialize();
        void run(DYNAMIC_MODE mode);


    private://parameters
        STEPS* toolkit;

        string station_name;
        unsigned int station_number;
        unsigned int terminal_number;
        bool status;

        DC_STATION_STRUCTURE station_structure;
        DC_STATION_MODE station_mode;

        unsigned int n_cascade_lcc;
        unsigned int n_parallel_vsc;

        //vector<HLCC> lccs;
        //vector<HVSC> vscs;

        double vsc_station_dynamic_udc_in_kV;

        INTEGRAL_BLOCK udc_block;
        double C;
        double Udc_input;
        double Udc_initial_value;
};
#endif // STATION_H
