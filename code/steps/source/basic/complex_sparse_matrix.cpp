#include "header/basic/complex_sparse_matrix.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;

COMPLEX_SPARSE_MATRIX::COMPLEX_SPARSE_MATRIX()
{
    //constructor
    update_clock_when_LU_factorization_is_performed();
    update_clock_when_matrix_is_changed();
    set_lu_factorization_failed_flag(false);
}

COMPLEX_SPARSE_MATRIX::~COMPLEX_SPARSE_MATRIX()
{
    // destructor
}


bool COMPLEX_SPARSE_MATRIX::matrix_in_compressed_column_form() const
{
    return not matrix_in_triplet_form();
}

int COMPLEX_SPARSE_MATRIX::get_column_number_of_entry_index(int index) const
{
    int n = get_matrix_size();
    if(index<=get_starting_index_of_column(n))
    {
        int k_start, k_end;
        for(int col = 0; col!=n; ++col)
        {
            k_start = get_starting_index_of_column(col);
            k_end = get_starting_index_of_column(col+1)-1;
            if(index>=k_start and index<=k_end)
                return col;
        }
        return INDEX_NOT_EXIST;
    }
    else
        return INDEX_NOT_EXIST;
}

complex<double> COMPLEX_SPARSE_MATRIX::get_entry_value(int row, int col) const
{
    int index = get_entry_index(row, col);
    if(index!=INDEX_NOT_EXIST)
        return get_entry_value(index);
    else
        return 0.0;
}

complex<double> COMPLEX_SPARSE_MATRIX::get_entry_value(int index) const
{
    //return complex<double>(get_real_entry_value(index), get_imag_entry_value(index));
    return get_complex_entry_value(index);
}

void COMPLEX_SPARSE_MATRIX::change_entry_value(int row, int col, const complex<double>& value)
{
    // get entry index of a compress matrix
    int index = get_entry_index(row, col); // if it is triplet format, will be converted into compressed format
    change_entry_value(index, value);
}

void COMPLEX_SPARSE_MATRIX::change_entry_value(int index, const complex<double>& value)
{
    if(index != INDEX_NOT_EXIST)
    {
        change_real_entry_value(index, value.real());
        change_imag_entry_value(index, value.imag());
    }
}


bool COMPLEX_SPARSE_MATRIX::LU_factorization_is_performed() const
{
    if(get_clock_when_LU_factorization_is_performed() > get_clock_when_matrix_is_changed())
        return true;
    else
        return false;
}
void COMPLEX_SPARSE_MATRIX::update_clock_when_matrix_is_changed()
{
    clock_when_matrix_is_changed = clock();
}

void COMPLEX_SPARSE_MATRIX::update_clock_when_LU_factorization_is_performed()
{
    clock_when_LU_factorization_is_performed = clock();
}

clock_t COMPLEX_SPARSE_MATRIX::get_clock_when_matrix_is_changed() const
{
    return clock_when_matrix_is_changed;
}

clock_t COMPLEX_SPARSE_MATRIX::get_clock_when_LU_factorization_is_performed() const
{
    return clock_when_LU_factorization_is_performed;
}

void COMPLEX_SPARSE_MATRIX::set_lu_factorization_failed_flag(bool flag)
{
    lu_factorization_failed = flag;
}

bool COMPLEX_SPARSE_MATRIX::get_lu_factorization_failed_flag() const
{
    return lu_factorization_failed;
}

bool COMPLEX_SPARSE_MATRIX::is_lu_factorization_failed() const
{
    return lu_factorization_failed;
}

bool COMPLEX_SPARSE_MATRIX::is_lu_factorization_successful() const
{
    return !lu_factorization_failed;
}
