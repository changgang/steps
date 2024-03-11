#include "header/basic/test_macro.h"
#include "header/basic/sparse_matrix_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <complex>

#ifdef ENABLE_STEPS_TEST
using namespace std;

SPARSE_MATRIX_TEST::SPARSE_MATRIX_TEST()
{
    TEST_ADD(SPARSE_MATRIX_TEST::test_constructor);
    TEST_ADD(SPARSE_MATRIX_TEST::test_add_entry);
    TEST_ADD(SPARSE_MATRIX_TEST::test_set_get_matrix);
    TEST_ADD(SPARSE_MATRIX_TEST::test_matrix_format);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_matrix_size);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_matrix_entry_count);
    TEST_ADD(SPARSE_MATRIX_TEST::test_make_compressed_matrix);
    TEST_ADD(SPARSE_MATRIX_TEST::test_transpose);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_starting_index_of_column);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_row_column_number_of_index);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_entry_index);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_entry_value_with_row_column);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_entry_value_with_index);
    TEST_ADD(SPARSE_MATRIX_TEST::test_change_entry_value);
    TEST_ADD(SPARSE_MATRIX_TEST::test_clear);
    TEST_ADD(SPARSE_MATRIX_TEST::test_get_reorder_permutation);
    TEST_ADD(SPARSE_MATRIX_TEST::test_slove_Ax_equal_b);
    TEST_ADD(SPARSE_MATRIX_TEST::test_solve_Ax_equal_b_with_operator_slash);

    TEST_ADD(SPARSE_MATRIX_TEST::test_copy_with_operator_equal);
    TEST_ADD(SPARSE_MATRIX_TEST::test_copy_with_copy_constructor);

    TEST_ADD(SPARSE_MATRIX_TEST::test_matrix_add);
    TEST_ADD(SPARSE_MATRIX_TEST::test_matrix_minus);
    TEST_ADD(SPARSE_MATRIX_TEST::test_matrix_multiply);
    TEST_ADD(SPARSE_MATRIX_TEST::test_matrix_divide);
    TEST_ADD(SPARSE_MATRIX_TEST::test_matrix_inverse);
    TEST_ADD(SPARSE_MATRIX_TEST::test_concatenate_matrix_diagnally);
    TEST_ADD(SPARSE_MATRIX_TEST::test_build_identity_matrix);
    TEST_ADD(SPARSE_MATRIX_TEST::test_expand_matrix_to_new_size);
    TEST_ADD(SPARSE_MATRIX_TEST::test_shrink_matrix_to_new_size);
    TEST_ADD(SPARSE_MATRIX_TEST::test_change_matrix_to_new_size);


    TEST_ADD(SPARSE_MATRIX_TEST::test_save_matrix_to_file);
}

void SPARSE_MATRIX_TEST::setup()
{
    ;
}

void SPARSE_MATRIX_TEST::tear_down()
{
    matrix.clear();
    TEST_ASSERT(matrix.get_matrix_size()==1);
    show_test_end_information();
}

void SPARSE_MATRIX_TEST::prepare_basic_matrix()
{
    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    // LU factorization of real part is
    //     [1  0  0]      [1  0  0]
    // L = [0  1  0]  U = [0  2  4]
    //     [1  0  1]      [0  0  1]
    // solution to real*x = b where b=[2 20 5]' is
    // [2  4  3]'

    matrix.clear();
    matrix.add_entry(0,0,1.0);
    matrix.add_entry(1,1,complex<double>(2.0, 5.0));
    matrix.add_entry(1,2,complex<double>(4.0, 9.0));
    matrix.add_entry(2,2,complex<double>(1.0, 2.0));
    matrix.add_entry(2,0,1.0);

    matrix.compress_and_merge_duplicate_entries();
}

void SPARSE_MATRIX_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    TEST_ASSERT(matrix.get_matrix_size()==1);
}

