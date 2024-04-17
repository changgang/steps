#include "header/toolkit/eign_calculator/eign_calculator.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

class STEPS;

#define ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR

#define USE_DYNAMIC_CURRENT_MISMATCH_CONTROL

EIGN_CALCULATOR::EIGN_CALCULATOR(STEPS& toolkit)
{
    this->toolkit = (&toolkit);

    clear();
}

EIGN_CALCULATOR::~EIGN_CALCULATOR()
{
    toolkit = NULL;

    clear();
}

STEPS& EIGN_CALCULATOR::get_toolkit() const
{
    return *toolkit;
}

void EIGN_CALCULATOR::clear()
{
}

void EIGN_CALCULATOR::build_system_linearized_matrix_ABCD()
{
    build_linearized_matrix_ABCD_for_all_devices();
    temp_func();
}

void EIGN_CALCULATOR::build_linearized_matrix_ABCD_for_all_devices()
{
    STEPS& toolkit = get_toolkit();

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<GENERATOR*> gens = psdb.get_all_generators();
    unsigned int n = gens.size();
    for(unsigned int i = 0; i<n; ++i)
    {
        GENERATOR* gen = gens[i];
        gen->build_linearized_matrix_ABCD();
    }

    vector<LOAD*> loads = psdb.get_all_loads();
    unsigned int np = loads.size();
    for(unsigned int i = 0; i<np; ++i)
    {
        LOAD* load = loads[i];
        load->build_linearized_matrix_ABCD();
    }
}

