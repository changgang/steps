#include "cxs.h"

/* convert from complex to real (int version) */
/* C = real(A) if real is true, imag(A) otherwise */
cxs_di *cxs_i_real (cxs_ci *A, int real)
{
    cxs_di *C ;
    int n, triplet, nn, p, nz, *Ap, *Ai, *Cp, *Ci ;
    cxs_complex_t *Ax ;
    double *Cx ;
    if (!A || !A->x) return (NULL) ;    /* return if A NULL or pattern-only */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    triplet = (A->nz >= 0) ;            /* true if A is a triplet matrix */
    nz = triplet ? A->nz : Ap [n] ;
    C = cxs_di_spalloc (A->m, n, A->nzmax, 1, triplet) ;
    if (!C) return (NULL) ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    nn = triplet ? nz : (n+1) ;
    for (p = 0 ; p < nz ; p++) Ci [p] = Ai [p] ;
    for (p = 0 ; p < nn ; p++) Cp [p] = Ap [p] ;
    for (p = 0 ; p < nz ; p++) Cx [p] = real ? creal (Ax [p]) : cimag (Ax [p]) ;
    if (triplet) C->nz = nz ;
    return (C) ;
}

/* convert from real to complex (int version) */
/* C = A if real is true, or C = i*A otherwise */
cxs_ci *cxs_i_complex (cxs_di *A, int real)
{
    cxs_ci *C ;
    int n, triplet, nn, p, nz, *Ap, *Ai, *Cp, *Ci ;
    double *Ax ;
    cxs_complex_t *Cx ;
    if (!A || !A->x) return (NULL) ;    /* return if A NULL or pattern-only */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    triplet = (A->nz >= 0) ;            /* true if A is a triplet matrix */
    nz = triplet ? A->nz : Ap [n] ;
    C = cxs_ci_spalloc (A->m, n, A->nzmax, 1, triplet) ;
    if (!C) return (NULL) ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    nn = triplet ? nz : (n+1) ;
    for (p = 0 ; p < nz ; p++) Ci [p] = Ai [p] ;
    for (p = 0 ; p < nn ; p++) Cp [p] = Ap [p] ;
    for (p = 0 ; p < nz ; p++) Cx [p] = real ? Ax [p] : (I * Ax [p]) ;
    if (triplet) C->nz = nz ;
    return (C) ;
}

/* convert from complex to real (cxs_long_t version) */
/* C = real(A) if real is true, imag(A) otherwise */
cxs_dl *cxs_l_real (cxs_cl *A, cxs_long_t real)
{
    cxs_dl *C ;
    cxs_long_t n, triplet, nn, p, nz, *Ap, *Ai, *Cp, *Ci ;
    cxs_complex_t *Ax ;
    double *Cx ;
    if (!A || !A->x) return (NULL) ;    /* return if A NULL or pattern-only */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    triplet = (A->nz >= 0) ;            /* true if A is a triplet matrix */
    nz = triplet ? A->nz : Ap [n] ;
    C = cxs_dl_spalloc (A->m, n, A->nzmax, 1, triplet) ;
    if (!C) return (NULL) ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    nn = triplet ? nz : (n+1) ;
    for (p = 0 ; p < nz ; p++) Ci [p] = Ai [p] ;
    for (p = 0 ; p < nn ; p++) Cp [p] = Ap [p] ;
    for (p = 0 ; p < nz ; p++) Cx [p] = real ? creal (Ax [p]) : cimag (Ax [p]) ;
    if (triplet) C->nz = nz ;
    return (C) ;
}

/* convert from real to complex (cxs_long_t version) */
/* C = A if real is true, or C = i*A otherwise */
cxs_cl *cxs_l_complex (cxs_dl *A, cxs_long_t real)
{
    cxs_cl *C ;
    cxs_long_t n, triplet, nn, p, nz, *Ap, *Ai, *Cp, *Ci ;
    double *Ax ;
    cxs_complex_t *Cx ;
    if (!A || !A->x) return (NULL) ;    /* return if A NULL or pattern-only */
    n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    triplet = (A->nz >= 0) ;            /* true if A is a triplet matrix */
    nz = triplet ? A->nz : Ap [n] ;
    C = cxs_cl_spalloc (A->m, n, A->nzmax, 1, triplet) ;
    if (!C) return (NULL) ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    nn = triplet ? nz : (n+1) ;
    for (p = 0 ; p < nz ; p++) Ci [p] = Ai [p] ;
    for (p = 0 ; p < nn ; p++) Cp [p] = Ap [p] ;
    for (p = 0 ; p < nz ; p++) Cx [p] = real ? Ax [p] : (I * Ax [p]) ;
    if (triplet) C->nz = nz ;
    return (C) ;
}
