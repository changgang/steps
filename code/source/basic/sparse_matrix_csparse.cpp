#include "header/basic/sparse_matrix_csparse.h"
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

SPARSE_MATRIX_CSPARSE::SPARSE_MATRIX_CSPARSE():SPARSE_MATRIX()
{
    //constructor

    matrix_real = NULL;
    matrix_imag = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;

    clear();
}

SPARSE_MATRIX_CSPARSE::SPARSE_MATRIX_CSPARSE(const SPARSE_MATRIX_CSPARSE& matrix)
{
    matrix_real = NULL;
    matrix_imag = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;

    clear();

    copy_from_const_matrix(matrix);
}

SPARSE_MATRIX_CSPARSE& SPARSE_MATRIX_CSPARSE::operator=(const SPARSE_MATRIX_CSPARSE& matrix)
{
    if(this==(&matrix)) return *this;

    copy_from_const_matrix(matrix);

    return *this;
}

void SPARSE_MATRIX_CSPARSE::copy_from_const_matrix(const SPARSE_MATRIX_CSPARSE& matrix)
{
    clear();

    int nz = matrix.get_matrix_entry_count();

    complex<double> value;
    int row, col;
    for(int k=0; k!=nz; ++k)
    {
        row = matrix.get_row_number_of_entry_index(k);
        col = matrix.get_column_number_of_entry_index(k);
        value = matrix.get_entry_value(k);

        add_entry(row, col, value);
    }

    //os<<"done copying matrix, nz = %d", nz);
    //show_information_with_leading_time_stamp(osstream);

    if(matrix.matrix_in_compressed_column_form())
        compress_and_merge_duplicate_entries();

    //os<<"done compressing matrix");
    //show_information_with_leading_time_stamp(osstream);

    update_clock_when_matrix_is_changed();
}


SPARSE_MATRIX_CSPARSE::~SPARSE_MATRIX_CSPARSE()
{
    // destructor
    if(matrix_real!=NULL) cs_spfree(matrix_real);
    if(matrix_imag!=NULL) cs_spfree(matrix_imag);
    if(LU!=NULL)          cs_nfree(LU);
    if(LU_symbolic!=NULL) cs_sfree(LU_symbolic);
    if(LU_workspace!=NULL) free(LU_workspace);

    matrix_real = NULL;
    matrix_imag = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;
}

void SPARSE_MATRIX_CSPARSE::clear()
{
    // clear function
    update_clock_when_LU_factorization_is_performed();

    if(matrix_real!=NULL) cs_spfree(matrix_real);
    if(matrix_imag!=NULL) cs_spfree(matrix_imag);
    if(LU!=NULL)          cs_nfree(LU);
    if(LU_symbolic!=NULL) cs_sfree(LU_symbolic);
    if(LU_workspace!=NULL) free(LU_workspace);

    matrix_real = NULL;
    matrix_imag = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;

    matrix_real = cs_spalloc(1,1,1,1,1); // arguments: row = 1, column = 1, max_entry = 1, allocate_memory = 1, triplet_form = 1
    matrix_imag = cs_spalloc(1,1,1,1,1);

    update_clock_when_matrix_is_changed();
}

bool SPARSE_MATRIX_CSPARSE::matrix_in_triplet_form() const
{
    if(matrix_real->nz != -1) return true;
    else return false;
}


void SPARSE_MATRIX_CSPARSE::add_entry(int row, int col, complex<double> value)
{
    if(matrix_in_triplet_form())
    {
        cs_entry(matrix_real, row, col, value.real());
        cs_entry(matrix_imag, row, col, value.imag());

        update_clock_when_matrix_is_changed();
    }
    else
    {
        int index = get_entry_index(row, col);
        if(index != INDEX_NOT_EXIST)
        {
            complex<double> current_value = get_entry_value(index);
            change_entry_value(index, current_value+value);
        }
        else
        {
            convert_to_triplet_form();
            cs_entry(matrix_real, row, col, value.real());
            cs_entry(matrix_imag, row, col, value.imag());
            compress_and_merge_duplicate_entries();
        }
    }
}

