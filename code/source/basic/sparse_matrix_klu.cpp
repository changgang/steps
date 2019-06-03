#include "header/basic/sparse_matrix_klu.h"
#include "umfpack.h"
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

SPARSE_MATRIX_KLU::SPARSE_MATRIX_KLU():SPARSE_MATRIX()
{
    //constructor
    n_row = 0;
    n_column = 0;
    triplet_row_index.clear();
    triplet_column_index.clear();
    triplet_matrix_real.clear();
    triplet_matrix_imag.clear();

    compressed_column_starting_index = NULL;
    compressed_row_index = NULL;
    compressed_matrix_real = NULL;
    compressed_matrix_imag = NULL;

    Symbolic = NULL;
    Numeric = NULL;

    flag_matrix_in_triplet_form = true;

    clear();
}

SPARSE_MATRIX_KLU::SPARSE_MATRIX_KLU(const SPARSE_MATRIX_KLU& matrix)
{
    n_row = 0;
    n_column = 0;

    triplet_row_index.clear();
    triplet_column_index.clear();
    triplet_matrix_real.clear();
    triplet_matrix_imag.clear();

    compressed_column_starting_index = NULL;
    compressed_row_index = NULL;
    compressed_matrix_real = NULL;
    compressed_matrix_imag = NULL;

    Symbolic = NULL;
    Numeric = NULL;

    flag_matrix_in_triplet_form = true;
    clear();

    copy_from_const_matrix(matrix);
}

SPARSE_MATRIX_KLU& SPARSE_MATRIX_KLU::operator=(const SPARSE_MATRIX_KLU& matrix)
{
    if(this==(&matrix)) return *this;

    copy_from_const_matrix(matrix);

    return *this;
}

void SPARSE_MATRIX_KLU::copy_from_const_matrix(const SPARSE_MATRIX_KLU& matrix)
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
    if(matrix.matrix_in_compressed_column_form())
        compress_and_merge_duplicate_entries();

    update_clock_when_matrix_is_changed();
}


SPARSE_MATRIX_KLU::~SPARSE_MATRIX_KLU()
{
    // destructor
    clear();
}

void SPARSE_MATRIX_KLU::clear()
{
    // clear function
    update_clock_when_LU_factorization_is_performed();

    n_row = 0;
    n_column = 0;

    triplet_row_index.clear();
    triplet_column_index.clear();
    triplet_matrix_real.clear();
    triplet_matrix_imag.clear();

    if(compressed_column_starting_index!=NULL) free(compressed_column_starting_index);
    if(compressed_row_index!=NULL) free(compressed_row_index);
    if(compressed_matrix_real!=NULL) free(compressed_matrix_real);
    if(compressed_matrix_imag!=NULL) free(compressed_matrix_imag);

    compressed_column_starting_index = NULL;
    compressed_row_index = NULL;
    compressed_matrix_real = NULL;
    compressed_matrix_imag = NULL;

    if(Symbolic!=NULL) klu_free_symbolic (&Symbolic, &Common);
    if(Numeric!=NULL) klu_free_numeric (&Numeric, &Common);

    Symbolic = NULL;
    Numeric = NULL;

    flag_matrix_in_triplet_form = true;

    update_clock_when_matrix_is_changed();
}

bool SPARSE_MATRIX_KLU::matrix_in_triplet_form() const
{
    return flag_matrix_in_triplet_form;
}


void SPARSE_MATRIX_KLU::add_entry(int row, int col, complex<double> value)
{
    if(matrix_in_triplet_form())
    {
        size_t nz = get_matrix_entry_count();
        for(size_t i=0; i<nz; ++i)
        {
            size_t r = triplet_row_index[i];
            size_t c = triplet_column_index[i];
            if(r==row and c==col)
            {
                triplet_matrix_real[i] += value.real();
                triplet_matrix_imag[i] += value.imag();
                return;
            }
        }
        triplet_row_index.push_back(row);
        triplet_column_index.push_back(col);
        triplet_matrix_real.push_back(value.real());
        triplet_matrix_imag.push_back(value.imag());

        if(row+1>n_row) n_row = row+1;
        if(col+1>n_column) n_column = col+1;
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
            triplet_row_index.push_back(row);
            triplet_column_index.push_back(col);
            triplet_matrix_real.push_back(value.real());
            triplet_matrix_imag.push_back(value.imag());
            if(row+1>n_row) n_row = row+1;
            if(col+1>n_column) n_column = col+1;
            compress_and_merge_duplicate_entries();
        }
    }
}

void SPARSE_MATRIX_KLU::convert_to_triplet_form()
{
    if(matrix_in_compressed_column_form())
    {
        triplet_row_index.clear();
        triplet_column_index.clear();
        triplet_matrix_real.clear();
        triplet_matrix_imag.clear();

        int row, col;
        size_t nz = get_matrix_entry_count();
        for(int k=0; k!=nz; ++k)
        {
            row = get_row_number_of_entry_index(k);
            col = get_column_number_of_entry_index(k);
            triplet_row_index.push_back(row);
            triplet_column_index.push_back(col);
            triplet_matrix_real.push_back(get_real_entry_value(k));
            triplet_matrix_imag.push_back(get_imag_entry_value(k));
        }
        update_clock_when_matrix_is_changed();

        flag_matrix_in_triplet_form = true;

        if(compressed_column_starting_index!=NULL) free(compressed_column_starting_index);
        if(compressed_row_index!=NULL) free(compressed_row_index);
        if(compressed_matrix_real!=NULL) free(compressed_matrix_real);
        if(compressed_matrix_imag!=NULL) free(compressed_matrix_imag);
    }
}

