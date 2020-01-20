#include "cxs.h"
/* x=A\b where A is unsymmetric; b overwritten with solution */
CXS_INT cxs_lusol (CXS_INT order, const cxs *A, CXS_ENTRY *b, double tol)
{
    CXS_ENTRY *x ;
    cxss *S ;
    cxsn *N ;
    CXS_INT n, ok ;
    if (!CXS_CSC (A) || !b) return (0) ;     /* check inputs */
    n = A->n ;
    S = cxs_sqr (order, A, 0) ;              /* ordering and symbolic analysis */
    N = cxs_lu (A, S, tol) ;                 /* numeric LU factorization */
    x = cxs_malloc (n, sizeof (CXS_ENTRY)) ;    /* get workspace */
    ok = (S && N && x) ;
    if (ok)
    {
        cxs_ipvec (N->pinv, b, x, n) ;       /* x = b(p) */
        cxs_lsolve (N->L, x) ;               /* x = L\x */
        cxs_usolve (N->U, x) ;               /* x = U\x */
        cxs_ipvec (S->q, x, b, n) ;          /* b(q) = x */
    }
    cxs_free (x) ;
    cxs_sfree (S) ;
    cxs_nfree (N) ;
    return (ok) ;
}