void SPARSE_MATRIX_CSPARSE::convert_to_triplet_form()
{
    if(matrix_in_triplet_form())
        return;

    cs *mat_real; // temp mat
    cs *mat_imag; // temp mat
    csi *tempi; // temp index
    double *tempd; // temp value

    // copy
    mat_real = cs_spalloc(1,1,1,1,1);
    mat_imag = cs_spalloc(1,1,1,1,1);
    int n = get_matrix_entry_count();
    int row, col;
    for(int k=0; k!=n; ++k)
    {
        row = get_row_number_of_entry_index(k);
        col = get_column_number_of_entry_index(k);
        cs_entry(mat_real, row, col, get_real_entry_value(k));
        cs_entry(mat_imag, row, col, get_imag_entry_value(k));
    }
    // swap
    matrix_real->nzmax = mat_real->nzmax;
    matrix_real->m = mat_real->m;
    matrix_real->n = mat_real->n;
    matrix_real->nz = mat_real->nz;
    tempi = matrix_real->p; matrix_real->p = mat_real->p; mat_real->p = tempi;
    tempi = matrix_real->i; matrix_real->i = mat_real->i; mat_real->i = tempi;
    tempd = matrix_real->x; matrix_real->x = mat_real->x; mat_real->x = tempd;
    // free temp mat
    cs_spfree(mat_real);

    // swap
    matrix_imag->nzmax = mat_imag->nzmax;
    matrix_imag->m = mat_imag->m;
    matrix_imag->n = mat_imag->n;
    matrix_imag->nz = mat_imag->nz;
    tempi = matrix_imag->p; matrix_imag->p = mat_imag->p; mat_imag->p = tempi;
    tempi = matrix_imag->i; matrix_imag->i = mat_imag->i; mat_imag->i = tempi;
    tempd = matrix_imag->x; matrix_imag->x = mat_imag->x; mat_imag->x = tempd;
    // free temp mat
    cs_spfree(mat_imag);

    update_clock_when_matrix_is_changed();
}

void SPARSE_MATRIX_CSPARSE::compress_and_merge_duplicate_entries()
{
    // compress the sparse matrix
    if(matrix_in_compressed_column_form()) return;

    cs *mat; // temp mat
    csi *tempi; // temp index
    double *tempd; // temp value

    // real part
    // compress the matrix
    mat = cs_compress(matrix_real);
    // swap
    matrix_real->nzmax = mat->nzmax;
    matrix_real->m = mat->m;
    matrix_real->n = mat->n;
    matrix_real->nz = mat->nz;
    tempi = matrix_real->p; matrix_real->p = mat->p; mat->p = tempi;
    tempi = matrix_real->i; matrix_real->i = mat->i; mat->i = tempi;
    tempd = matrix_real->x; matrix_real->x = mat->x; mat->x = tempd;
    // free temp mat
    cs_spfree(mat);

    cs_dupl(matrix_real); // merge duplicate entries

    //  imaginary part
    // compress the matrix
    mat = cs_compress(matrix_imag);
    // swap
    matrix_imag->nzmax = mat->nzmax;
    matrix_imag->m = mat->m;
    matrix_imag->n = mat->n;
    matrix_imag->nz = mat->nz;
    tempi = matrix_imag->p; matrix_imag->p = mat->p; mat->p = tempi;
    tempi = matrix_imag->i; matrix_imag->i = mat->i; mat->i = tempi;
    tempd = matrix_imag->x; matrix_imag->x = mat->x; mat->x = tempd;
    // free temp mat
    cs_spfree(mat);

    cs_dupl(matrix_imag); // merge duplicate entries

    // at last, transpose twice
    transpose();
    transpose();

    update_clock_when_matrix_is_changed();
}

void SPARSE_MATRIX_CSPARSE::transpose()
{
    // transpose a compress format matrix
    if(matrix_in_triplet_form()) // if in triplet format, convert to compressed format
        compress_and_merge_duplicate_entries(); // convert


    cs *mat; // temp mat
    csi *tempi; // temp index
    double *tempd; // temp value

    // transpose real part
    mat = cs_transpose(matrix_real, 1);
    // swap
    matrix_real->nzmax = mat->nzmax;
    matrix_real->m = mat->m;
    matrix_real->n = mat->n;
    matrix_real->nz = mat->nz;
    tempi = matrix_real->p; matrix_real->p = mat->p; mat->p = tempi;
    tempi = matrix_real->i; matrix_real->i = mat->i; mat->i = tempi;
    tempd = matrix_real->x; matrix_real->x = mat->x; mat->x = tempd;
    // free temp mat
    cs_spfree(mat);


    // transpose imaginary part
    mat = cs_transpose(matrix_imag, 1);
    // swap
    matrix_imag->nzmax = mat->nzmax;
    matrix_imag->m = mat->m;
    matrix_imag->n = mat->n;
    matrix_imag->nz = mat->nz;
    tempi = matrix_imag->p; matrix_imag->p = mat->p; mat->p = tempi;
    tempi = matrix_imag->i; matrix_imag->i = mat->i; mat->i = tempi;
    tempd = matrix_imag->x; matrix_imag->x = mat->x; mat->x = tempd;
    // free temp mat
    cs_spfree(mat);

    update_clock_when_matrix_is_changed();
}