void SPARSE_MATRIX_TEST::test_add_entry()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    STEPS_SPARSE_MATRIX mat;
    mat.add_entry(0,0,1);
    mat.add_entry(1,0,complex<double>(1.0, 2.0));
    mat.add_entry(0,1,complex<double>(2.0, 3.0));
    mat.add_entry(1,1,complex<double>(0.0, 4.0));

    mat.compress_and_merge_duplicate_entries();

    mat.add_entry(0,0,complex<double>(1.0, 2.0));

    TEST_ASSERT(mat.matrix_in_compressed_column_form()==true);
    TEST_ASSERT(mat.get_entry_value(0,0)==complex<double>(2.0, 2.0));
    TEST_ASSERT(mat.get_entry_value(1,0)==complex<double>(1.0, 2.0));
    TEST_ASSERT(mat.get_entry_value(0,1)==complex<double>(2.0, 3.0));
    TEST_ASSERT(mat.get_entry_value(1,1)==complex<double>(0.0, 4.0));

}
void SPARSE_MATRIX_TEST::test_set_get_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.get_matrix_size()==3);

    TEST_ASSERT(matrix.get_entry_value(0,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(0,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(0,2)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,0)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,1)==complex<double>(2.0, 5.0));
    TEST_ASSERT(matrix.get_entry_value(1,2)==complex<double>(4.0, 9.0));
    TEST_ASSERT(matrix.get_entry_value(2,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2,2)==complex<double>(1.0, 2.0));
    TEST_ASSERT(matrix.get_entry_value(0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2)==complex<double>(2.0, 5.0));
    TEST_ASSERT(matrix.get_entry_value(3)==complex<double>(4.0, 9.0));
    TEST_ASSERT(matrix.get_entry_value(1)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(4)==complex<double>(1.0, 2.0));
}

void SPARSE_MATRIX_TEST::test_matrix_format()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    TEST_ASSERT(matrix.matrix_in_triplet_form()==true);
    TEST_ASSERT(matrix.matrix_in_compressed_column_form()==false);

    prepare_basic_matrix();

    TEST_ASSERT(matrix.matrix_in_triplet_form()==false);
    TEST_ASSERT(matrix.matrix_in_compressed_column_form()==true);
}

void SPARSE_MATRIX_TEST::test_get_matrix_size()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();
    TEST_ASSERT(matrix.get_matrix_size()==3);
}

void SPARSE_MATRIX_TEST::test_get_matrix_entry_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();
    TEST_ASSERT(matrix.get_matrix_entry_count()==5);

}

void SPARSE_MATRIX_TEST::test_make_compressed_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.matrix_in_triplet_form()==false);
    TEST_ASSERT(matrix.matrix_in_compressed_column_form()==true);
}

void SPARSE_MATRIX_TEST::test_transpose()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    matrix.transpose();

    TEST_ASSERT(matrix.get_entry_value(0,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,0)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2,0)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(0,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,1)==complex<double>(2.0, 5.0));
    TEST_ASSERT(matrix.get_entry_value(2,1)==complex<double>(4.0, 9.0));
    TEST_ASSERT(matrix.get_entry_value(0,2)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,2)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2,2)==complex<double>(1.0, 2.0));
}

void SPARSE_MATRIX_TEST::test_get_starting_index_of_column()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.get_starting_index_of_column(0)==0);
    TEST_ASSERT(matrix.get_starting_index_of_column(1)==2);
    TEST_ASSERT(matrix.get_starting_index_of_column(2)==3);
}

void SPARSE_MATRIX_TEST::test_get_row_column_number_of_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.get_row_number_of_entry_index(0)==0);
    TEST_ASSERT(matrix.get_row_number_of_entry_index(1)==2);
    TEST_ASSERT(matrix.get_row_number_of_entry_index(2)==1);
    TEST_ASSERT(matrix.get_row_number_of_entry_index(3)==1);
    TEST_ASSERT(matrix.get_row_number_of_entry_index(4)==2);

    TEST_ASSERT(matrix.get_column_number_of_entry_index(0)==0);
    TEST_ASSERT(matrix.get_column_number_of_entry_index(1)==0);
    TEST_ASSERT(matrix.get_column_number_of_entry_index(2)==1);
    TEST_ASSERT(matrix.get_column_number_of_entry_index(3)==2);
    TEST_ASSERT(matrix.get_column_number_of_entry_index(4)==2);
}