void SPARSE_MATRIX_KLU::compress_and_merge_duplicate_entries()
{
    // compress the sparse matrix
    if(matrix_in_triplet_form())
    {
        size_t nz = triplet_matrix_real.size();
        int* row_index = (int*)calloc(nz, sizeof(int));
        int* col_index = (int*)calloc(nz, sizeof(int));
        double* vreal = (double*)calloc(nz, sizeof(double));
        double* vimag = (double*)calloc(nz, sizeof(double));
        for(size_t i=0; i<nz; ++i)
        {
            row_index[i] = triplet_row_index[i];
            col_index[i] = triplet_column_index[i];
            vreal[i]=triplet_matrix_real[i];
            vimag[i]=triplet_matrix_imag[i];
            //cout<<row_index[i]<<", "<<col_index[i]<<": "<<vreal[i]<<"+j"<<vimag[i]<<endl;
        }
        compressed_column_starting_index = (int*)calloc(n_column+1, sizeof(int));
        compressed_row_index = (int*)calloc(nz, sizeof(int));
        // real part
        compressed_matrix_real = (double*)calloc(nz, sizeof(double));
        umfpack_di_triplet_to_col (n_row, n_column, nz, row_index, col_index, vreal,
                                   compressed_column_starting_index, compressed_row_index, compressed_matrix_real, (int *) NULL) ;
        // imaginary part
        compressed_matrix_imag = (double*)calloc(nz, sizeof(double));
        umfpack_di_triplet_to_col (n_row, n_column, nz, row_index, col_index, vimag,
                                   compressed_column_starting_index, compressed_row_index, compressed_matrix_imag, (int *) NULL) ;
        triplet_row_index.clear();
        triplet_column_index.clear();
        triplet_matrix_real.clear();
        triplet_matrix_imag.clear();
        flag_matrix_in_triplet_form = false;

        free(row_index);
        free(col_index);
        free(vreal);
        free(vimag);

        update_clock_when_matrix_is_changed();
    }
}

void SPARSE_MATRIX_KLU::transpose()
{
    // transpose a compress format matrix
    if(matrix_in_triplet_form()) // if in triplet format, convert to compressed format
        compress_and_merge_duplicate_entries(); // convert

    size_t nz = get_matrix_entry_count();

    int *temp_column_starting_index = (int *) calloc ((n_column+1), sizeof (int)) ;
    int *temp_row_index = (int *) calloc (nz, sizeof (int)) ;
    double *temp_rvalue = (double *) calloc (nz, sizeof (double)) ;
    double *temp_ivalue = (double *) calloc (nz, sizeof (double)) ;
    int *itemp;
    double *dtemp;
    // transpose real part
    umfpack_di_transpose (n_row, n_column, compressed_column_starting_index, compressed_row_index, compressed_matrix_real,
                                   (int *) NULL,  (int *) NULL, temp_column_starting_index, temp_row_index, temp_rvalue) ;
    dtemp = compressed_matrix_real;
    compressed_matrix_real = temp_rvalue;
    temp_rvalue = dtemp;

    // transpose imaginary part
    umfpack_di_transpose (n_row, n_column, compressed_column_starting_index, compressed_row_index, compressed_matrix_imag,
                                   (int *) NULL,  (int *) NULL, temp_column_starting_index, temp_row_index, temp_ivalue) ;
    dtemp = compressed_matrix_imag;
    compressed_matrix_imag = temp_ivalue;
    temp_ivalue = dtemp;
    itemp = compressed_column_starting_index;
    compressed_column_starting_index = temp_column_starting_index;
    temp_column_starting_index = itemp;
    itemp = compressed_row_index;
    compressed_row_index = temp_row_index;
    temp_row_index = itemp;

    free(temp_column_starting_index);
    free(temp_row_index);
    free(temp_rvalue);
    free(temp_ivalue);

    size_t stemp;
    stemp = n_row;
    n_row = n_column;
    n_column = stemp;

    update_clock_when_matrix_is_changed();
}

int SPARSE_MATRIX_KLU::get_matrix_size() const
{
    return n_column;
}

int SPARSE_MATRIX_KLU::get_matrix_entry_count() const
{
    if(matrix_in_compressed_column_form())
        return compressed_column_starting_index[n_column];
    else
        return int(triplet_matrix_real.size());
}

int SPARSE_MATRIX_KLU::get_starting_index_of_column(int col) const
{
    if(col>=0 and col<=get_matrix_size()) return compressed_column_starting_index[col];
    else                                  return INDEX_NOT_EXIST;
}