int SPARSE_MATRIX_CSPARSE::get_matrix_size() const
{
    if(matrix_real!=NULL) return matrix_real->n;
    else                  return 0;
}

int SPARSE_MATRIX_CSPARSE::get_matrix_entry_count() const
{
    return matrix_real->nzmax;
}

int SPARSE_MATRIX_CSPARSE::get_starting_index_of_column(int col) const
{
    if(col>=0 and col<=get_matrix_size()) return matrix_real->p[col];
    else                                  return INDEX_NOT_EXIST;
}

int SPARSE_MATRIX_CSPARSE::get_row_number_of_entry_index(int index) const
{
    int n = get_matrix_size();
    if(index<=get_starting_index_of_column(n))  return matrix_real->i[index];
    else                                        return INDEX_NOT_EXIST;
}

int SPARSE_MATRIX_CSPARSE::get_entry_index(int row, int col) const
{
    // return entry index of compressed matrix

    if(row >= 0 and col >= 0)
    {
        if(not matrix_in_compressed_column_form())
        {
            ostringstream osstream;
            osstream<<"Error. Sparse matrix not in compressed form when getting entry index.\nINDEX_NOT_EXIST will be returned.";
            show_information_with_leading_time_stamp(osstream);
            return INDEX_NOT_EXIST;
        }

        int index = INDEX_NOT_EXIST;

        if(row < get_matrix_size() and col < get_matrix_size())
        {
            for(int k=matrix_real->p[col]; k!=matrix_real->p[col+1]; ++k)
            {
                if(matrix_real->i[k] > row) break; // if no entry of (row, col)
                if(matrix_real->i[k] == row)
                {
                    index = k;
                    break;
                }
            }
        }
        return index;
    }
    else
        return INDEX_NOT_EXIST;
}


double SPARSE_MATRIX_CSPARSE::get_real_entry_value(int index) const
{
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return matrix_real->x[index];
    else
        return 0.0;
}

double SPARSE_MATRIX_CSPARSE::get_imag_entry_value(int index) const
{
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return matrix_imag->x[index];
    else
        return 0.0;
}

void SPARSE_MATRIX_CSPARSE::change_real_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        matrix_real->x[index]=value; // found

        update_clock_when_matrix_is_changed();
    }
}


void SPARSE_MATRIX_CSPARSE::change_imag_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        matrix_imag->x[index]=value; // found

        update_clock_when_matrix_is_changed();
    }
}


vector<size_t> SPARSE_MATRIX_CSPARSE::get_reorder_permutation()
{
    csi* p = cs_amd(1,matrix_real); // use the real part for re-ordering

    vector<size_t> permutation;
    permutation.reserve(get_matrix_size());
    int n = get_matrix_size();
    for(int i = 0; i!=n; ++i)
        permutation.push_back(size_t(p[i]));

    free(p);

    return permutation;
}

