/* ========================================================================== */
/* CXSparse/Include/cxs.h file */
/* ========================================================================== */

/* This is the CXSparse/Include/cxs.h file.  It has the same name (cxs.h) as
   the CSparse/Include/cxs.h file.  The 'make install' for SuiteSparse installs
   CXSparse, and this file, instead of CSparse.  The two packages have the same
   cxs.h include filename, because CXSparse is a superset of CSparse.  Any user
   program that uses CSparse can rely on CXSparse instead, with no change to the
   user code.  The #include "cxs.h" line will work for both versions, in user
   code, and the function names and user-visible typedefs from CSparse all
   appear in CXSparse.  For experimenting and changing the package itself, I
   recommend using CSparse since it's simpler and easier to modify.  For
   using the package in production codes, I recommend CXSparse since it has
   more features (support for complex matrices, and both int and long
   versions).
 */

/* ========================================================================== */

#ifndef _CXS_H
#define _CXS_H
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
//#ifdef MATLAB_MEX_FILE
//#include "mex.h"
//#endif


#ifdef __cplusplus
#ifndef NCOMPLEX
#include <complex>
typedef std::complex<double> cxs_complex_t ;
#endif
extern "C" {
#else
#ifndef NCOMPLEX
#include <complex.h>
#define cxs_complex_t double _Complex
#endif
#endif

#define CXS_VER 3                    /* CXSparse Version */
#define CXS_SUBVER 1
#define CXS_SUBSUB 9
#define CXS_DATE "May 4, 2016"       /* CXSparse release date */
#define CXS_COPYRIGHT "Copyright (c) Timothy A. Davis, 2006-2016"
#define CXSPARSE

#include "SuiteSparse_config.h"
#define cxs_long_t       SuiteSparse_long
#define cxs_long_t_id    SuiteSparse_long_id
#define cxs_long_t_max   SuiteSparse_long_max

/* -------------------------------------------------------------------------- */
/* double/int version of CXSparse */
/* -------------------------------------------------------------------------- */

/* --- primary CSparse routines and data structures ------------------------- */

typedef struct cxs_di_sparse  /* matrix in compressed-column or triplet form */
{
    int nzmax ;     /* maximum number of entries */
    int m ;         /* number of rows */
    int n ;         /* number of columns */
    int *p ;        /* column pointers (size n+1) or col indices (size nzmax) */
    int *i ;        /* row indices, size nzmax */
    double *x ;     /* numerical values, size nzmax */
    int nz ;        /* # of entries in triplet matrix, -1 for compressed-col */
} cxs_di ;

cxs_di *cxs_di_add (const cxs_di *A, const cxs_di *B, double alpha, double beta) ;
int cxs_di_cholsol (int order, const cxs_di *A, double *b) ;
int cxs_di_dupl (cxs_di *A) ;
int cxs_di_entry (cxs_di *T, int i, int j, double x) ;
int cxs_di_lusol (int order, const cxs_di *A, double *b, double tol) ;
int cxs_di_gaxpy (const cxs_di *A, const double *x, double *y) ;
cxs_di *cxs_di_multiply (const cxs_di *A, const cxs_di *B) ;
int cxs_di_qrsol (int order, const cxs_di *A, double *b) ;
cxs_di *cxs_di_transpose (const cxs_di *A, int values) ;
cxs_di *cxs_di_compress (const cxs_di *T) ;
double cxs_di_norm (const cxs_di *A) ;
int cxs_di_print (const cxs_di *A, int brief) ;
cxs_di *cxs_di_load (FILE *f) ;

/* utilities */
void *cxs_di_calloc (int n, size_t size) ;
void *cxs_di_free (void *p) ;
void *cxs_di_realloc (void *p, int n, size_t size, int *ok) ;
cxs_di *cxs_di_spalloc (int m, int n, int nzmax, int values, int t) ;
cxs_di *cxs_di_spfree (cxs_di *A) ;
int cxs_di_sprealloc (cxs_di *A, int nzmax) ;
void *cxs_di_malloc (int n, size_t size) ;

/* --- secondary CSparse routines and data structures ----------------------- */

typedef struct cxs_di_symbolic  /* symbolic Cholesky, LU, or QR analysis */
{
    int *pinv ;     /* inverse row perm. for QR, fill red. perm for Chol */
    int *q ;        /* fill-reducing column permutation for LU and QR */
    int *parent ;   /* elimination tree for Cholesky and QR */
    int *cp ;       /* column pointers for Cholesky, row counts for QR */
    int *leftmost ; /* leftmost[i] = min(find(A(i,:))), for QR */
    int m2 ;        /* # of rows for QR, after adding fictitious rows */
    double lnz ;    /* # entries in L for LU or Cholesky; in V for QR */
    double unz ;    /* # entries in U for LU; in R for QR */
} cxs_dis ;

typedef struct cxs_di_numeric   /* numeric Cholesky, LU, or QR factorization */
{
    cxs_di *L ;      /* L for LU and Cholesky, V for QR */
    cxs_di *U ;      /* U for LU, r for QR, not used for Cholesky */
    int *pinv ;     /* partial pivoting for LU */
    double *B ;     /* beta [0..n-1] for QR */
} cxs_din ;

typedef struct cxs_di_dmperm_results    /* cxs_di_dmperm or cxs_di_scc output */
{
    int *p ;        /* size m, row permutation */
    int *q ;        /* size n, column permutation */
    int *r ;        /* size nb+1, block k is rows r[k] to r[k+1]-1 in A(p,q) */
    int *s ;        /* size nb+1, block k is cols s[k] to s[k+1]-1 in A(p,q) */
    int nb ;        /* # of blocks in fine dmperm decomposition */
    int rr [5] ;    /* coarse row decomposition */
    int cc [5] ;    /* coarse column decomposition */
} cxs_did ;

int *cxs_di_amd (int order, const cxs_di *A) ;
cxs_din *cxs_di_chol (const cxs_di *A, const cxs_dis *S) ;
cxs_did *cxs_di_dmperm (const cxs_di *A, int seed) ;
int cxs_di_droptol (cxs_di *A, double tol) ;
int cxs_di_dropzeros (cxs_di *A) ;
int cxs_di_happly (const cxs_di *V, int i, double beta, double *x) ;
int cxs_di_ipvec (const int *p, const double *b, double *x, int n) ;
int cxs_di_lsolve (const cxs_di *L, double *x) ;
int cxs_di_ltsolve (const cxs_di *L, double *x) ;
cxs_din *cxs_di_lu (const cxs_di *A, const cxs_dis *S, double tol) ;
cxs_di *cxs_di_permute (const cxs_di *A, const int *pinv, const int *q,
    int values) ;
int *cxs_di_pinv (const int *p, int n) ;
int cxs_di_pvec (const int *p, const double *b, double *x, int n) ;
cxs_din *cxs_di_qr (const cxs_di *A, const cxs_dis *S) ;
cxs_dis *cxs_di_schol (int order, const cxs_di *A) ;
cxs_dis *cxs_di_sqr (int order, const cxs_di *A, int qr) ;
cxs_di *cxs_di_symperm (const cxs_di *A, const int *pinv, int values) ;
int cxs_di_usolve (const cxs_di *U, double *x) ;
int cxs_di_utsolve (const cxs_di *U, double *x) ;
int cxs_di_updown (cxs_di *L, int sigma, const cxs_di *C, const int *parent) ;

/* utilities */
cxs_dis *cxs_di_sfree (cxs_dis *S) ;
cxs_din *cxs_di_nfree (cxs_din *N) ;
cxs_did *cxs_di_dfree (cxs_did *D) ;

/* --- tertiary CSparse routines -------------------------------------------- */

int *cxs_di_counts (const cxs_di *A, const int *parent, const int *post,
    int ata) ;
double cxs_di_cumsum (int *p, int *c, int n) ;
int cxs_di_dfs (int j, cxs_di *G, int top, int *xi, int *pstack,
    const int *pinv) ;
int *cxs_di_etree (const cxs_di *A, int ata) ;
int cxs_di_fkeep (cxs_di *A, int (*fkeep) (int, int, double, void *),
    void *other) ;
double cxs_di_house (double *x, double *beta, int n) ;
int *cxs_di_maxtrans (const cxs_di *A, int seed) ;
int *cxs_di_post (const int *parent, int n) ;
cxs_did *cxs_di_scc (cxs_di *A) ;
int cxs_di_scatter (const cxs_di *A, int j, double beta, int *w, double *x,
    int mark, cxs_di *C, int nz) ;
int cxs_di_tdfs (int j, int k, int *head, const int *next, int *post,
    int *stack) ;
int cxs_di_leaf (int i, int j, const int *first, int *maxfirst, int *prevleaf,
    int *ancestor, int *jleaf) ;
int cxs_di_reach (cxs_di *G, const cxs_di *B, int k, int *xi, const int *pinv) ;
int cxs_di_spsolve (cxs_di *L, const cxs_di *B, int k, int *xi, double *x,
    const int *pinv, int lo) ;
int cxs_di_ereach (const cxs_di *A, int k, const int *parent, int *s, int *w) ;
int *cxs_di_randperm (int n, int seed) ;

/* utilities */
cxs_did *cxs_di_dalloc (int m, int n) ;
cxs_di *cxs_di_done (cxs_di *C, void *w, void *x, int ok) ;
int *cxs_di_idone (int *p, cxs_di *C, void *w, int ok) ;
cxs_din *cxs_di_ndone (cxs_din *N, cxs_di *C, void *w, void *x, int ok) ;
cxs_did *cxs_di_ddone (cxs_did *D, cxs_di *C, void *w, int ok) ;


/* -------------------------------------------------------------------------- */
/* double/cxs_long_t version of CXSparse */
/* -------------------------------------------------------------------------- */

/* --- primary CSparse routines and data structures ------------------------- */

typedef struct cxs_dl_sparse  /* matrix in compressed-column or triplet form */
{
    cxs_long_t nzmax ; /* maximum number of entries */
    cxs_long_t m ;     /* number of rows */
    cxs_long_t n ;     /* number of columns */
    cxs_long_t *p ;    /* column pointers (size n+1) or col indlces (size nzmax) */
    cxs_long_t *i ;    /* row indices, size nzmax */
    double *x ;     /* numerical values, size nzmax */
    cxs_long_t nz ;    /* # of entries in triplet matrix, -1 for compressed-col */
} cxs_dl ;

cxs_dl *cxs_dl_add (const cxs_dl *A, const cxs_dl *B, double alpha, double beta) ;
cxs_long_t cxs_dl_cholsol (cxs_long_t order, const cxs_dl *A, double *b) ;
cxs_long_t cxs_dl_dupl (cxs_dl *A) ;
cxs_long_t cxs_dl_entry (cxs_dl *T, cxs_long_t i, cxs_long_t j, double x) ;
cxs_long_t cxs_dl_lusol (cxs_long_t order, const cxs_dl *A, double *b, double tol) ;
cxs_long_t cxs_dl_gaxpy (const cxs_dl *A, const double *x, double *y) ;
cxs_dl *cxs_dl_multiply (const cxs_dl *A, const cxs_dl *B) ;
cxs_long_t cxs_dl_qrsol (cxs_long_t order, const cxs_dl *A, double *b) ;
cxs_dl *cxs_dl_transpose (const cxs_dl *A, cxs_long_t values) ;
cxs_dl *cxs_dl_compress (const cxs_dl *T) ;
double cxs_dl_norm (const cxs_dl *A) ;
cxs_long_t cxs_dl_print (const cxs_dl *A, cxs_long_t brief) ;
cxs_dl *cxs_dl_load (FILE *f) ;

/* utilities */
void *cxs_dl_calloc (cxs_long_t n, size_t size) ;
void *cxs_dl_free (void *p) ;
void *cxs_dl_realloc (void *p, cxs_long_t n, size_t size, cxs_long_t *ok) ;
cxs_dl *cxs_dl_spalloc (cxs_long_t m, cxs_long_t n, cxs_long_t nzmax, cxs_long_t values,
    cxs_long_t t) ;
cxs_dl *cxs_dl_spfree (cxs_dl *A) ;
cxs_long_t cxs_dl_sprealloc (cxs_dl *A, cxs_long_t nzmax) ;
void *cxs_dl_malloc (cxs_long_t n, size_t size) ;

/* --- secondary CSparse routines and data structures ----------------------- */

typedef struct cxs_dl_symbolic  /* symbolic Cholesky, LU, or QR analysis */
{
    cxs_long_t *pinv ;     /* inverse row perm. for QR, fill red. perm for Chol */
    cxs_long_t *q ;        /* fill-reducing column permutation for LU and QR */
    cxs_long_t *parent ;   /* elimination tree for Cholesky and QR */
    cxs_long_t *cp ;       /* column pointers for Cholesky, row counts for QR */
    cxs_long_t *leftmost ; /* leftmost[i] = min(find(A(i,:))), for QR */
    cxs_long_t m2 ;        /* # of rows for QR, after adding fictitious rows */
    double lnz ;        /* # entries in L for LU or Cholesky; in V for QR */
    double unz ;        /* # entries in U for LU; in R for QR */
} cxs_dls ;

typedef struct cxs_dl_numeric   /* numeric Cholesky, LU, or QR factorization */
{
    cxs_dl *L ;      /* L for LU and Cholesky, V for QR */
    cxs_dl *U ;      /* U for LU, r for QR, not used for Cholesky */
    cxs_long_t *pinv ; /* partial pivoting for LU */
    double *B ;     /* beta [0..n-1] for QR */
} cxs_dln ;

typedef struct cxs_dl_dmperm_results    /* cxs_dl_dmperm or cxs_dl_scc output */
{
    cxs_long_t *p ;    /* size m, row permutation */
    cxs_long_t *q ;    /* size n, column permutation */
    cxs_long_t *r ;    /* size nb+1, block k is rows r[k] to r[k+1]-1 in A(p,q) */
    cxs_long_t *s ;    /* size nb+1, block k is cols s[k] to s[k+1]-1 in A(p,q) */
    cxs_long_t nb ;    /* # of blocks in fine dmperm decomposition */
    cxs_long_t rr [5] ;    /* coarse row decomposition */
    cxs_long_t cc [5] ;    /* coarse column decomposition */
} cxs_dld ;

cxs_long_t *cxs_dl_amd (cxs_long_t order, const cxs_dl *A) ;
cxs_dln *cxs_dl_chol (const cxs_dl *A, const cxs_dls *S) ;
cxs_dld *cxs_dl_dmperm (const cxs_dl *A, cxs_long_t seed) ;
cxs_long_t cxs_dl_droptol (cxs_dl *A, double tol) ;
cxs_long_t cxs_dl_dropzeros (cxs_dl *A) ;
cxs_long_t cxs_dl_happly (const cxs_dl *V, cxs_long_t i, double beta, double *x) ;
cxs_long_t cxs_dl_ipvec (const cxs_long_t *p, const double *b, double *x, cxs_long_t n) ;
cxs_long_t cxs_dl_lsolve (const cxs_dl *L, double *x) ;
cxs_long_t cxs_dl_ltsolve (const cxs_dl *L, double *x) ;
cxs_dln *cxs_dl_lu (const cxs_dl *A, const cxs_dls *S, double tol) ;
cxs_dl *cxs_dl_permute (const cxs_dl *A, const cxs_long_t *pinv, const cxs_long_t *q,
    cxs_long_t values) ;
cxs_long_t *cxs_dl_pinv (const cxs_long_t *p, cxs_long_t n) ;
cxs_long_t cxs_dl_pvec (const cxs_long_t *p, const double *b, double *x, cxs_long_t n) ;
cxs_dln *cxs_dl_qr (const cxs_dl *A, const cxs_dls *S) ;
cxs_dls *cxs_dl_schol (cxs_long_t order, const cxs_dl *A) ;
cxs_dls *cxs_dl_sqr (cxs_long_t order, const cxs_dl *A, cxs_long_t qr) ;
cxs_dl *cxs_dl_symperm (const cxs_dl *A, const cxs_long_t *pinv, cxs_long_t values) ;
cxs_long_t cxs_dl_usolve (const cxs_dl *U, double *x) ;
cxs_long_t cxs_dl_utsolve (const cxs_dl *U, double *x) ;
cxs_long_t cxs_dl_updown (cxs_dl *L, cxs_long_t sigma, const cxs_dl *C,
    const cxs_long_t *parent) ;

/* utilities */
cxs_dls *cxs_dl_sfree (cxs_dls *S) ;
cxs_dln *cxs_dl_nfree (cxs_dln *N) ;
cxs_dld *cxs_dl_dfree (cxs_dld *D) ;

/* --- tertiary CSparse routines -------------------------------------------- */

cxs_long_t *cxs_dl_counts (const cxs_dl *A, const cxs_long_t *parent,
    const cxs_long_t *post, cxs_long_t ata) ;
double cxs_dl_cumsum (cxs_long_t *p, cxs_long_t *c, cxs_long_t n) ;
cxs_long_t cxs_dl_dfs (cxs_long_t j, cxs_dl *G, cxs_long_t top, cxs_long_t *xi,
    cxs_long_t *pstack, const cxs_long_t *pinv) ;
cxs_long_t *cxs_dl_etree (const cxs_dl *A, cxs_long_t ata) ;
cxs_long_t cxs_dl_fkeep (cxs_dl *A,
    cxs_long_t (*fkeep) (cxs_long_t, cxs_long_t, double, void *), void *other) ;
double cxs_dl_house (double *x, double *beta, cxs_long_t n) ;
cxs_long_t *cxs_dl_maxtrans (const cxs_dl *A, cxs_long_t seed) ;
cxs_long_t *cxs_dl_post (const cxs_long_t *parent, cxs_long_t n) ;
cxs_dld *cxs_dl_scc (cxs_dl *A) ;
cxs_long_t cxs_dl_scatter (const cxs_dl *A, cxs_long_t j, double beta, cxs_long_t *w,
    double *x, cxs_long_t mark,cxs_dl *C, cxs_long_t nz) ;
cxs_long_t cxs_dl_tdfs (cxs_long_t j, cxs_long_t k, cxs_long_t *head, const cxs_long_t *next,
    cxs_long_t *post, cxs_long_t *stack) ;
cxs_long_t cxs_dl_leaf (cxs_long_t i, cxs_long_t j, const cxs_long_t *first,
    cxs_long_t *maxfirst, cxs_long_t *prevleaf, cxs_long_t *ancestor, cxs_long_t *jleaf) ;
cxs_long_t cxs_dl_reach (cxs_dl *G, const cxs_dl *B, cxs_long_t k, cxs_long_t *xi,
    const cxs_long_t *pinv) ;
cxs_long_t cxs_dl_spsolve (cxs_dl *L, const cxs_dl *B, cxs_long_t k, cxs_long_t *xi,
    double *x, const cxs_long_t *pinv, cxs_long_t lo) ;
cxs_long_t cxs_dl_ereach (const cxs_dl *A, cxs_long_t k, const cxs_long_t *parent,
    cxs_long_t *s, cxs_long_t *w) ;
cxs_long_t *cxs_dl_randperm (cxs_long_t n, cxs_long_t seed) ;

/* utilities */
cxs_dld *cxs_dl_dalloc (cxs_long_t m, cxs_long_t n) ;
cxs_dl *cxs_dl_done (cxs_dl *C, void *w, void *x, cxs_long_t ok) ;
cxs_long_t *cxs_dl_idone (cxs_long_t *p, cxs_dl *C, void *w, cxs_long_t ok) ;
cxs_dln *cxs_dl_ndone (cxs_dln *N, cxs_dl *C, void *w, void *x, cxs_long_t ok) ;
cxs_dld *cxs_dl_ddone (cxs_dld *D, cxs_dl *C, void *w, cxs_long_t ok) ;


/* -------------------------------------------------------------------------- */
/* complex/int version of CXSparse */
/* -------------------------------------------------------------------------- */

#ifndef NCOMPLEX

/* --- primary CSparse routines and data structures ------------------------- */

typedef struct cxs_ci_sparse  /* matrix in compressed-column or triplet form */
{
    int nzmax ;     /* maximum number of entries */
    int m ;         /* number of rows */
    int n ;         /* number of columns */
    int *p ;        /* column pointers (size n+1) or col indices (size nzmax) */
    int *i ;        /* row indices, size nzmax */
    cxs_complex_t *x ;    /* numerical values, size nzmax */
    int nz ;        /* # of entries in triplet matrix, -1 for compressed-col */
} cxs_ci ;

cxs_ci *cxs_ci_add (const cxs_ci *A, const cxs_ci *B, cxs_complex_t alpha,
    cxs_complex_t beta) ;
int cxs_ci_cholsol (int order, const cxs_ci *A, cxs_complex_t *b) ;
int cxs_ci_dupl (cxs_ci *A) ;
int cxs_ci_entry (cxs_ci *T, int i, int j, cxs_complex_t x) ;
int cxs_ci_lusol (int order, const cxs_ci *A, cxs_complex_t *b, double tol) ;
int cxs_ci_gaxpy (const cxs_ci *A, const cxs_complex_t *x, cxs_complex_t *y) ;
cxs_ci *cxs_ci_multiply (const cxs_ci *A, const cxs_ci *B) ;
int cxs_ci_qrsol (int order, const cxs_ci *A, cxs_complex_t *b) ;
cxs_ci *cxs_ci_transpose (const cxs_ci *A, int values) ;
cxs_ci *cxs_ci_compress (const cxs_ci *T) ;
double cxs_ci_norm (const cxs_ci *A) ;
int cxs_ci_print (const cxs_ci *A, int brief) ;
cxs_ci *cxs_ci_load (FILE *f) ;

/* utilities */
void *cxs_ci_calloc (int n, size_t size) ;
void *cxs_ci_free (void *p) ;
void *cxs_ci_realloc (void *p, int n, size_t size, int *ok) ;
cxs_ci *cxs_ci_spalloc (int m, int n, int nzmax, int values, int t) ;
cxs_ci *cxs_ci_spfree (cxs_ci *A) ;
int cxs_ci_sprealloc (cxs_ci *A, int nzmax) ;
void *cxs_ci_malloc (int n, size_t size) ;

/* --- secondary CSparse routines and data structures ----------------------- */

typedef struct cxs_ci_symbolic  /* symbolic Cholesky, LU, or QR analysis */
{
    int *pinv ;     /* inverse row perm. for QR, fill red. perm for Chol */
    int *q ;        /* fill-reducing column permutation for LU and QR */
    int *parent ;   /* elimination tree for Cholesky and QR */
    int *cp ;       /* column pointers for Cholesky, row counts for QR */
    int *leftmost ; /* leftmost[i] = min(find(A(i,:))), for QR */
    int m2 ;        /* # of rows for QR, after adding fictitious rows */
    double lnz ;    /* # entries in L for LU or Cholesky; in V for QR */
    double unz ;    /* # entries in U for LU; in R for QR */
} cxs_cis ;

typedef struct cxs_ci_numeric   /* numeric Cholesky, LU, or QR factorization */
{
    cxs_ci *L ;      /* L for LU and Cholesky, V for QR */
    cxs_ci *U ;      /* U for LU, r for QR, not used for Cholesky */
    int *pinv ;     /* partial pivoting for LU */
    double *B ;     /* beta [0..n-1] for QR */
} cxs_cin ;

typedef struct cxs_ci_dmperm_results    /* cxs_ci_dmperm or cxs_ci_scc output */
{
    int *p ;        /* size m, row permutation */
    int *q ;        /* size n, column permutation */
    int *r ;        /* size nb+1, block k is rows r[k] to r[k+1]-1 in A(p,q) */
    int *s ;        /* size nb+1, block k is cols s[k] to s[k+1]-1 in A(p,q) */
    int nb ;        /* # of blocks in fine dmperm decomposition */
    int rr [5] ;    /* coarse row decomposition */
    int cc [5] ;    /* coarse column decomposition */
} cxs_cid ;

int *cxs_ci_amd (int order, const cxs_ci *A) ;
cxs_cin *cxs_ci_chol (const cxs_ci *A, const cxs_cis *S) ;
cxs_cid *cxs_ci_dmperm (const cxs_ci *A, int seed) ;
int cxs_ci_droptol (cxs_ci *A, double tol) ;
int cxs_ci_dropzeros (cxs_ci *A) ;
int cxs_ci_happly (const cxs_ci *V, int i, double beta, cxs_complex_t *x) ;
int cxs_ci_ipvec (const int *p, const cxs_complex_t *b, cxs_complex_t *x, int n) ;
int cxs_ci_lsolve (const cxs_ci *L, cxs_complex_t *x) ;
int cxs_ci_ltsolve (const cxs_ci *L, cxs_complex_t *x) ;
cxs_cin *cxs_ci_lu (const cxs_ci *A, const cxs_cis *S, double tol) ;
cxs_ci *cxs_ci_permute (const cxs_ci *A, const int *pinv, const int *q,
    int values) ;
int *cxs_ci_pinv (const int *p, int n) ;
int cxs_ci_pvec (const int *p, const cxs_complex_t *b, cxs_complex_t *x, int n) ;
cxs_cin *cxs_ci_qr (const cxs_ci *A, const cxs_cis *S) ;
cxs_cis *cxs_ci_schol (int order, const cxs_ci *A) ;
cxs_cis *cxs_ci_sqr (int order, const cxs_ci *A, int qr) ;
cxs_ci *cxs_ci_symperm (const cxs_ci *A, const int *pinv, int values) ;
int cxs_ci_usolve (const cxs_ci *U, cxs_complex_t *x) ;
int cxs_ci_utsolve (const cxs_ci *U, cxs_complex_t *x) ;
int cxs_ci_updown (cxs_ci *L, int sigma, const cxs_ci *C, const int *parent) ;

/* utilities */
cxs_cis *cxs_ci_sfree (cxs_cis *S) ;
cxs_cin *cxs_ci_nfree (cxs_cin *N) ;
cxs_cid *cxs_ci_dfree (cxs_cid *D) ;

/* --- tertiary CSparse routines -------------------------------------------- */

int *cxs_ci_counts (const cxs_ci *A, const int *parent, const int *post,
    int ata) ;
double cxs_ci_cumsum (int *p, int *c, int n) ;
int cxs_ci_dfs (int j, cxs_ci *G, int top, int *xi, int *pstack,
    const int *pinv) ;
int *cxs_ci_etree (const cxs_ci *A, int ata) ;
int cxs_ci_fkeep (cxs_ci *A, int (*fkeep) (int, int, cxs_complex_t, void *),
    void *other) ;
cxs_complex_t cxs_ci_house (cxs_complex_t *x, double *beta, int n) ;
int *cxs_ci_maxtrans (const cxs_ci *A, int seed) ;
int *cxs_ci_post (const int *parent, int n) ;
cxs_cid *cxs_ci_scc (cxs_ci *A) ;
int cxs_ci_scatter (const cxs_ci *A, int j, cxs_complex_t beta, int *w,
    cxs_complex_t *x, int mark,cxs_ci *C, int nz) ;
int cxs_ci_tdfs (int j, int k, int *head, const int *next, int *post,
    int *stack) ;
int cxs_ci_leaf (int i, int j, const int *first, int *maxfirst, int *prevleaf,
    int *ancestor, int *jleaf) ;
int cxs_ci_reach (cxs_ci *G, const cxs_ci *B, int k, int *xi, const int *pinv) ;
int cxs_ci_spsolve (cxs_ci *L, const cxs_ci *B, int k, int *xi,
    cxs_complex_t *x, const int *pinv, int lo) ;
int cxs_ci_ereach (const cxs_ci *A, int k, const int *parent, int *s, int *w) ;
int *cxs_ci_randperm (int n, int seed) ;

/* utilities */
cxs_cid *cxs_ci_dalloc (int m, int n) ;
cxs_ci *cxs_ci_done (cxs_ci *C, void *w, void *x, int ok) ;
int *cxs_ci_idone (int *p, cxs_ci *C, void *w, int ok) ;
cxs_cin *cxs_ci_ndone (cxs_cin *N, cxs_ci *C, void *w, void *x, int ok) ;
cxs_cid *cxs_ci_ddone (cxs_cid *D, cxs_ci *C, void *w, int ok) ;


/* -------------------------------------------------------------------------- */
/* complex/cxs_long_t version of CXSparse */
/* -------------------------------------------------------------------------- */

/* --- primary CSparse routines and data structures ------------------------- */

typedef struct cxs_cl_sparse  /* matrix in compressed-column or triplet form */
{
    cxs_long_t nzmax ; /* maximum number of entries */
    cxs_long_t m ;     /* number of rows */
    cxs_long_t n ;     /* number of columns */
    cxs_long_t *p ;    /* column pointers (size n+1) or col indlces (size nzmax) */
    cxs_long_t *i ;    /* row indices, size nzmax */
    cxs_complex_t *x ;    /* numerical values, size nzmax */
    cxs_long_t nz ;    /* # of entries in triplet matrix, -1 for compressed-col */
} cxs_cl ;

cxs_cl *cxs_cl_add (const cxs_cl *A, const cxs_cl *B, cxs_complex_t alpha,
    cxs_complex_t beta) ;
cxs_long_t cxs_cl_cholsol (cxs_long_t order, const cxs_cl *A, cxs_complex_t *b) ;
cxs_long_t cxs_cl_dupl (cxs_cl *A) ;
cxs_long_t cxs_cl_entry (cxs_cl *T, cxs_long_t i, cxs_long_t j, cxs_complex_t x) ;
cxs_long_t cxs_cl_lusol (cxs_long_t order, const cxs_cl *A, cxs_complex_t *b,
    double tol) ;
cxs_long_t cxs_cl_gaxpy (const cxs_cl *A, const cxs_complex_t *x, cxs_complex_t *y) ;
cxs_cl *cxs_cl_multiply (const cxs_cl *A, const cxs_cl *B) ;
cxs_long_t cxs_cl_qrsol (cxs_long_t order, const cxs_cl *A, cxs_complex_t *b) ;
cxs_cl *cxs_cl_transpose (const cxs_cl *A, cxs_long_t values) ;
cxs_cl *cxs_cl_compress (const cxs_cl *T) ;
double cxs_cl_norm (const cxs_cl *A) ;
cxs_long_t cxs_cl_print (const cxs_cl *A, cxs_long_t brief) ;
cxs_cl *cxs_cl_load (FILE *f) ;

/* utilities */
void *cxs_cl_calloc (cxs_long_t n, size_t size) ;
void *cxs_cl_free (void *p) ;
void *cxs_cl_realloc (void *p, cxs_long_t n, size_t size, cxs_long_t *ok) ;
cxs_cl *cxs_cl_spalloc (cxs_long_t m, cxs_long_t n, cxs_long_t nzmax, cxs_long_t values,
    cxs_long_t t) ;
cxs_cl *cxs_cl_spfree (cxs_cl *A) ;
cxs_long_t cxs_cl_sprealloc (cxs_cl *A, cxs_long_t nzmax) ;
void *cxs_cl_malloc (cxs_long_t n, size_t size) ;

/* --- secondary CSparse routines and data structures ----------------------- */

typedef struct cxs_cl_symbolic  /* symbolic Cholesky, LU, or QR analysis */
{
    cxs_long_t *pinv ;     /* inverse row perm. for QR, fill red. perm for Chol */
    cxs_long_t *q ;        /* fill-reducing column permutation for LU and QR */
    cxs_long_t *parent ;   /* elimination tree for Cholesky and QR */
    cxs_long_t *cp ;       /* column pointers for Cholesky, row counts for QR */
    cxs_long_t *leftmost ; /* leftmost[i] = min(find(A(i,:))), for QR */
    cxs_long_t m2 ;        /* # of rows for QR, after adding fictitious rows */
    double lnz ;        /* # entries in L for LU or Cholesky; in V for QR */
    double unz ;        /* # entries in U for LU; in R for QR */
} cxs_cls ;

typedef struct cxs_cl_numeric   /* numeric Cholesky, LU, or QR factorization */
{
    cxs_cl *L ;          /* L for LU and Cholesky, V for QR */
    cxs_cl *U ;          /* U for LU, r for QR, not used for Cholesky */
    cxs_long_t *pinv ;     /* partial pivoting for LU */
    double *B ;         /* beta [0..n-1] for QR */
} cxs_cln ;

typedef struct cxs_cl_dmperm_results    /* cxs_cl_dmperm or cxs_cl_scc output */
{
    cxs_long_t *p ;    /* size m, row permutation */
    cxs_long_t *q ;    /* size n, column permutation */
    cxs_long_t *r ;    /* size nb+1, block k is rows r[k] to r[k+1]-1 in A(p,q) */
    cxs_long_t *s ;    /* size nb+1, block k is cols s[k] to s[k+1]-1 in A(p,q) */
    cxs_long_t nb ;    /* # of blocks in fine dmperm decomposition */
    cxs_long_t rr [5] ;   /* coarse row decomposition */
    cxs_long_t cc [5] ;   /* coarse column decomposition */
} cxs_cld ;

cxs_long_t *cxs_cl_amd (cxs_long_t order, const cxs_cl *A) ;
cxs_cln *cxs_cl_chol (const cxs_cl *A, const cxs_cls *S) ;
cxs_cld *cxs_cl_dmperm (const cxs_cl *A, cxs_long_t seed) ;
cxs_long_t cxs_cl_droptol (cxs_cl *A, double tol) ;
cxs_long_t cxs_cl_dropzeros (cxs_cl *A) ;
cxs_long_t cxs_cl_happly (const cxs_cl *V, cxs_long_t i, double beta, cxs_complex_t *x) ;
cxs_long_t cxs_cl_ipvec (const cxs_long_t *p, const cxs_complex_t *b,
    cxs_complex_t *x, cxs_long_t n) ;
cxs_long_t cxs_cl_lsolve (const cxs_cl *L, cxs_complex_t *x) ;
cxs_long_t cxs_cl_ltsolve (const cxs_cl *L, cxs_complex_t *x) ;
cxs_cln *cxs_cl_lu (const cxs_cl *A, const cxs_cls *S, double tol) ;
cxs_cl *cxs_cl_permute (const cxs_cl *A, const cxs_long_t *pinv, const cxs_long_t *q,
    cxs_long_t values) ;
cxs_long_t *cxs_cl_pinv (const cxs_long_t *p, cxs_long_t n) ;
cxs_long_t cxs_cl_pvec (const cxs_long_t *p, const cxs_complex_t *b,
    cxs_complex_t *x, cxs_long_t n) ;
cxs_cln *cxs_cl_qr (const cxs_cl *A, const cxs_cls *S) ;
cxs_cls *cxs_cl_schol (cxs_long_t order, const cxs_cl *A) ;
cxs_cls *cxs_cl_sqr (cxs_long_t order, const cxs_cl *A, cxs_long_t qr) ;
cxs_cl *cxs_cl_symperm (const cxs_cl *A, const cxs_long_t *pinv, cxs_long_t values) ;
cxs_long_t cxs_cl_usolve (const cxs_cl *U, cxs_complex_t *x) ;
cxs_long_t cxs_cl_utsolve (const cxs_cl *U, cxs_complex_t *x) ;
cxs_long_t cxs_cl_updown (cxs_cl *L, cxs_long_t sigma, const cxs_cl *C,
    const cxs_long_t *parent) ;

/* utilities */
cxs_cls *cxs_cl_sfree (cxs_cls *S) ;
cxs_cln *cxs_cl_nfree (cxs_cln *N) ;
cxs_cld *cxs_cl_dfree (cxs_cld *D) ;

/* --- tertiary CSparse routines -------------------------------------------- */

cxs_long_t *cxs_cl_counts (const cxs_cl *A, const cxs_long_t *parent,
    const cxs_long_t *post, cxs_long_t ata) ;
double cxs_cl_cumsum (cxs_long_t *p, cxs_long_t *c, cxs_long_t n) ;
cxs_long_t cxs_cl_dfs (cxs_long_t j, cxs_cl *G, cxs_long_t top, cxs_long_t *xi,
    cxs_long_t *pstack, const cxs_long_t *pinv) ;
cxs_long_t *cxs_cl_etree (const cxs_cl *A, cxs_long_t ata) ;
cxs_long_t cxs_cl_fkeep (cxs_cl *A,
    cxs_long_t (*fkeep) (cxs_long_t, cxs_long_t, cxs_complex_t, void *), void *other) ;
cxs_complex_t cxs_cl_house (cxs_complex_t *x, double *beta, cxs_long_t n) ;
cxs_long_t *cxs_cl_maxtrans (const cxs_cl *A, cxs_long_t seed) ;
cxs_long_t *cxs_cl_post (const cxs_long_t *parent, cxs_long_t n) ;
cxs_cld *cxs_cl_scc (cxs_cl *A) ;
cxs_long_t cxs_cl_scatter (const cxs_cl *A, cxs_long_t j, cxs_complex_t beta,
    cxs_long_t *w, cxs_complex_t *x, cxs_long_t mark,cxs_cl *C, cxs_long_t nz) ;
cxs_long_t cxs_cl_tdfs (cxs_long_t j, cxs_long_t k, cxs_long_t *head, const cxs_long_t *next,
    cxs_long_t *post, cxs_long_t *stack) ;
cxs_long_t cxs_cl_leaf (cxs_long_t i, cxs_long_t j, const cxs_long_t *first,
    cxs_long_t *maxfirst, cxs_long_t *prevleaf, cxs_long_t *ancestor, cxs_long_t *jleaf) ;
cxs_long_t cxs_cl_reach (cxs_cl *G, const cxs_cl *B, cxs_long_t k, cxs_long_t *xi,
    const cxs_long_t *pinv) ;
cxs_long_t cxs_cl_spsolve (cxs_cl *L, const cxs_cl *B, cxs_long_t k, cxs_long_t *xi,
    cxs_complex_t *x, const cxs_long_t *pinv, cxs_long_t lo) ;
cxs_long_t cxs_cl_ereach (const cxs_cl *A, cxs_long_t k, const cxs_long_t *parent,
    cxs_long_t *s, cxs_long_t *w) ;
cxs_long_t *cxs_cl_randperm (cxs_long_t n, cxs_long_t seed) ;

/* utilities */
cxs_cld *cxs_cl_dalloc (cxs_long_t m, cxs_long_t n) ;
cxs_cl *cxs_cl_done (cxs_cl *C, void *w, void *x, cxs_long_t ok) ;
cxs_long_t *cxs_cl_idone (cxs_long_t *p, cxs_cl *C, void *w, cxs_long_t ok) ;
cxs_cln *cxs_cl_ndone (cxs_cln *N, cxs_cl *C, void *w, void *x, cxs_long_t ok) ;
cxs_cld *cxs_cl_ddone (cxs_cld *D, cxs_cl *C, void *w, cxs_long_t ok) ;

#endif

/* -------------------------------------------------------------------------- */
/* Macros for constructing each version of CXSparse */
/* -------------------------------------------------------------------------- */
#ifdef CXS_LONG
    #undef CXS_LONG
#endif // CXS_LONG
#ifndef CXS_COMPLEX
    #define CXS_COMPLEX
#endif // CXS_COMPLEX

#ifdef CXS_LONG
#define CXS_INT cxs_long_t
#define CXS_INT_MAX cxs_long_t_max
#define CXS_ID cxs_long_t_id
#ifdef CXS_COMPLEX
#define CXS_ENTRY cxs_complex_t
#define CXS_NAME(nm) cxs_cl ## nm
#define cxs cxs_cl
#else
#define CXS_ENTRY double
#define CXS_NAME(nm) cxs_dl ## nm
#define cxs cxs_dl
#endif
#else
#define CXS_INT int
#define CXS_INTs_MAX INT_MAX
#define CXS_ID "%d"
#ifdef CXS_COMPLEX
#define CXS_ENTRY cxs_complex_t
#define CXS_NAME(nm) cxs_ci ## nm
#define cxs cxs_ci
#else
#define CXS_ENTRY double
#define CXS_NAME(nm) cxs_di ## nm
#define cxs cxs_di
#endif
#endif

#ifdef CXS_COMPLEX
#define CXS_REAL(x) creal(x)
#define CXS_IMAG(x) cimag(x)
#define CXS_CONJ(x) conj(x)
#define CXS_ABS(x) cabs(x)
#else
#define CXS_REAL(x) (x)
#define CXS_IMAG(x) (0.)
#define CXS_CONJ(x) (x)
#define CXS_ABS(x) fabs(x)
#endif

#define CXS_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define CXS_MIN(a,b) (((a) < (b)) ? (a) : (b))
#define CXS_FLIP(i) (-(i)-2)
#define CXS_UNFLIP(i) (((i) < 0) ? CXS_FLIP(i) : (i))
#define CXS_MARKED(w,j) (w [j] < 0)
#define CXS_MARK(w,j) { w [j] = CXS_FLIP (w [j]) ; }
#define CXS_CSC(A) (A && (A->nz == -1))
#define CXS_TRIPLET(A) (A && (A->nz >= 0))

/* --- primary CSparse routines and data structures ------------------------- */

#define cxs_add CXS_NAME (_add)
#define cxs_cholsol CXS_NAME (_cholsol)
#define cxs_dupl CXS_NAME (_dupl)
#define cxs_entry CXS_NAME (_entry)
#define cxs_lusol CXS_NAME (_lusol)
#define cxs_gaxpy CXS_NAME (_gaxpy)
#define cxs_multiply CXS_NAME (_multiply)
#define cxs_qrsol CXS_NAME (_qrsol)
#define cxs_transpose CXS_NAME (_transpose)
#define cxs_compress CXS_NAME (_compress)
#define cxs_norm CXS_NAME (_norm)
#define cxs_print CXS_NAME (_print)
#define cxs_load CXS_NAME (_load)

/* utilities */
#define cxs_calloc CXS_NAME (_calloc)
#define cxs_free CXS_NAME (_free)
#define cxs_realloc CXS_NAME (_realloc)
#define cxs_spalloc CXS_NAME (_spalloc)
#define cxs_spfree CXS_NAME (_spfree)
#define cxs_sprealloc CXS_NAME (_sprealloc)
#define cxs_malloc CXS_NAME (_malloc)

/* --- secondary CSparse routines and data structures ----------------------- */
#define cxss CXS_NAME (s)
#define cxsn CXS_NAME (n)
#define cxsd CXS_NAME (d)

#define cxs_amd CXS_NAME (_amd)
#define cxs_chol CXS_NAME (_chol)
#define cxs_dmperm CXS_NAME (_dmperm)
#define cxs_droptol CXS_NAME (_droptol)
#define cxs_dropzeros CXS_NAME (_dropzeros)
#define cxs_happly CXS_NAME (_happly)
#define cxs_ipvec CXS_NAME (_ipvec)
#define cxs_lsolve CXS_NAME (_lsolve)
#define cxs_ltsolve CXS_NAME (_ltsolve)
#define cxs_lu CXS_NAME (_lu)
#define cxs_permute CXS_NAME (_permute)
#define cxs_pinv CXS_NAME (_pinv)
#define cxs_pvec CXS_NAME (_pvec)
#define cxs_qr CXS_NAME (_qr)
#define cxs_schol CXS_NAME (_schol)
#define cxs_sqr CXS_NAME (_sqr)
#define cxs_symperm CXS_NAME (_symperm)
#define cxs_usolve CXS_NAME (_usolve)
#define cxs_utsolve CXS_NAME (_utsolve)
#define cxs_updown CXS_NAME (_updown)

/* utilities */
#define cxs_sfree CXS_NAME (_sfree)
#define cxs_nfree CXS_NAME (_nfree)
#define cxs_dfree CXS_NAME (_dfree)

/* --- tertiary CSparse routines -------------------------------------------- */
#define cxs_counts CXS_NAME (_counts)
#define cxs_cumsum CXS_NAME (_cumsum)
#define cxs_dfs CXS_NAME (_dfs)
#define cxs_etree CXS_NAME (_etree)
#define cxs_fkeep CXS_NAME (_fkeep)
#define cxs_house CXS_NAME (_house)
#define cxs_invmatch CXS_NAME (_invmatch)
#define cxs_maxtrans CXS_NAME (_maxtrans)
#define cxs_post CXS_NAME (_post)
#define cxs_scc CXS_NAME (_scc)
#define cxs_scatter CXS_NAME (_scatter)
#define cxs_tdfs CXS_NAME (_tdfs)
#define cxs_reach CXS_NAME (_reach)
#define cxs_spsolve CXS_NAME (_spsolve)
#define cxs_ereach CXS_NAME (_ereach)
#define cxs_randperm CXS_NAME (_randperm)
#define cxs_leaf CXS_NAME (_leaf)

/* utilities */
#define cxs_dalloc CXS_NAME (_dalloc)
#define cxs_done CXS_NAME (_done)
#define cxs_idone CXS_NAME (_idone)
#define cxs_ndone CXS_NAME (_ndone)
#define cxs_ddone CXS_NAME (_ddone)

/* -------------------------------------------------------------------------- */
/* Conversion routines */
/* -------------------------------------------------------------------------- */

#ifndef NCOMPLEX
cxs_di *cxs_i_real (cxs_ci *A, int real) ;
cxs_ci *cxs_i_complex (cxs_di *A, int real) ;
cxs_dl *cxs_l_real (cxs_cl *A, cxs_long_t real) ;
cxs_cl *cxs_l_complex (cxs_dl *A, cxs_long_t real) ;
#endif

#ifdef __cplusplus
}
#endif
#endif
