#ifndef EIGN_CALCULATOR_H
#define EIGN_CALCULATOR_H

#include "header/basic/power_mismatch_struct.h"
#include "header/basic/continuous_buffer.h"
#include "header/meter/meter.h"
#include "header/network/network_matrix.h"
#include "header/basic/sparse_matrix_define.h"
#include <fstream>

class POWER_SYSTEM_DATABASE;

class EIGN_CALCULATOR
{
    public:
        EIGN_CALCULATOR(STEPS& toolkit);
        ~EIGN_CALCULATOR();
        STEPS& get_toolkit() const;

        void clear();

        void build_system_linearized_matrix_ABCD();
        void save_system_linearized_matrix_to_file(string file);
    public:
    private:
        void build_linearized_matrix_ABCD_for_all_devices();
        void temp_func();

        STEPS* toolkit;

        STEPS_SPARSE_MATRIX State_Matrix;
};

#endif // EIGN_CALCULATOR_H
