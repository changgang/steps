#ifndef SPARSE_MATRIX_UMFPACK_H
#define SPARSE_MATRIX_UMFPACK_H

extern "C"
{
    #include "cs.h"
}
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
    virtual ~SPARSE_MATRIX_UMFPACK();

    void add_entry(int row, int col, double value);
    void add_entry(int row, int col, complex<double> value);

    bool matrix_in_triplet_form() const;
    bool matrix_in_compressed_column_form() const;

    void compress_and_merge_duplicate_entries();

    void transpose();

    int get_matrix_size() const;
    int get_matrix_entry_count() const;
    int get_starting_index_of_column(int col) const;
    int get_row_number_of_entry_index(int index) const;
    int get_column_number_of_entry_index(int index) const;

    int    get_entry_index(int row, int col) const;
    complex<double> get_entry_value(int row, int col) const;
    complex<double> get_entry_value(int index) const;

    void change_entry_value(int row, int col, double value);
    void change_entry_value(int row, int col, complex<double> value);

    void change_entry_value(int index, double value);
    void change_entry_value(int index, complex<double> value);

    void clear();

    vector<size_t> get_reorder_permutation();

    void LU_factorization(int order=1, double tolerance = 1e-6);

    vector<double> solve_Ax_eq_b(vector<double> b);

    void report_brief() const;
    void report_full() const;
    void save_matrix_to_file(string filename) const;

    //SPARSE_MATRIX_UMFPACK& operator=(const SPARSE_MATRIX_UMFPACK& matrix);
    virtual SPARSE_MATRIX_UMFPACK& operator=(const SPARSE_MATRIX_UMFPACK& matrix);
private:
    double get_real_entry_value(int index) const;
    double get_imag_entry_value(int index) const;
    void change_real_entry_value(int index, double value);
    void change_imag_entry_value(int index, double value);

    void convert_to_triplet_form();

    bool LU_factorization_is_performed() const;
    void solve_Lx_eq_b(vector<double>& b);
    void solve_xU_eq_b(vector<double>& b);



    cs     *matrix_real; // real components
    cs     *matrix_imag; // imaginary components , not used for real matrix
    csn    *LU;
    css    *LU_symbolic;
    double *LU_workspace;
};


vector<double> operator/(vector<double>b, SPARSE_MATRIX_UMFPACK& A);
#endif // SPARSE_MATRIX_UMFPACK_H
