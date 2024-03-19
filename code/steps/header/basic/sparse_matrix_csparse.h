#ifndef SPARSE_MATRIX_CSPARSE_H
#define SPARSE_MATRIX_CSPARSE_H

extern "C"
{
    #include "cs.h"
}
#include <vector>
#include <ctime>
#include <complex>
#include "sparse_matrix.h"
using namespace std;

class SPARSE_MATRIX_CSPARSE : public SPARSE_MATRIX
{
public:
    SPARSE_MATRIX_CSPARSE();
    SPARSE_MATRIX_CSPARSE(const SPARSE_MATRIX_CSPARSE& matrix);
    virtual SPARSE_MATRIX_CSPARSE& operator=(const SPARSE_MATRIX_CSPARSE& matrix);
    virtual ~SPARSE_MATRIX_CSPARSE();

    virtual void add_entry(int row, int col, const complex<double>& value);

    virtual void convert_to_triplet_form();

    virtual bool matrix_in_triplet_form()  const;

    virtual void compress_and_merge_duplicate_entries();
    void compress_and_merge_cs_matrix(cs* matrix);

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
    virtual double get_real_entry_value(int row, int col)  const;
    virtual double get_imag_entry_value(int row, int col)  const;

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
public:
    cs* get_cs_real_matrix();
    cs* get_cs_imag_matrix();
    void set_cs_real_matrix(cs* matrix);
    void set_cs_imag_matrix(cs* matrix);
private:
    void solve_Lx_eq_b(vector<double>& b);
    void solve_xU_eq_b(vector<double>& b);
    void copy_from_const_matrix(const SPARSE_MATRIX_CSPARSE& matrix);
    cs     *matrix_real; // real components
    cs     *matrix_imag; // imaginary components , not used for real matrix
    csn    *LU;
    css    *LU_symbolic;
    double *LU_workspace;
    double* bb;
    unsigned int bb_size;

    bool add_entry_callled;
};


vector<double>& operator/(vector<double>&b, SPARSE_MATRIX_CSPARSE& A);
SPARSE_MATRIX_CSPARSE operator+(SPARSE_MATRIX_CSPARSE&A, SPARSE_MATRIX_CSPARSE& B);
SPARSE_MATRIX_CSPARSE operator-(SPARSE_MATRIX_CSPARSE&A, SPARSE_MATRIX_CSPARSE& B);
SPARSE_MATRIX_CSPARSE operator*(SPARSE_MATRIX_CSPARSE&A, SPARSE_MATRIX_CSPARSE& B);
SPARSE_MATRIX_CSPARSE operator/(double b, SPARSE_MATRIX_CSPARSE& A);
SPARSE_MATRIX_CSPARSE inv(SPARSE_MATRIX_CSPARSE&A);
SPARSE_MATRIX_CSPARSE concatenate_matrix_diagnally(vector<SPARSE_MATRIX_CSPARSE*> matrix);
SPARSE_MATRIX_CSPARSE build_identity_matrix(const SPARSE_MATRIX_CSPARSE&A);
SPARSE_MATRIX_CSPARSE build_identity_matrix(unsigned int n);
SPARSE_MATRIX_CSPARSE change_matrix_to_new_size(const SPARSE_MATRIX_CSPARSE&A, unsigned int mrow, unsigned int ncol);
SPARSE_MATRIX_CSPARSE expand_matrix_to_new_size(const SPARSE_MATRIX_CSPARSE&A, unsigned int mrow, unsigned int ncol);
SPARSE_MATRIX_CSPARSE shrink_matrix_to_new_size(const SPARSE_MATRIX_CSPARSE&A, unsigned int mrow, unsigned int ncol);
#endif // SPARSE_MATRIX_CSPARSE_H
