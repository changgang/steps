#include "cxs.h"
/* x=A\b where A is symmetric positive definite; b overwritten with solution */
CXS_INT cxs_cholsol (CXS_INT order, const cxs *A, CXS_ENTRY *b)
{
    CXS_ENTRY *x ;
    cxss *S ;
    cxsn *N ;
    CXS_INT n, ok ;
    if (!CXS_CSC (A) || !b) return (0) ;     /* check inputs */
    n = A->n ;
    S = cxs_schol (order, A) ;               /* ordering and symbolic analysis */
    N = cxs_chol (A, S) ;                    /* numeric Cholesky factorization */
    x = cxs_malloc (n, sizeof (CXS_ENTRY)) ;    /* get workspace */
    ok = (S && N && x) ;
    if (ok)
    {
        cxs_ipvec (S->pinv, b, x, n) ;   /* x = P*b */
        cxs_lsolve (N->L, x) ;           /* x = L\x */
        cxs_ltsolve (N->L, x) ;          /* x = L'\x */
        cxs_pvec (S->pinv, x, b, n) ;    /* b = P'*x */
    }
    cxs_free (x) ;
    cxs_sfree (S) ;
    cxs_nfree (N) ;
    return (ok) ;
}
