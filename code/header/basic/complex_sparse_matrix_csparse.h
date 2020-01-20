#ifndef COMPLEX_SPARSE_MATRIX_CSPARSE_H
#define COMPLEX_SPARSE_MATRIX_CSPARSE_H

#include "cxs.h"

#include <vector>
#include <ctime>
#include <complex>
#include "complex_sparse_matrix.h"
using namespace std;

class COMPLEX_SPARSE_MATRIX_CSPARSE : public COMPLEX_SPARSE_MATRIX
{
public:
    COMPLEX_SPARSE_MATRIX_CSPARSE();
    COMPLEX_SPARSE_MATRIX_CSPARSE(const COMPLEX_SPARSE_MATRIX_CSPARSE& matrix);
    virtual COMPLEX_SPARSE_MATRIX_CSPARSE& operator=(const COMPLEX_SPARSE_MATRIX_CSPARSE& matrix);
    virtual ~COMPLEX_SPARSE_MATRIX_CSPARSE();

    virtual void add_entry(int row, int col, const complex<double>& value);

    virtual void convert_to_triplet_form();

    virtual bool matrix_in_triplet_form()  const;

    virtual void compress_and_merge_duplicate_entries();

    virtual void transpose();

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

    virtual vector<complex<double> >& solve_Ax_eq_b(vector<complex<double> >& b);

    virtual void report_brief()  const;
    virtual void report_full()  const;
    virtual void save_matrix_to_file(string filename)  const;

private:
    void solve_Lx_eq_b(vector<complex<double> >& b);
    void solve_xU_eq_b(vector<complex<double> >& b);
    void copy_from_const_matrix(const COMPLEX_SPARSE_MATRIX_CSPARSE& matrix);
    cxs     *matrix_complex;
    cxsn    *LU;
    cxss    *LU_symbolic;
    complex<double> *LU_workspace;
    complex<double> *bb;
    unsigned int bb_size;
};


vector<complex<double> >& operator/(vector<complex<double> >&b, COMPLEX_SPARSE_MATRIX_CSPARSE& A);
#endif // COMPLEX_SPARSE_MATRIX_CSPARSE_H
