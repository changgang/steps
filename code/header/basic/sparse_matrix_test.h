#ifndef SPARSE_MATRIX_TEST_H
#define SPARSE_MATRIX_TEST_H

#include <istream>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/sparse_matrix_define.h"
#include "header/STEPS.h"

using namespace std;

class SPARSE_MATRIX_TEST : public Test::Suite
{
    public:
        SPARSE_MATRIX_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_add_entry();
        void test_set_get_matrix();

        void test_matrix_format();
        void test_get_matrix_size();
        void test_get_matrix_entry_count();

        void test_make_compressed_matrix();
        void test_transpose();

        void test_get_starting_index_of_column();
        void test_get_row_column_number_of_index();

        void test_get_entry_index();
        void test_get_entry_value_with_row_column();
        void test_get_entry_value_with_index();
        void test_change_entry_value();

        void test_clear();

        void test_get_reorder_permutation();

        void test_LU_factorization();
        void test_slove_Ax_equal_b();
        void test_solve_Ax_equal_b_with_operator_slash();

        void test_copy_with_operator_equal();
        void test_copy_with_copy_constructor();

        void test_save_matrix_to_file();
    private:
        void prepare_basic_matrix();
        STEPS_SPARSE_MATRIX matrix;

};

#endif // SPARSE_MATRIX_TEST_H
