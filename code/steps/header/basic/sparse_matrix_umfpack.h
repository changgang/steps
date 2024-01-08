#ifndef SPARSE_MATRIX_UMFPACK_H
#define SPARSE_MATRIX_UMFPACK_H
#include <vector>
#include <ctime>
#include <complex>
#include "sparse_matrix.h"
using namespace std;

class SPARSE_MATRIX_UMFPACK : public SPARSE_MATRIX
{
public:
    SPARSE_MATRIX_UMFPACK();
    SPARSE_MATRIX_UMFPACK(const SPARSE_MATRIX_UMFPACK& matrix);
    virtual SPARSE_MATRIX_UMFPACK& operator=(const SPARSE_MATRIX_UMFPACK& matrix);
    virtual ~SPARSE_MATRIX_UMFPACK();

    virtual void add_entry(int row, int col, const complex<double>& value);

    virtual void convert_to_triplet_form();

    virtual bool matrix_in_triplet_form()  const;

    virtual void compress_and_merge_duplicate_entries();

    virtual void transpose();

    virtual int get_matrix_row_count()  const;
    virtual int get_matrix_column_count()  const;
    virtual int get_matrix_size()  const;
    virtual int get_matrix_entry_count()  const;
    virtual int get_starting_index_of_column(int col)  const;
    virtual int get_row_number_of_entry_index(int index)  const;

    virtual int    get_entry_index(int row, int col)  const;

    virtual complex<double> get_complex_entry_value(int index)  const;
    virtual double get_real_entry_value(int index)  const;
    virtual double get_imag_entry_value(int index)  const;

    virtual void change_real_entry_value(int index, double value);
    virtual void change_imag_entry_value(int index, double value);

    virtual void clear();

    virtual vector<unsigned int> get_reorder_permutation();

    virtual void LU_factorization(int order=1, double tolerance = 1e-13);
    virtual vector<double>& solve_Ax_eq_b(vector<double>& b);

    virtual void report_brief()  const;
    virtual void report_full()  const;
    virtual void save_matrix_to_file(string filename)  const;

    virtual unsigned int get_memory_usage_in_bytes();
private:
    void copy_from_const_matrix(const SPARSE_MATRIX_UMFPACK& matrix);
    vector<unsigned int> triplet_row_index, triplet_column_index;
    vector<double> triplet_matrix_real, triplet_matrix_imag;
    unsigned int n_row, n_column;

    int *compressed_column_starting_index, *compressed_row_index;
    double *compressed_matrix_real, *compressed_matrix_imag;

    bool flag_matrix_in_triplet_form;

    void *Symbolic, *Numeric ;
};


vector<double>& operator/(vector<double>& b, SPARSE_MATRIX_UMFPACK& A);
#endif // SPARSE_MATRIX_UMFPACK_H
