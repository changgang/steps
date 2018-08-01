#include "header/basic/sparse_matrix.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;

SPARSE_MATRIX::SPARSE_MATRIX()
{
    //constructor
    clock_when_LU_factorization_is_performed = clock();

    matrix_real = cs_spalloc(1,1,1,1,1); // arguments: row = 1, column = 1, max_entry = 1, allocate_memory = 1, triplet_form = 1
    matrix_imag = cs_spalloc(1,1,1,1,1);
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;

    clock_when_matrix_is_changed = clock();
}

SPARSE_MATRIX::SPARSE_MATRIX(const SPARSE_MATRIX& matrix)
{
    clock_when_LU_factorization_is_performed = clock();

    matrix_real = cs_spalloc(1,1,1,1,1); // arguments: row = 1, column = 1, max_entry = 1, allocate_memory = 1, triplet_form = 1
    matrix_imag = cs_spalloc(1,1,1,1,1);
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;

    int n = matrix.get_matrix_entry_count();
    complex<double> value;
    int row, col;
    for(int k=0; k!=n; ++k)
    {
        row = matrix.get_row_number_of_entry_index(k);
        col = matrix.get_column_number_of_entry_index(k);
        value = matrix.get_entry_value(k);

        this->add_entry(row, col, value);
    }
    if(matrix.matrix_in_compressed_column_form())
        this->compress_and_merge_duplicate_entries();

    clock_when_matrix_is_changed = clock();
}

SPARSE_MATRIX::~SPARSE_MATRIX()
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

    clock_when_matrix_is_changed = clock();
}

void SPARSE_MATRIX::clear()
{
    // clear function
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

/*
    // when perform clear, cs_spfree is not required. only the following parameters need to be reset
    // without the cs_spfree(), the allocated memory for matrix_real and matrix_imag will be not freed to save time
    matrix_real->m = 1;// m: number of rows
    matrix_real->n = 1;//n: number of columns
    matrix_real->nz = 0; // nz:set as 0 to indicate triplet form, and the number of entries is 0
    matrix_real->nzmax = 1; // nzmax: number of max entries

    matrix_imag->m = 1;
    matrix_imag->n = 1;
    matrix_imag->nz = 0;
    matrix_imag->nzmax = 1;

*/
    matrix_real = cs_spalloc(1,1,1,1,1); // arguments: row = 1, column = 1, max_entry = 1, allocate_memory = 1, triplet_form = 1
    matrix_imag = cs_spalloc(1,1,1,1,1);

    clock_when_matrix_is_changed = clock();
}

bool SPARSE_MATRIX::matrix_in_triplet_form() const
{
    if(matrix_real->nz != -1) return true;
    else return false;
}

bool SPARSE_MATRIX::matrix_in_compressed_column_form() const
{
    return not matrix_in_triplet_form();
}

void SPARSE_MATRIX::add_entry(int row, int col, double value)
{
    add_entry(row, col, complex<double>(value,0.0));
}

void SPARSE_MATRIX::add_entry(int row, int col, complex<double> value)
{
    //if(value==0.0)
    //    return;

    if(matrix_in_triplet_form())
    {

        cs_entry(matrix_real, row, col, value.real());
        cs_entry(matrix_imag, row, col, value.imag());

        clock_when_matrix_is_changed = clock();
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

void SPARSE_MATRIX::convert_to_triplet_form()
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

    clock_when_matrix_is_changed = clock();
}

void SPARSE_MATRIX::compress_and_merge_duplicate_entries()
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

    clock_when_matrix_is_changed = clock();
}

void SPARSE_MATRIX::transpose()
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

    clock_when_matrix_is_changed = clock();
}

int SPARSE_MATRIX::get_matrix_size() const
{
    if(matrix_real!=NULL or matrix_imag != NULL) return matrix_real->n;
    else                                         return 0;
}

int SPARSE_MATRIX::get_matrix_entry_count() const
{
    return matrix_real->nzmax;
}