void SPARSE_MATRIX_CSPARSE::LU_factorization(int order, double tolerance)
{
    //if(LU_factorization_is_performed()) return;

    char buffer[256];

    cs_nfree(LU); // free LU (csn *)
    cs_sfree(LU_symbolic);
    csi n, ok ;
    if (!CS_CSC (matrix_real))/* check inputs */
    {
        snprintf(buffer, 256, "No real matrix is set for LU factorization. Return. (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    n = matrix_real->n ;
    LU_symbolic = cs_sqr (order, matrix_real, 0) ;              /* ordering and symbolic analysis */
    LU = cs_lu (matrix_real, LU_symbolic, tolerance) ;                 /* numeric LU factorization */
    ok = (LU_symbolic && LU) ; // check
    if(ok!=1)
    {
        snprintf(buffer, 256, "ALERT!  LU factorization for sparse matrix(CSparse) failed! (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    // reset working space
    if(LU_workspace!=NULL) free(LU_workspace);
    LU_workspace = (double *)cs_malloc(n, sizeof (double)) ;  // get workspace
    if(LU_workspace == NULL)
    {
        snprintf(buffer, 256, "Global variable LU_workspace was not successfully set. No workspace for LU solution. (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp(buffer);
    }

    update_clock_when_LU_factorization_is_performed(); // mark the clock when finish the LU decomposition
    return;
}

vector<double> SPARSE_MATRIX_CSPARSE::solve_Ax_eq_b(vector<double>& b)
{
    if(not LU_factorization_is_performed())
        LU_factorization(1, 1e-6);

    solve_Lx_eq_b(b);

    solve_xU_eq_b(b);

    return b;
}

void SPARSE_MATRIX_CSPARSE::solve_Lx_eq_b(vector<double>& b)
{
    ostringstream osstream;
    size_t n = b.size();
    double* bb = (double*)malloc(n*sizeof(double));
    if(bb==NULL)
    {
        osstream<<"Error. Failed to allocate temporary array for solving Lx=b.(function "<<__FUNCTION__<<" in file "<<__FILE__;
        show_information_with_leading_time_stamp(osstream);

        return; // failed to allocate array bb
    }

    for(size_t i=0; i!=n; ++i) bb[i]=b[i]; // set bb

    cs_ipvec(LU->pinv, bb, LU_workspace, matrix_real->n) ;       /* x = b(p) */
    int OK = cs_lsolve (LU->L, LU_workspace) ;               /* x = L\x */
    if(OK != 1)
    {
        osstream<<"Error. Failed to solve Lx=b.(function "<<__FUNCTION__<<" in file "<<__FILE__;
        show_information_with_leading_time_stamp(osstream);
        return; // failed to solve equation
    }
    // now solution is OK
    for(size_t i=0; i!=n; ++i) b[i]=LU_workspace[i]; // now reset b with bb

    free(bb);

    return;
}

void SPARSE_MATRIX_CSPARSE::solve_xU_eq_b(vector<double>& b)
{
    string buffer;
    char cbuffer[1000];

    size_t n = b.size();
    double* bb = (double*)malloc(n*sizeof(double));
    if(bb==NULL)
    {
        snprintf(cbuffer, 1000, "Error. Failed to allocate temporary array for solving xU=b.(function %s in file %s)\n",__FUNCTION__,__FILE__);
        buffer = cbuffer;
        show_information_with_leading_time_stamp(buffer);
        return; // failed to allocate array bb
    }
    for(size_t i=0; i!=n; ++i) LU_workspace[i]=b[i]; // set bb
    int OK = cs_usolve(LU->U, LU_workspace) ;               /* x = U\x */
    cs_ipvec(LU_symbolic->q, LU_workspace, bb, matrix_real->n) ;          /* b(q) = x */

    if(OK != 1)
    {
        snprintf(cbuffer, 1000, "Error. Failed to solve xU=b.(function %s in file %s)\n",__FUNCTION__,__FILE__);
        buffer = cbuffer;
        show_information_with_leading_time_stamp(buffer);
        return; // failed to solve equation
    }
    // now solution is OK
    for(size_t i=0; i!=n; ++i) b[i]=bb[i]; // now reset b with bb

    free(bb);

    return;
}


void SPARSE_MATRIX_CSPARSE::report_brief() const
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="matrix entries:\n";
    show_information_with_leading_time_stamp(buffer);

	snprintf(cbuffer, 1000, "row   column     real       imaginary\n");
    show_information_with_leading_time_stamp(cbuffer);

    int n = get_matrix_size();
    for(j=0;j!=n;++j)
    {
        for(k=matrix_real->p[j];k<matrix_real->p[j+1];++k)
        {
            i=matrix_real->i[k];

            if(matrix_imag->n==0)
                snprintf(cbuffer,1000, "%-6d, %-6d, % 10.6f\n",i,j,matrix_real->x[k]);
            else
                snprintf(cbuffer,1000, "%-6d, %-6d, % 10.6f, % 10.6f\n",i,j,matrix_real->x[k],matrix_imag->x[k]);

            buffer = cbuffer;
            show_information_with_leading_time_stamp(buffer);
        }
    }
}


void SPARSE_MATRIX_CSPARSE::report_full() const
{
    string buffer;
}

void SPARSE_MATRIX_CSPARSE::save_matrix_to_file(string filename) const
{
    ostringstream osstream;

    ofstream file;
    file.open(filename);

    if(not file.is_open())
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving sparse matrix(CSparse) contents."<<endl
          <<"No sparse matrix(CSparse) will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    int i=0, j=0, k=0;

	file<<"ROW,COLUMN,REAL,IMAGINARY"<<endl;

	int n = get_matrix_size();
    for(j=0;j!=n;++j)
    {
        for(k=matrix_real->p[j];k<matrix_real->p[j+1];++k)
        {
            i=matrix_real->i[k];

            if(matrix_imag->n==0)
                file<<i<<","<<j<<","
                    <<setprecision(6)<<fixed<<matrix_real->x[k]<<",0.0"<<endl;
            else
                file<<i<<","<<j<<","
                    <<setprecision(6)<<fixed<<matrix_real->x[k]<<","
                    <<setprecision(6)<<fixed<<matrix_imag->x[k]<<endl;
        }
    }
    file.close();
}

vector<double> operator/(vector<double>&b, SPARSE_MATRIX_CSPARSE& A)
{
    return A.solve_Ax_eq_b(b);
}
