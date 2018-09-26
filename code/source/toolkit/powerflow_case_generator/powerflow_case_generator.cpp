#include "header/toolkit/powerflow_case_generator/powerflow_case_generator.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <istream>
#include <iostream>
#include <ctime>
#include <random>

using namespace std;

POWERFLOW_CASE_GENERATOR::POWERFLOW_CASE_GENERATOR()
{
    set_power_system_database_maximum_bus_number(10000);
    psdb.set_system_base_power_in_MVA(100.0);

    set_dynamic_simulation_time_step_in_s(0.01);

    set_generator_title("MODE GENERATOR");

    set_base_powerflow_data_filename("");
    set_uniform_load_scale(0.0);
    set_random_load_scale(0.0);
    set_maximum_case_count_to_generate(100);
}

POWERFLOW_CASE_GENERATOR::~POWERFLOW_CASE_GENERATOR()
{
    ;
}

void POWERFLOW_CASE_GENERATOR::set_power_system_database_maximum_bus_number(size_t number)
{
    psdb.set_allowed_max_bus_number(number);
}

void POWERFLOW_CASE_GENERATOR::set_generator_title(string title)
{
    operation_mode_generator_title = title;
}

void POWERFLOW_CASE_GENERATOR::set_base_powerflow_data_filename(string filename)
{
    base_powerflow_data_filename = filename;
}

void POWERFLOW_CASE_GENERATOR::set_uniform_load_scale(double scale)
{
    uniform_load_scale = scale;
}

void POWERFLOW_CASE_GENERATOR::set_random_load_scale(double scale)
{
    random_load_scale = scale;
}

void POWERFLOW_CASE_GENERATOR::set_maximum_case_count_to_generate(size_t n)
{
    max_case_count = n;
}

size_t POWERFLOW_CASE_GENERATOR::get_power_system_database_maximum_bus_number() const
{
    return psdb.get_allowed_max_bus_number();
}

string POWERFLOW_CASE_GENERATOR::get_generator_title() const
{
    return operation_mode_generator_title;
}

string POWERFLOW_CASE_GENERATOR::get_base_powerflow_data_filename() const
{
    return base_powerflow_data_filename;
}

double POWERFLOW_CASE_GENERATOR::get_uniform_load_scale() const
{
    return uniform_load_scale;
}

double POWERFLOW_CASE_GENERATOR::get_random_load_scale() const
{
    return random_load_scale;
}

size_t POWERFLOW_CASE_GENERATOR::get_maximum_case_count_to_generate() const
{
    return max_case_count;
}

vector< vector<double> > POWERFLOW_CASE_GENERATOR::generate_load_scale_randoms()
{
    psdb.clear_database();

    PSSE_IMEXPORTER imexporter;

    string pf_file = get_base_powerflow_data_filename();
    imexporter.load_powerflow_data(pf_file);

    vector<DEVICE_ID> loads_did = psdb.get_all_loads_device_id();
    size_t nload = loads_did.size();

    size_t nmax = get_maximum_case_count_to_generate();
    double uniform_scale = get_uniform_load_scale();
    double random_scale = get_random_load_scale();

    vector< vector<double> > random_cases;
    random_device rd;
    mt19937 gen(rd()); // Mersenne Twister MT19937
    normal_distribution<double> normal(uniform_scale,random_scale);// mean and std

    for(size_t i=0; i!=nmax; ++i)
    {
        vector<double> load_scale;
        for(size_t j=0; j!=nload; ++j)
        {
            double scale = normal(gen);
            load_scale.push_back(scale);
        }
        random_cases.push_back(load_scale);
    }
    psdb.clear_database();

    return random_cases;
}

void POWERFLOW_CASE_GENERATOR::generate_cases()
{
    vector< vector<double> > random_cases = generate_load_scale_randoms();

    size_t n_case = random_cases.size();

    for(size_t i=0; i<n_case; ++i)
    {
        generate_case_with_load_random(random_cases[i], i);
    }

}

void POWERFLOW_CASE_GENERATOR::generate_case_with_load_random(vector<double> load_randoms, size_t n)
{
    ostringstream osstream;

    psdb.clear_database();

    PSSE_IMEXPORTER imexporter;

    string pf_file = get_base_powerflow_data_filename();
    imexporter.load_powerflow_data(pf_file);

    // increase load
    vector<LOAD*> loads = psdb.get_all_loads();
    size_t nload = loads.size();
    LOAD* load;
    double scale;
    for(size_t i=0; i!=nload; ++i)
    {
        load = loads[i];
        if(load->get_status()==true)
        {
            scale = load_randoms[i];
            psdb.scale_load_power(load->get_device_id(), scale);
        }
    }

    double Pload = psdb.get_total_load_power_in_MVA().real();
    double Ploss = psdb.get_total_loss_power_in_MVA().real();

    double uniform_load_scale = get_uniform_load_scale();
    Ploss *= (1.0+uniform_load_scale);


    vector<GENERATOR*> generators = psdb.get_all_generators();
    size_t nsource = generators.size();
    GENERATOR* generator;

    double Pgen = psdb.get_total_generation_power_in_MVA().real();
    double uniform_gen_scale = (Pload+Ploss-Pgen)/Pgen;

    bool load_level_exceeds_max_generation = false;

    while(true)
    {
        if(fabs(uniform_gen_scale)<1e-4)
            break;

        for(size_t i=0; i!=nsource; ++i)
        {
            generator = generators[i];
            psdb.scale_generator_power(generator->get_device_id(), uniform_gen_scale);
        }

        double Pgen_new = psdb.get_total_generation_power_in_MVA().real();
        double uniform_gen_scale_new = (Pload+Ploss-Pgen_new)/Pgen_new;

        if(fabs(uniform_gen_scale-uniform_gen_scale_new)<FLOAT_EPSILON)
        {
            load_level_exceeds_max_generation = true;
            break;
        }
        else
            uniform_gen_scale = uniform_gen_scale_new;
    }

    POWERFLOW_SOLVER solver;
    solver.set_flat_start_logic(false);
    solver.set_max_iteration(100);

    solver.solve_with_fast_decoupled_solution();

    if(not solver.is_converged())
    {
        osstream<<"Powerflow is converged. Need to manually tune it.";
        show_information_with_leading_time_stamp(osstream);
    }

    vector<string> splitted_string = split_string(pf_file, ".");
    string filename = splitted_string[0]+ "_load_increase_";
    filename = filename+num2str(uniform_load_scale);
    filename = filename+"_case_"+num2str(n);
    if(load_level_exceeds_max_generation)
        filename = filename+"_load_level_exceeds_pmax";
    if(not solver.is_converged())
        filename = filename+"_power_flow_not_converged";
    filename = filename+".raw";

    string randoms_info = psdb.get_case_additional_information()+"  PS:LOAD RANDOMS ARE: ";
    for(size_t i=0; i!=nload; ++i)
    {
        load = loads[i];
        scale = load_randoms[i];
        randoms_info += num2str(scale)+"  ";
    }

    psdb.set_case_additional_information(randoms_info);


    imexporter.export_powerflow_data(filename);
}

bool POWERFLOW_CASE_GENERATOR::is_valid() const
{
    return true; // disabled
}
void POWERFLOW_CASE_GENERATOR::check()
{
    ;// disabled
}
void POWERFLOW_CASE_GENERATOR::clear()
{
    ;//disabled
}
