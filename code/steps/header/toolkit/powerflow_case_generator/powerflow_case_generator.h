#ifndef POWERFLOW_CASE_GENERATOR_H
#define POWERFLOW_CASE_GENERATOR_H

#include "header/meter/meter.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include <fstream>

class POWERFLOW_CASE_GENERATOR
{
    public:
        POWERFLOW_CASE_GENERATOR();
        virtual ~POWERFLOW_CASE_GENERATOR();

        void set_power_system_database_maximum_bus_number(unsigned int number);
        void set_generator_title(string title);

        void set_base_powerflow_data_filename(string filename);
        void set_uniform_load_scale(double scale);
        void set_random_load_scale(double scale);
        void set_maximum_case_count_to_generate(unsigned int n);

        unsigned int get_power_system_database_maximum_bus_number() const;
        string get_generator_title() const;
        string get_base_powerflow_data_filename() const;
        double get_uniform_load_scale() const;
        double get_random_load_scale() const;
        unsigned int get_maximum_case_count_to_generate() const;

        void generate_cases();
        vector< vector<double> > generate_load_scale_randoms();

    private:
        bool is_generator_properly_set() const;
        void generate_case_with_load_random(vector<double> load_randoms, unsigned int n);

        string operation_mode_generator_title;
        string base_powerflow_data_filename;

        double uniform_load_scale;
        double random_load_scale;

        unsigned int max_case_count;
    private:
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
};
#endif // POWERFLOW_CASE_GENERATOR_H