void SPARSE_MATRIX_TEST::test_get_entry_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.get_entry_index(0,0)==0);
    TEST_ASSERT(matrix.get_entry_index(0,1)==INDEX_NOT_EXIST);
    TEST_ASSERT(matrix.get_entry_index(0,2)==INDEX_NOT_EXIST);
    TEST_ASSERT(matrix.get_entry_index(1,0)==INDEX_NOT_EXIST);
    TEST_ASSERT(matrix.get_entry_index(1,1)==2);
    TEST_ASSERT(matrix.get_entry_index(1,2)==3);
    TEST_ASSERT(matrix.get_entry_index(2,0)==1);
    TEST_ASSERT(matrix.get_entry_index(2,1)==INDEX_NOT_EXIST);
    TEST_ASSERT(matrix.get_entry_index(2,2)==4);
}

void SPARSE_MATRIX_TEST::test_get_entry_value_with_row_column()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.get_matrix_size()==3);

    TEST_ASSERT(matrix.get_entry_value(0,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(0,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(0,2)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,0)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1,1)==complex<double>(2.0, 5.0));
    TEST_ASSERT(matrix.get_entry_value(1,2)==complex<double>(4.0, 9.0));
    TEST_ASSERT(matrix.get_entry_value(2,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2,2)==complex<double>(1.0, 2.0));
}


void SPARSE_MATRIX_TEST::test_get_entry_value_with_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    TEST_ASSERT(matrix.get_matrix_size()==3);

    TEST_ASSERT(matrix.get_entry_value(0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(1)==complex<double>(1.0, 0.0));
    TEST_ASSERT(matrix.get_entry_value(2)==complex<double>(2.0, 5.0));
    TEST_ASSERT(matrix.get_entry_value(3)==complex<double>(4.0, 9.0));
    TEST_ASSERT(matrix.get_entry_value(4)==complex<double>(1.0, 2.0));
}

void SPARSE_MATRIX_TEST::test_change_entry_value()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    matrix.change_entry_value(0, 0, 7.0);
    TEST_ASSERT(matrix.get_entry_value(0, 0)==7.0);

    matrix.change_entry_value(0, 0, 7.0);
    TEST_ASSERT(matrix.get_entry_value(0, 0)==7.0);

    matrix.change_entry_value(2, 0, complex<double>(1.0, 7.0));
    TEST_ASSERT(matrix.get_entry_value(2, 0)==complex<double>(1.0,7.0));
}

void SPARSE_MATRIX_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    matrix.clear();

    test_constructor();
}

void SPARSE_MATRIX_TEST::test_get_reorder_permutation()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    // original matrix
    // [1  1  1  1]
    // [1  1  1  0]
    // [1  1  1  0]
    // [1  0  0  1]
    // ordered should be
    // [1  0  0  1]
    // [0  1  1  1]
    // [0  1  1  1]
    // [1  1  1  1]
    // permutation should be
    // [3 1 2 0] or [3 2 1 0]
    // however, the actual result is
    // [1 2 3 0]
    matrix.add_entry(0,0, 1.0);
    matrix.add_entry(0,1, 1.0);
    matrix.add_entry(0,2, 1.0);
    matrix.add_entry(0,3, 1.0);
    matrix.add_entry(1,0, 1.0);
    matrix.add_entry(1,1, 1.0);
    matrix.add_entry(1,2, 1.0);
    matrix.add_entry(2,0, 1.0);
    matrix.add_entry(2,1, 1.0);
    matrix.add_entry(2,2, 1.0);
    matrix.add_entry(3,0, 1.0);
    matrix.add_entry(3,3, 1.0);

    matrix.compress_and_merge_duplicate_entries();

    vector<unsigned int> perm = matrix.get_reorder_permutation();
    // only the last one is checked. the cs_amd is approximate
    // others are not tested.

    //cout<<"perm : "<<perm[0]<<", "<<perm[1]<<", "<<perm[2]<<", "<<perm[3]<<endl;
    TEST_ASSERT(perm[3] == 0);
}