int SPARSE_MATRIX_KLU::get_row_number_of_entry_index(int index) const
{
    int n = get_matrix_size();
    if(index<=get_starting_index_of_column(n))  return compressed_row_index[index];
    else                                        return INDEX_NOT_EXIST;
}

int SPARSE_MATRIX_KLU::get_entry_index(int row, int col) const
{
    // return entry index of compressed matrix
    if(row >= 0 and col >= 0)
    {
        if(not matrix_in_compressed_column_form())
        {
            ostringstream osstream;
            osstream<<"Error. Sparse matrix not in compressed form when getting entry index.\nINDEX_NOT_EXIST will be returned.";
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            return INDEX_NOT_EXIST;
        }

        int index = INDEX_NOT_EXIST;

        if(row < get_matrix_size() and col < get_matrix_size())
        {
            for(int k=compressed_column_starting_index[col]; k!=compressed_column_starting_index[col+1]; ++k)
            {
                if(compressed_row_index[k] > row) break; // if no entry of (row, col)
                if(compressed_row_index[k] == row)
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

complex<double> SPARSE_MATRIX_KLU::get_complex_entry_value(int index)  const
{
    //if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
    if(index>=0 && index<=compressed_column_starting_index[n_column])
        return complex<double>(compressed_matrix_real[index], compressed_matrix_imag[index]);
    else
        return 0.0;
}

double SPARSE_MATRIX_KLU::get_real_entry_value(int index) const
{
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return compressed_matrix_real[index];
    else
        return 0.0;
}

double SPARSE_MATRIX_KLU::get_imag_entry_value(int index) const
{
    if(index>=0 && index<=get_starting_index_of_column(get_matrix_size()))
        return compressed_matrix_imag[index];
    else
        return 0.0;
}

void SPARSE_MATRIX_KLU::change_real_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        compressed_matrix_real[index]=value; // found

        update_clock_when_matrix_is_changed();
    }
}

void SPARSE_MATRIX_KLU::change_imag_entry_value(int index, double value)
{
    if(index != INDEX_NOT_EXIST and index>=0 and index<get_starting_index_of_column(get_matrix_size()))
    {
        compressed_matrix_imag[index]=value; // found

        update_clock_when_matrix_is_changed();
    }
}


vector<size_t> SPARSE_MATRIX_KLU::get_reorder_permutation()
{
    if(matrix_in_triplet_form())
        compress_and_merge_duplicate_entries();

    vector<size_t> permutation;
    permutation.reserve(n_column);
    int *P = (int*) calloc(n_column, sizeof(int));
    amd_order (n_column, compressed_column_starting_index, compressed_row_index, P, (double *) NULL, (double *) NULL) ;
    for(size_t i=0; i<n_column; ++i)
        permutation[i]=P[i];
    free(P);

    return permutation;
}

void SPARSE_MATRIX_KLU::LU_factorization(int order, double tolerance)
{
    if(not LU_factorization_is_performed())
    {
        if(Symbolic!=NULL) klu_free_symbolic(&Symbolic, &Common);
        if(Numeric!=NULL) klu_free_numeric(&Numeric, &Common);

        Symbolic = klu_analyze (n_column, compressed_column_starting_index, compressed_row_index, &Common) ;
        Numeric = klu_factor (compressed_column_starting_index, compressed_row_index, compressed_matrix_real, Symbolic, &Common) ;

        update_clock_when_LU_factorization_is_performed(); // mark the clock when finish the LU decomposition
    }
}

vector<double> SPARSE_MATRIX_KLU::solve_Ax_eq_b(vector<double>& b)
{
    if(not LU_factorization_is_performed())   LU_factorization();
    double * B = (double*)calloc(b.size(), sizeof(double));
    for(size_t i=0; i<n_row; ++i) B[i] = b[i];

    klu_solve (Symbolic, Numeric, n_row, 1, B, &Common) ;

    for(size_t i=0; i<n_row; ++i) b[i] = B[i];
    free(B);
    return b;
}

void SPARSE_MATRIX_KLU::report_brief() const
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
        for(k=compressed_column_starting_index[j];k<compressed_column_starting_index[j+1];++k)
        {
            i=compressed_row_index[k];

            snprintf(cbuffer,1000, "%-6d, %-6d, % 10.6f, % 10.6f\n",i,j,compressed_matrix_real[k],compressed_matrix_imag[k]);

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}


void SPARSE_MATRIX_KLU::report_full() const
{
    string buffer;
}

void SPARSE_MATRIX_KLU::save_matrix_to_file(string filename) const
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
            for(k=compressed_column_starting_index[j];k<compressed_column_starting_index[j+1];++k)
            {
                i=compressed_row_index[k];

                file<<i<<","<<j<<","
                    <<setprecision(6)<<fixed<<compressed_matrix_real[k]<<","
                    <<setprecision(6)<<fixed<<compressed_matrix_imag[k]<<endl;
            }
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving sparse matrix(CSparse) contents."<<endl
          <<"No sparse matrix(CSparse) will be exported.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }
}

vector<double> operator/(vector<double>&b, SPARSE_MATRIX_KLU& A)
{
    return A.solve_Ax_eq_b(b);
}