int SPARSE_MATRIX::get_starting_index_of_column(int col) const
{
    if(col>=0 and col<=get_matrix_size()) return matrix_real->p[col];
    else                                  return INDEX_NOT_EXIST;
}

int SPARSE_MATRIX::get_row_number_of_entry_index(int index) const
{
    int n = get_matrix_size();
    if(index<=get_starting_index_of_column(n))  return matrix_real->i[index];
    else                                        return INDEX_NOT_EXIST;
}

int SPARSE_MATRIX::get_column_number_of_entry_index(int index) const
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

int SPARSE_MATRIX::get_entry_index(int row, int col) const
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

complex<double> SPARSE_MATRIX::get_entry_value(int row, int col) const
{
    return get_entry_value(get_entry_index(row, col));
}

complex<double> SPARSE_MATRIX::get_entry_value(int index) const
{
    return complex<double>(get_real_entry_value(index), get_imag_entry_value(index));
}

double SPARSE_MATRIX::get_real_entry_value(int index) const
{
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return matrix_real->x[index];
    else
        return 0.0;
}

double SPARSE_MATRIX::get_imag_entry_value(int index) const
{
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return matrix_imag->x[index];
    else
        return 0.0;
}

void SPARSE_MATRIX::change_entry_value(int row, int col, double value)
{
    change_entry_value(row, col, complex<double>(value, 0.0));
}

void SPARSE_MATRIX::change_entry_value(int index, double value)
{
    change_entry_value(index, complex<double>(value, 0.0));
}

void SPARSE_MATRIX::change_entry_value(int row, int col, complex<double> value)
{
    // get entry index of a compress matrix
    int index = get_entry_index(row, col); // if it is triplet format, will be converted into compressed format
    change_entry_value(index, value);
}

void SPARSE_MATRIX::change_entry_value(int index, complex<double> value)
{
    if(index != INDEX_NOT_EXIST)
    {
        change_real_entry_value(index, value.real());
        change_imag_entry_value(index, value.imag());
    }
}

void SPARSE_MATRIX::change_real_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        matrix_real->x[index]=value; // found

        clock_when_matrix_is_changed = clock();
    }
}


void SPARSE_MATRIX::change_imag_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        matrix_imag->x[index]=value; // found

        clock_when_matrix_is_changed = clock();
    }
}


vector<size_t> SPARSE_MATRIX::get_reorder_permutation()
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

