#include "header/basic/complex_sparse_matrix_csparse.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

COMPLEX_SPARSE_MATRIX_CSPARSE::COMPLEX_SPARSE_MATRIX_CSPARSE():COMPLEX_SPARSE_MATRIX()
{
    //constructor

    matrix_complex = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;
    bb = NULL;
    bb_size = 0;

    clear();
}

COMPLEX_SPARSE_MATRIX_CSPARSE::COMPLEX_SPARSE_MATRIX_CSPARSE(const COMPLEX_SPARSE_MATRIX_CSPARSE& matrix)
{
    matrix_complex = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;
    bb = NULL;
    bb_size = 0;

    clear();

    copy_from_const_matrix(matrix);
}

COMPLEX_SPARSE_MATRIX_CSPARSE& COMPLEX_SPARSE_MATRIX_CSPARSE::operator=(const COMPLEX_SPARSE_MATRIX_CSPARSE& matrix)
{
    if(this==(&matrix)) return *this;

    copy_from_const_matrix(matrix);

    return *this;
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::copy_from_const_matrix(const COMPLEX_SPARSE_MATRIX_CSPARSE& matrix)
{
    clear();

    /* the following codes are replace on June 28, 2019 for higher performance
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
    if(matrix.matrix_in_compressed_column_form())
        compress_and_merge_duplicate_entries();
        */
    if(matrix.matrix_in_triplet_form())
    {
        ostringstream osstream;
        osstream<<"Matrix is in triplet form. Cannot copy matrix with "<<__FUNCTION__<<"() in "<<__FILE__;
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }

    int ncol = matrix.get_matrix_size();

    complex<double> value;
    for(int col=0; col!=ncol; ++col)
    {
        int k_start = matrix.get_starting_index_of_column(col);
        int k_end = matrix.get_starting_index_of_column(col+1);
        for(int k=k_start; k!=k_end; ++k)
        {
            int row = matrix.get_row_number_of_entry_index(k);
            value = matrix.get_entry_value(k);

            add_entry(row, col, value);
        }
    }
    compress_and_merge_duplicate_entries();


    update_clock_when_matrix_is_changed();
}


COMPLEX_SPARSE_MATRIX_CSPARSE::~COMPLEX_SPARSE_MATRIX_CSPARSE()
{
    // destructor
    if(matrix_complex!=NULL) cxs_spfree(matrix_complex);
    if(LU!=NULL)          cxs_nfree(LU);
    if(LU_symbolic!=NULL) cxs_sfree(LU_symbolic);
    if(LU_workspace!=NULL) free(LU_workspace);
    if(bb!=NULL) free(bb);

    /*matrix_complex = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;
    bb = NULL;
    bb_size = 0;*/
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::clear()
{
    // clear function
    update_clock_when_LU_factorization_is_performed();

    if(matrix_complex!=NULL) cxs_spfree(matrix_complex);
    if(LU!=NULL)          cxs_nfree(LU);
    if(LU_symbolic!=NULL) cxs_sfree(LU_symbolic);
    if(LU_workspace!=NULL) free(LU_workspace);
    if(bb!=NULL) free(bb);

    matrix_complex = NULL;
    LU = NULL;
    LU_symbolic = NULL;
    LU_workspace = NULL;
    bb = NULL;
    bb_size = 0;

    matrix_complex = cxs_spalloc(1,1,1,1,1); // arguments: row = 1, column = 1, max_entry = 1, allocate_memory = 1, triplet_form = 1

    update_clock_when_matrix_is_changed();
}

bool COMPLEX_SPARSE_MATRIX_CSPARSE::matrix_in_triplet_form() const
{
    if(matrix_complex->nz != -1) return true;
    else return false;
}


void COMPLEX_SPARSE_MATRIX_CSPARSE::add_entry(int row, int col, const complex<double>& value)
{
    if(matrix_in_triplet_form())
    {
        cxs_entry(matrix_complex, row, col, value);

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
            cxs_entry(matrix_complex, row, col, value);
            compress_and_merge_duplicate_entries();
        }
    }
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::convert_to_triplet_form()
{
    if(matrix_in_compressed_column_form())
    {
        cxs *mat_complex; // temp mat
        int *tempi; // temp index
        complex<double> *tempd; // temp value

        // copy
        mat_complex = cxs_spalloc(1,1,1,1,1);
        int n = get_matrix_entry_count();
        int row, col;
        for(int k=0; k!=n; ++k)
        {
            row = get_row_number_of_entry_index(k);
            col = get_column_number_of_entry_index(k);
            cxs_entry(mat_complex, row, col, get_real_entry_value(k));
        }
        // swap
        matrix_complex->nzmax = mat_complex->nzmax;
        matrix_complex->m = mat_complex->m;
        matrix_complex->n = mat_complex->n;
        matrix_complex->nz = mat_complex->nz;
        tempi = matrix_complex->p; matrix_complex->p = mat_complex->p; mat_complex->p = tempi;
        tempi = matrix_complex->i; matrix_complex->i = mat_complex->i; mat_complex->i = tempi;
        tempd = matrix_complex->x; matrix_complex->x = mat_complex->x; mat_complex->x = tempd;
        // free temp mat
        cxs_spfree(mat_complex);

        update_clock_when_matrix_is_changed();
    }
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::compress_and_merge_duplicate_entries()
{
    // compress the sparse matrix
    if(matrix_in_triplet_form())
    {
        cxs *mat; // temp mat
        int *tempi; // temp index
        complex<double> *tempd; // temp value

        // real part
        // compress the matrix
        mat = cxs_compress(matrix_complex);
        // swap
        matrix_complex->nzmax = mat->nzmax;
        matrix_complex->m = mat->m;
        matrix_complex->n = mat->n;
        matrix_complex->nz = mat->nz;
        tempi = matrix_complex->p; matrix_complex->p = mat->p; mat->p = tempi;
        tempi = matrix_complex->i; matrix_complex->i = mat->i; mat->i = tempi;
        tempd = matrix_complex->x; matrix_complex->x = mat->x; mat->x = tempd;
        // free temp mat
        cxs_spfree(mat);

        cxs_dupl(matrix_complex); // merge duplicate entries

        // at last, transpose twice
        transpose();
        transpose();

        update_clock_when_matrix_is_changed();
    }
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::transpose()
{
    // transpose a compress format matrix
    if(matrix_in_triplet_form()) // if in triplet format, convert to compressed format
        compress_and_merge_duplicate_entries(); // convert

    cxs *mat; // temp mat
    int *tempi; // temp index
    complex<double> *tempd; // temp value

    // transpose without conjugate, -1 is used. if 1 is used, conjugate is used.
    mat = cxs_transpose(matrix_complex, -1);

    // swap
    matrix_complex->nzmax = mat->nzmax;
    matrix_complex->m = mat->m;
    matrix_complex->n = mat->n;
    matrix_complex->nz = mat->nz;
    tempi = matrix_complex->p; matrix_complex->p = mat->p; mat->p = tempi;
    tempi = matrix_complex->i; matrix_complex->i = mat->i; mat->i = tempi;
    tempd = matrix_complex->x; matrix_complex->x = mat->x; mat->x = tempd;
    // free temp mat
    cxs_spfree(mat);

    update_clock_when_matrix_is_changed();
}

int COMPLEX_SPARSE_MATRIX_CSPARSE::get_matrix_size() const
{
    return matrix_complex->n;

    if(matrix_complex!=NULL) return matrix_complex->n;
    else                  return 0;
}

int COMPLEX_SPARSE_MATRIX_CSPARSE::get_matrix_entry_count() const
{
    return matrix_complex->nzmax;
}

int COMPLEX_SPARSE_MATRIX_CSPARSE::get_starting_index_of_column(int col) const
{
    return matrix_complex->p[col];
    if(col>=0 and col<=get_matrix_size()) return matrix_complex->p[col];
    else                                  return INDEX_NOT_EXIST;
}

int COMPLEX_SPARSE_MATRIX_CSPARSE::get_row_number_of_entry_index(int index) const
{
    return matrix_complex->i[index];
    int n = get_matrix_size();
    if(index<=get_starting_index_of_column(n))  return matrix_complex->i[index];
    else                                        return INDEX_NOT_EXIST;
}

int COMPLEX_SPARSE_MATRIX_CSPARSE::get_entry_index(int row, int col) const
{
    // return entry index of compressed matrix

    if(row >= 0 and col >= 0)
    {
        if(matrix_in_compressed_column_form())
        {
            int index = INDEX_NOT_EXIST;

            if(row < get_matrix_size() and col < get_matrix_size())
            {
                int kstart = matrix_complex->p[col];
                int kend = matrix_complex->p[col+1];
                for(int k=kstart; k!=kend; ++k)
                {
                    if(matrix_complex->i[k] > row) break; // if no entry of (row, col)
                    if(matrix_complex->i[k] == row)
                    {
                        index = k;
                        break;
                    }
                }
            }
            return index;
        }
        else
        {
            ostringstream osstream;
            osstream<<"Error. Sparse matrix not in compressed form when getting entry index.\nINDEX_NOT_EXIST will be returned.";
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            return INDEX_NOT_EXIST;
        }
    }
    else
        return INDEX_NOT_EXIST;
}

complex<double> COMPLEX_SPARSE_MATRIX_CSPARSE::get_complex_entry_value(int index)  const
{
    return matrix_complex->x[index];
    if(index>=0 && index<=matrix_complex->p[matrix_complex->n]) // this condition is equivalent to previous line
        return matrix_complex->x[index];
    else
        return 0.0;
}

double COMPLEX_SPARSE_MATRIX_CSPARSE::get_real_entry_value(int index) const
{
    return (matrix_complex->x[index]).real();
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return (matrix_complex->x[index]).real();
    else
        return 0.0;
}

double COMPLEX_SPARSE_MATRIX_CSPARSE::get_imag_entry_value(int index) const
{
    return matrix_complex->x[index].imag();
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return matrix_complex->x[index].imag();
    else
        return 0.0;
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::change_real_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        matrix_complex->x[index]=complex<double>(value, matrix_complex->x[index].imag()); // found

        update_clock_when_matrix_is_changed();
    }
}


void COMPLEX_SPARSE_MATRIX_CSPARSE::change_imag_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        matrix_complex->x[index]=complex<double>(matrix_complex->x[index].real(),value); // found

        update_clock_when_matrix_is_changed();
    }
}


vector<unsigned int> COMPLEX_SPARSE_MATRIX_CSPARSE::get_reorder_permutation()
{
    int* p = cxs_amd(1,matrix_complex); // use the real part for re-ordering

    vector<unsigned int> permutation;
    permutation.reserve(get_matrix_size());
    int n = get_matrix_size();
    for(int i = 0; i!=n; ++i)
        permutation.push_back((unsigned int)(p[i]));

    free(p);

    return permutation;
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::LU_factorization(int order, double tolerance)
{
    //if(LU_factorization_is_performed()) return;

    char buffer[256];

    cxs_nfree(LU); // free LU (csn *)
    cxs_sfree(LU_symbolic);
    csi n, ok ;
    if (CS_CSC (matrix_complex))/* check inputs */
    {
        n = matrix_complex->n ;
        LU_symbolic = cxs_sqr (order, matrix_complex, 0) ;              /* ordering and symbolic analysis */
        LU = cxs_lu (matrix_complex, LU_symbolic, tolerance) ;                 /* numeric LU factorization */

        ok = (LU_symbolic && LU) ; // check
        if(ok==1)
        {
            // reset working space
            if(LU_workspace!=NULL) free(LU_workspace);
            LU_workspace = (complex<double> *)cxs_malloc(n, sizeof (complex<double>)) ;  // get workspace
            if(LU_workspace != NULL)
                ;
            else
            {
                snprintf(buffer, 256, "Global variable LU_workspace was not successfully set. No workspace for LU solution. (line %d in file %s)\n",__LINE__,__FILE__);
                show_information_with_leading_time_stamp_with_default_toolkit(buffer);
            }
            update_clock_when_LU_factorization_is_performed(); // mark the clock when finish the LU decomposition
        }
        else
        {
            snprintf(buffer, 256, "ALERT!  LU factorization for sparse matrix(CXSparse) failed! (line %d in file %s)\n",__LINE__,__FILE__);
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
    else
    {
        snprintf(buffer, 256, "No real matrix is set for LU factorization. Return. (line %d in file %s)\n",__LINE__,__FILE__);
        show_information_with_leading_time_stamp_with_default_toolkit(buffer);
    }
}

vector<complex<double> >& COMPLEX_SPARSE_MATRIX_CSPARSE::solve_Ax_eq_b(vector<complex<double> >& b)
{
    if(LU_factorization_is_performed())
    {
        solve_Lx_eq_b(b);
        solve_xU_eq_b(b);
    }
    else
    {
        LU_factorization(1, 1e-13);
        /*
        cout<<"Div LU->L->n = "<<LU->L->n<<endl;
        cout<<"Mul LU->L->p[n] = "<<LU->L->p[LU->L->n]<<endl;
        cout<<"Div LU->U->n = "<<LU->U->n<<endl;
        cout<<"Mul LU->U->p[n] = "<<LU->U->p[LU->U->n]<<endl;
        */
        solve_Lx_eq_b(b);

        solve_xU_eq_b(b);
    }

    return b;
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::solve_Lx_eq_b(vector<complex<double> >& b)
{
    ostringstream osstream;
    unsigned int n = b.size();
    if(bb!=NULL)
    {
        if(n<=bb_size)
        {
            ;
        }
        else
        {
            free(bb);
            bb = (complex<double>*)malloc(n*sizeof(complex<double>));
            bb_size = n;
        }
    }
    else
    {
        bb = (complex<double>*)malloc(n*sizeof(complex<double>));
        bb_size = n;
    }
    if(bb!=NULL)
    {
        for(unsigned int i=0; i!=n; ++i) bb[i]=b[i]; // set bb

        cxs_ipvec(LU->pinv, bb, LU_workspace, matrix_complex->n) ;       /* x = b(p) */
        int OK = cxs_lsolve (LU->L, LU_workspace) ;               /* x = L\x */

        if(OK == 1)
        {
            // now solution is OK
            for(unsigned int i=0; i!=n; ++i) b[i]=LU_workspace[i]; // now reset b with bb
            //memcpy(b, LU_workspace, n*sizeof(complex<double>));
        }
        else
        {
            osstream<<"Error. Failed to solve Lx=b.(function "<<__FUNCTION__<<" in file "<<__FILE__;
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        }
    }
    else
    {
        osstream<<"Error. Failed to allocate temporary array for solving Lx=b.(function "<<__FUNCTION__<<" in file "<<__FILE__;
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::solve_xU_eq_b(vector<complex<double> >& b)
{
    string buffer;
    char cbuffer[1000];

    unsigned int n = b.size();
    if(bb!=NULL)
    {
        if(n<=bb_size)
        {
            ;
        }
        else
        {
            free(bb);
            bb = (complex<double>*)malloc(n*sizeof(complex<double>));
            bb_size = n;
        }
    }
    else
    {
        bb = (complex<double>*)malloc(n*sizeof(complex<double>));
        bb_size = n;
    }
    if(bb!=NULL)
    {
        for(unsigned int i=0; i!=n; ++i) LU_workspace[i]=b[i]; // set bb

        int OK = cxs_usolve(LU->U, LU_workspace) ;               /* x = U\x */
        cxs_ipvec(LU_symbolic->q, LU_workspace, bb, matrix_complex->n) ;          /* b(q) = x */

        if(OK == 1)
        {
            // now solution is OK
            for(unsigned int i=0; i!=n; ++i) b[i]=bb[i]; // now reset b with bb
            //memcpy(b, bb, n*sizeof(complex<double>));
        }
        else
        {
            snprintf(cbuffer, 1000, "Error. Failed to solve xU=b.(function %s in file %s)\n",__FUNCTION__,__FILE__);
            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
    else
    {
        snprintf(cbuffer, 1000, "Error. Failed to allocate temporary array for solving xU=b.(function %s in file %s)\n",__FUNCTION__,__FILE__);
        buffer = cbuffer;
        show_information_with_leading_time_stamp_with_default_toolkit(buffer);
    }
}


void COMPLEX_SPARSE_MATRIX_CSPARSE::report_brief() const
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   column     real       imaginary\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = get_matrix_size();
    for(j=0;j!=n;++j)
    {
        for(k=matrix_complex->p[j];k<matrix_complex->p[j+1];++k)
        {
            i=matrix_complex->i[k];

            snprintf(cbuffer,1000, "%-6d, %-6d, % 10.6f, % 10.6f\n",i,j,matrix_complex->x[k].real(),matrix_complex->x[k].imag());

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}


void COMPLEX_SPARSE_MATRIX_CSPARSE::report_full() const
{
    string buffer;
}

void COMPLEX_SPARSE_MATRIX_CSPARSE::save_matrix_to_file(string filename) const
{
    ostringstream osstream;

    ofstream file;
    file.open(filename);

    if(file.is_open())
    {
        int i=0, j=0, k=0;

        file<<"ROW,COLUMN,REAL,IMAGINARY"<<endl;

        int n = get_matrix_size();
        for(j=0;j!=n;++j)
        {
            for(k=matrix_complex->p[j];k<matrix_complex->p[j+1];++k)
            {
                i=matrix_complex->i[k];

                file<<i<<","<<j<<","
                    <<setprecision(6)<<fixed<<matrix_complex->x[k].real()<<","
                    <<setprecision(6)<<fixed<<matrix_complex->x[k].imag()<<endl;
            }
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving sparse matrix(CXSparse) contents."<<endl
          <<"No sparse matrix(CXSparse) will be exported.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }
}

vector<complex<double> >& operator/(vector<complex<double> >&b, COMPLEX_SPARSE_MATRIX_CSPARSE& A)
{
    return A.solve_Ax_eq_b(b);
}

unsigned int COMPLEX_SPARSE_MATRIX_CSPARSE::get_memory_usage_in_bytes()
{
    unsigned int n = matrix_complex->nzmax*(sizeof(int)+sizeof(complex<double>))+
                     matrix_complex->n*sizeof(int);

    if(LU!=nullptr)
        n += LU->L->nzmax*(sizeof(int)+sizeof(complex<double>))+
             LU->L->n*sizeof(int)+
             LU->U->nzmax*(sizeof(int)+sizeof(complex<double>))+
             LU->U->n*sizeof(int)+
             LU->L->n*sizeof(int)+
             LU->L->n*sizeof(complex<double>);

    n += matrix_complex->n*sizeof(complex<double>)+
         bb_size*sizeof(complex<double>);
    return n;
}
