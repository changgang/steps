#ifndef EQUIVALENT_MODEL_IMEXPORTER_H
#define EQUIVALENT_MODEL_IMEXPORTER_H

#include "header/power_system_database.h"

#include <vector>
class EQUIVALENT_MODEL_IMEXPORTER
{
    public:
        EQUIVALENT_MODEL_IMEXPORTER();
        ~EQUIVALENT_MODEL_IMEXPORTER();

        void set_power_system_database(POWER_SYSTEM_DATABASE* db);
        POWER_SYSTEM_DATABASE* get_power_system_database() const;

        bool is_power_system_database_set() const;

        void load_equivalent_model(string file);
    private:
        void load_data_into_ram(string file);
        METER get_meter_from_data(const vector<string> & data_line, size_t& delay, double& coefficient);
        void add_equivalent_device(vector< vector<string> >& model_data);
        void load_ARXL_model(vector< vector<string> >& model_data);
    private:
        POWER_SYSTEM_DATABASE* psdb;
        vector< vector< vector<string> > > data_in_ram;

};

#endif // POWERFLOW_ASSEMBLER_H