void EIGN_CALCULATOR::temp_func()
{
    STEPS& toolkit = get_toolkit();

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    vector<GENERATOR*> gens = psdb.get_all_generators();
    vector<LOAD*> loads = psdb.get_all_loads();

    unsigned int n = 0;

    NETWORK_MATRIX& network = default_toolkit.get_network_matrix();
    STEPS_COMPLEX_SPARSE_MATRIX& Y = network.get_network_Y_matrix();
    unsigned int N = psdb.get_bus_count();
    unsigned int N_gen = psdb.get_generator_count();
    unsigned int N_load = psdb.get_load_count();
    unsigned int Ng = 0;
    unsigned int Nl = 0;
    unsigned int n_gen[100],n_link[100],n_ssa[100];
    for(unsigned int i = 1; i<=N; i++)
    {
        unsigned int k = 0;
        for(unsigned int j = 0; j!=N_gen; ++j)
        {
            if(gens[j]->is_connected_to_bus(i))
                k = 1;
        }
        if(k==1)
            n_gen[Ng++] = network.get_internal_bus_number_of_physical_bus(i);
        if(k==0)
            n_link[Nl++] = network.get_internal_bus_number_of_physical_bus(i);
    }
    for(unsigned int i = 0; i<N; i++)
    {
        if(i<Ng)
            n_ssa[i] = n_gen[i];
        else
            n_ssa[i] = n_link[i-Ng];
    }
    STEPS_SPARSE_MATRIX Y_ssa;
    for(unsigned int i = 0; i<N; i++)
    {
        for(unsigned int j = 0; j<N; j++)
        {
            double Gij = Y.get_entry_value(n_ssa[i],n_ssa[j]).real();
            double Bij = Y.get_entry_value(n_ssa[i],n_ssa[j]).imag();
            if(i==j)
            {
                unsigned int id = network.get_physical_bus_number_of_internal_bus(n_ssa[i]);
                complex<double> D00 = 0;
                complex<double> D01 = 0;
                complex<double> D10 = 0;
                complex<double> D11 = 0;
                if(i<Ng)
                {
                    for(unsigned int k = 0; k!=N_gen; ++k)
                    {
                        if(gens[k]->is_connected_to_bus(id))
                        {
                            STEPS_SPARSE_MATRIX Dg = gens[k]->get_linearized_matrix_D();
                            D00 += Dg.get_entry_value(0,0);
                            D01 += Dg.get_entry_value(0,1);
                            D10 += Dg.get_entry_value(1,0);
                            D11 += Dg.get_entry_value(1,1);
                        }
                    }
                }
                for(unsigned int k = 0; k!=N_load; ++k)
                {
                    if(loads[k]->is_connected_to_bus(id))
                    {
                        STEPS_SPARSE_MATRIX Dl = loads[k]->get_linearized_matrix_D();
                        D00 += Dl.get_entry_value(0,0);
                        D01 += Dl.get_entry_value(0,1);
                        D10 += Dl.get_entry_value(1,0);
                        D11 += Dl.get_entry_value(1,1);
                    }
                }
                Y_ssa.add_entry(2*i,2*j, Gij-D00);
                Y_ssa.add_entry(2*i,2*j+1, -Bij-D01);
                Y_ssa.add_entry(2*i+1,2*j, Bij-D10);
                Y_ssa.add_entry(2*i+1,2*j+1, Gij-D11);
            }
            else
            {
                Y_ssa.add_entry(2*i,2*j, Gij);
                Y_ssa.add_entry(2*i,2*j+1, -Bij);
                Y_ssa.add_entry(2*i+1,2*j, Bij);
                Y_ssa.add_entry(2*i+1,2*j+1, Gij);
            }
        }
    }
    Y_ssa.compress_and_merge_duplicate_entries();
    vector<STEPS_SPARSE_MATRIX*> matrix;
    for(unsigned int i = 0; i<n; ++i)
    {
        GENERATOR* gen = gens[i];
        STEPS_SPARSE_MATRIX* Aptr = gen->get_linearized_matrix_pointer_A();
        matrix.push_back(Aptr);
    }
    STEPS_SPARSE_MATRIX Ag = concatenate_matrix_diagnally(matrix);
    matrix.clear();
    for(unsigned int i = 0; i<n; ++i)
    {
        GENERATOR* gen = gens[i];
        STEPS_SPARSE_MATRIX* Bptr = gen->get_linearized_matrix_pointer_B();
        matrix.push_back(Bptr);
    }
    STEPS_SPARSE_MATRIX Bg = concatenate_matrix_diagnally(matrix);
    matrix.clear();
    for(unsigned int i = 0; i<n; ++i)
    {
        GENERATOR* gen = gens[i];
        STEPS_SPARSE_MATRIX* Cptr = gen->get_linearized_matrix_pointer_C();
        matrix.push_back(Cptr);
    }
    STEPS_SPARSE_MATRIX Cg = concatenate_matrix_diagnally(matrix);
    matrix.clear();
    for(unsigned int i = 0; i<n; ++i)
    {
        GENERATOR* gen = gens[i];
        STEPS_SPARSE_MATRIX* Dptr = gen->get_linearized_matrix_pointer_D();
        matrix.push_back(Dptr);
    }
    STEPS_SPARSE_MATRIX Dg = concatenate_matrix_diagnally(matrix);
    matrix.clear();
    STEPS_SPARSE_MATRIX zero_Cg = Cg-Cg;
    STEPS_SPARSE_MATRIX minus_Cg = zero_Cg-Cg;
    STEPS_SPARSE_MATRIX A = Ag;
    STEPS_SPARSE_MATRIX B = expand_matrix_to_new_size(Bg, Bg.get_matrix_row_count(), 2*N);
    STEPS_SPARSE_MATRIX C = expand_matrix_to_new_size(minus_Cg, 2*N, Cg.get_matrix_column_count());
    STEPS_SPARSE_MATRIX D = Y_ssa;
    STEPS_SPARSE_MATRIX INVD = inv(D);
    STEPS_SPARSE_MATRIX B_INVD = B*INVD;
    STEPS_SPARSE_MATRIX B_INVD_C = B_INVD*C;
    State_Matrix = A-B_INVD_C;
}
void temp_func();

void EIGN_CALCULATOR::save_system_linearized_matrix_to_file(string file)
{
    State_Matrix.save_matrix_to_file("State_Matrix.csv");
}