void SPARSE_MATRIX::LU_factorization(int order, double tolerance)
{
    if(LU_factorization_is_performed()) return;

    char buffer[256];

    cs_nfree(LU); // free LU (csn *)
    cs_sfree(LU_symbolic);
    csi n, ok ;
    if (!CS_CSC (matrix_real))/* check inputs */
    {
        sprintf(buffer, "No real matrix is set for LU factorization. Return. (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    n = matrix_real->n ;
    LU_symbolic = cs_sqr (order, matrix_real, 0) ;              /* ordering and symbolic analysis */
    LU = cs_lu (matrix_real, LU_symbolic, tolerance) ;                 /* numeric LU factorization */
    ok = (LU_symbolic && LU) ; // check
    if(ok!=1)
    {
        sprintf(buffer, "ALERT!  LU factorization for sparse matrix failed! (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    // reset working space
    if(LU_workspace!=NULL) free(LU_workspace);
    LU_workspace = (double *)cs_malloc(n, sizeof (double)) ;  // get workspace
    if(LU_workspace == NULL)
    {
        sprintf(buffer, "Global variable LU_workspace was not successfully set. No workspace for LU solution. (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp(buffer);
    }

    clock_when_LU_factorization_is_performed = clock(); // mark the clock when finish the LU decomposition
    return;
}

bool SPARSE_MATRIX::LU_factorization_is_performed() const
{
    if(clock_when_LU_factorization_is_performed > clock_when_matrix_is_changed)
        return true;
    else
        return false;
}

vector<double> SPARSE_MATRIX::solve_Ax_eq_b(vector<double> b)
{

    if(not LU_factorization_is_performed())
        LU_factorization(1, 1e-6);

    solve_Lx_eq_b(b);

    solve_xU_eq_b(b);

    return b;
}

void SPARSE_MATRIX::solve_Lx_eq_b(vector<double>& b)
{
    ostringstream osstream;
    double* bb = (double*)malloc(b.size()*sizeof(double));
    if(bb==NULL)
    {
        osstream<<"Error. Failed to allocate temporary array for solving Lx=b.(function "<<__FUNCTION__<<" in file "<<__FILE__;
        show_information_with_leading_time_stamp(osstream);

        return; // failed to allocate array bb
    }

    size_t n = b.size();
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

void SPARSE_MATRIX::solve_xU_eq_b(vector<double>& b)
{
    string buffer;
    char cbuffer[1000];
    double* bb = (double*)malloc(b.size()*sizeof(double));
    if(bb==NULL)
    {
        sprintf(cbuffer,"Error. Failed to allocate temporary array for solving xU=b.(function %s in file %s)\n",__FUNCTION__,__FILE__);
        buffer = cbuffer;
        show_information_with_leading_time_stamp(buffer);
        return; // failed to allocate array bb
    }

    size_t n = b.size();
    for(size_t i=0; i!=n; ++i) LU_workspace[i]=b[i]; // set bb
    int OK = cs_usolve(LU->U, LU_workspace) ;               /* x = U\x */
    cs_ipvec(LU_symbolic->q, LU_workspace, bb, matrix_real->n) ;          /* b(q) = x */

    if(OK != 1)
    {
        sprintf(cbuffer,"Error. Failed to solve xU=b.(function %s in file %s)\n",__FUNCTION__,__FILE__);
        buffer = cbuffer;
        show_information_with_leading_time_stamp(buffer);
        return; // failed to solve equation
    }
    // now solution is OK
    for(size_t i=0; i!=n; ++i) b[i]=bb[i]; // now reset b with bb

    free(bb);

    return;
}


void SPARSE_MATRIX::report_brief() const
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="matrix entries:\n";
    show_information_with_leading_time_stamp(buffer);

	sprintf(cbuffer,"row   column     real       imaginary\n");
    show_information_with_leading_time_stamp(cbuffer);

    int n = get_matrix_size();
    for(j=0;j!=n;++j)
    {
        for(k=matrix_real->p[j];k<matrix_real->p[j+1];++k)
        {
            i=matrix_real->i[k];

            if(matrix_imag->n==0)
                sprintf(cbuffer,"%-6d, %-6d, % 10.6f\n",i,j,matrix_real->x[k]);
            else
                sprintf(cbuffer,"%-6d, %-6d, % 10.6f, % 10.6f\n",i,j,matrix_real->x[k],matrix_imag->x[k]);

            buffer = cbuffer;
            show_information_with_leading_time_stamp(buffer);
        }
    }
}


void SPARSE_MATRIX::report_full() const
{
    string buffer;
}

void SPARSE_MATRIX::save_matrix_to_file(string filename) const
{
    ostringstream osstream;

    ofstream file;
    file.open(filename);

    if(not file.is_open())
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving sparse matrix contents."<<endl
          <<"No sparse matrix will be exported.";
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


SPARSE_MATRIX& SPARSE_MATRIX::operator=(const SPARSE_MATRIX& matrix)
{
    //ostringstream osstream;

    //os<<"go copying matrix");
    //show_information_with_leading_time_stamp(osstream);

    if(this==(&matrix)) return *this;

    clock_when_LU_factorization_is_performed = clock();

    int nz = matrix.get_matrix_entry_count();

    clear();

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

    clock_when_matrix_is_changed = clock();
    return *this;
}


vector<double> operator/(vector<double>b, SPARSE_MATRIX& A)
{
    return A.solve_Ax_eq_b(b);
}