void SPARSE_MATRIX_TEST::test_slove_Ax_equal_b()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");
    ostringstream osstream;
    osstream<<"LU_factorization is also tested.";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

    prepare_basic_matrix();

    vector<double> b;
    b.reserve(3);
    b.push_back(2.0);
    b.push_back(20.0);
    b.push_back(5.0);

    matrix.LU_factorization();
    vector<double> x = matrix.solve_Ax_eq_b(b);

    TEST_ASSERT(fabs(x[0] - 2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(x[1] - 4.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(x[2] - 3.0)<FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_solve_Ax_equal_b_with_operator_slash()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    vector<double> b;
    b.reserve(3);
    b.push_back(2.0);
    b.push_back(20.0);
    b.push_back(5.0);

    vector<double> x  = b/matrix;

    TEST_ASSERT(fabs(x[0] - 2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(x[1] - 4.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(x[2] - 3.0)<FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix;
    newmatrix = matrix;  // here , operator = is called

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(newmatrix.get_entry_value(0,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(0,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(0,2)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(1,0)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(1,1)==complex<double>(2.0, 5.0));
    TEST_ASSERT(newmatrix.get_entry_value(1,2)==complex<double>(4.0, 9.0));
    TEST_ASSERT(newmatrix.get_entry_value(2,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(2,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(2,2)==complex<double>(1.0, 2.0));
}

void SPARSE_MATRIX_TEST::test_copy_with_copy_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = matrix; //here, copy constructor is called

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(newmatrix.get_entry_value(0,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(0,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(0,2)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(1,0)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(1,1)==complex<double>(2.0, 5.0));
    TEST_ASSERT(newmatrix.get_entry_value(1,2)==complex<double>(4.0, 9.0));
    TEST_ASSERT(newmatrix.get_entry_value(2,0)==complex<double>(1.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(2,1)==complex<double>(0.0, 0.0));
    TEST_ASSERT(newmatrix.get_entry_value(2,2)==complex<double>(1.0, 2.0));
}

void SPARSE_MATRIX_TEST::test_matrix_add()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = matrix+matrix;

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(newmatrix.get_real_entry_value(0,0)==2*matrix.get_real_entry_value(0,0));
    TEST_ASSERT(newmatrix.get_real_entry_value(0,1)==2*matrix.get_real_entry_value(0,1));
    TEST_ASSERT(newmatrix.get_real_entry_value(0,2)==2*matrix.get_real_entry_value(0,2));
    TEST_ASSERT(newmatrix.get_real_entry_value(1,0)==2*matrix.get_real_entry_value(1,0));
    TEST_ASSERT(newmatrix.get_real_entry_value(1,1)==2*matrix.get_real_entry_value(1,1));
    TEST_ASSERT(newmatrix.get_real_entry_value(1,2)==2*matrix.get_real_entry_value(1,2));
    TEST_ASSERT(newmatrix.get_real_entry_value(2,0)==2*matrix.get_real_entry_value(2,0));
    TEST_ASSERT(newmatrix.get_real_entry_value(2,1)==2*matrix.get_real_entry_value(2,1));
    TEST_ASSERT(newmatrix.get_real_entry_value(2,2)==2*matrix.get_real_entry_value(2,2));
}

void SPARSE_MATRIX_TEST::test_matrix_minus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = matrix-matrix;

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(newmatrix.get_real_entry_value(0,0)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(0,1)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(0,2)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(1,0)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(1,1)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(1,2)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(2,0)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(2,1)==0.0);
    TEST_ASSERT(newmatrix.get_real_entry_value(2,2)==0.0);
}

void SPARSE_MATRIX_TEST::test_matrix_multiply()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = matrix*matrix;

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-12)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,0)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,2)-1)<=FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_matrix_divide()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = 3/matrix;

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-3)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-6)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-1.5)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-(-6))<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,0)-(-3))<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,2)-3)<=FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_matrix_inverse()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = inv(matrix);

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-0.5)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-(-2))<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,0)-(-1))<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,2)-1)<=FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_concatenate_matrix_diagnally()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    vector<STEPS_SPARSE_MATRIX*> matrixz;
    matrixz.push_back(&matrix);
    matrixz.push_back(&matrix);
    matrixz.push_back(&matrix);

    STEPS_SPARSE_MATRIX newmatrix = concatenate_matrix_diagnally(matrixz);

    TEST_ASSERT(newmatrix.get_matrix_size()==9);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,3)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,4)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,5)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,6)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,7)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,8)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,3)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,4)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,5)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,6)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,7)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,8)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,2)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,3)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,4)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,5)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,6)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,7)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,8)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(3,3)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(3,4)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(3,5)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(4,3)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(4,4)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(4,5)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(5,3)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(5,4)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(5,5)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(6,6)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(6,7)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(6,8)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(7,6)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(7,7)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(7,8)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(8,6)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(8,7)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(8,8)-1)<=FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_build_identity_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = build_identity_matrix(matrix);

    TEST_ASSERT(newmatrix.get_matrix_size()==3);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,2)-1)<=FLOAT_EPSILON);

    STEPS_SPARSE_MATRIX newmatrix2 = build_identity_matrix(3);

    TEST_ASSERT(newmatrix2.get_matrix_size()==3);

    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(1,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(1,1)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(1,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(2,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix2.get_real_entry_value(2,2)-1)<=FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_expand_matrix_to_new_size()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = expand_matrix_to_new_size(matrix,3,5);

    TEST_ASSERT(newmatrix.get_matrix_size()==5);
    TEST_ASSERT(newmatrix.get_matrix_row_count()==3);
    TEST_ASSERT(newmatrix.get_matrix_column_count()==5);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(2,2)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(3,3)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(4,4)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(4,3)-0)<=FLOAT_EPSILON);

    STEPS_SPARSE_MATRIX B = matrix*newmatrix;
    newmatrix.change_entry_value(2,4,2);
    B = matrix*newmatrix;

    TEST_ASSERT(B.get_matrix_row_count()==3);
    TEST_ASSERT(B.get_matrix_column_count()==5);
}

