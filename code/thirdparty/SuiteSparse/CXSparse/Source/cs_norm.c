#include "cxs.h"
/* 1-norm of a sparse matrix = max (sum (abs (A))), largest column sum */
double cxs_norm (const cxs *A)
{
    CXS_INT p, j, n, *Ap ;
    CXS_ENTRY *Ax ;
    double norm = 0, s ;
    if (!CXS_CSC (A) || !A->x) return (-1) ;             /* check inputs */
    n = A->n ; Ap = A->p ; Ax = A->x ;
    for (j = 0 ; j < n ; j++)
    {
        for (s = 0, p = Ap [j] ; p < Ap [j+1] ; p++) s += CXS_ABS (Ax [p]) ;
        norm = CXS_MAX (norm, s) ;
    }
    return (norm) ;
}
