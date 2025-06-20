#ifndef DC_TERMINAL_H
#define DC_TERMINAL_H


#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/device/dc_component/dc_station.h"

#include "header/model/model.h"
#include <complex>
#include <string>

using namespace std;

class BUS;
class DC_BUS;
class MODEL;


class DC_TERMINAL
{
    public:
        DC_TERMINAL(STEPS& toolkit);
        ~DC_TERMINAL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void clear();
        virtual DEVICE_ID get_device_id() const;

        virtual void set_model(MODEL* model) ;
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0) ;

        //set
        void set_terminal_name(const string name);
        void set_terminal_number(const unsigned int terminal_number);
        void set_terminal_dc_bus_number(const unsigned int dcbus);
        void set_status(const bool status);
        void set_terminal_mode(const DC_TERMINAL_MODE mode);

        //get
        string get_terminal_name() const;
        unsigned int get_terminal_number() const;
        unsigned int get_terminal_dc_bus_number() const;
        bool get_status() const;
        DC_TERMINAL_MODE get_terminal_mode() const;

        //vector<DC_STATION*> get_dc_stations_in_terminal();


        //solve
        //void initialize_dc_terminal_stations(vector<DC_STATION*> stations);


    private://parameters
        STEPS* toolkit;
        string terminal_name;
        unsigned int terminal_number;
        unsigned int dcbus_number;
        bool status;
        DC_TERMINAL_MODE terminal_mode;
        //vector<DC_STATION> dc_stations;
};
#endif // DC_TERMINAL_H