void SPARSE_MATRIX_TEST::test_shrink_matrix_to_new_size()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = shrink_matrix_to_new_size(matrix,2,3);

    TEST_ASSERT(newmatrix.get_matrix_size()==3);
    TEST_ASSERT(newmatrix.get_matrix_row_count()==2);
    TEST_ASSERT(newmatrix.get_matrix_column_count()==3);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-4)<=FLOAT_EPSILON);
}

void SPARSE_MATRIX_TEST::test_change_matrix_to_new_size()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    STEPS_SPARSE_MATRIX newmatrix = change_matrix_to_new_size(matrix,2,4);

    TEST_ASSERT(newmatrix.get_matrix_size()==4);
    TEST_ASSERT(newmatrix.get_matrix_row_count()==2);
    TEST_ASSERT(newmatrix.get_matrix_column_count()==4);

    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,0)-1)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,1)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,2)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(0,3)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,0)-0)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,1)-2)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,2)-4)<=FLOAT_EPSILON);
    TEST_ASSERT(fabs(newmatrix.get_real_entry_value(1,3)-0)<=FLOAT_EPSILON);

    STEPS_SPARSE_MATRIX B = change_matrix_to_new_size(matrix,4,2);

    TEST_ASSERT(B.get_matrix_size()==2);
    TEST_ASSERT(B.get_matrix_row_count()==4);
    TEST_ASSERT(B.get_matrix_column_count()==2);
}

void SPARSE_MATRIX_TEST::test_save_matrix_to_file()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SPARSE_MATRIX_TEST");

    prepare_basic_matrix();

    // matrix is
    // [1  0  0]      [0  0  0]
    // [0  2  4]  + j [0  5  9]
    // [1  0  1]      [0  0  2]

    matrix.save_matrix_to_file("test_log/sparse_matrix_contents.csv");
}

#endif
