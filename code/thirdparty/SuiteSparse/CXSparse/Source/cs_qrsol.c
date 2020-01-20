#include "cxs.h"
/* x=A\b where A can be rectangular; b overwritten with solution */
CXS_INT cxs_qrsol (CXS_INT order, const cxs *A, CXS_ENTRY *b)
{
    CXS_ENTRY *x ;
    cxss *S ;
    cxsn *N ;
    cxs *AT = NULL ;
    CXS_INT k, m, n, ok ;
    if (!CXS_CSC (A) || !b) return (0) ; /* check inputs */
    n = A->n ;
    m = A->m ;
    if (m >= n)
    {
        S = cxs_sqr (order, A, 1) ;          /* ordering and symbolic analysis */
        N = cxs_qr (A, S) ;                  /* numeric QR factorization */
        x = cxs_calloc (S ? S->m2 : 1, sizeof (CXS_ENTRY)) ;    /* get workspace */
        ok = (S && N && x) ;
        if (ok)
        {
            cxs_ipvec (S->pinv, b, x, m) ;   /* x(0:m-1) = b(p(0:m-1) */
            for (k = 0 ; k < n ; k++)       /* apply Householder refl. to x */
            {
                cxs_happly (N->L, k, N->B [k], x) ;
            }
            cxs_usolve (N->U, x) ;           /* x = R\x */
            cxs_ipvec (S->q, x, b, n) ;      /* b(q(0:n-1)) = x(0:n-1) */
        }
    }
    else
    {
        AT = cxs_transpose (A, 1) ;          /* Ax=b is underdetermined */
        S = cxs_sqr (order, AT, 1) ;         /* ordering and symbolic analysis */
        N = cxs_qr (AT, S) ;                 /* numeric QR factorization of A' */
        x = cxs_calloc (S ? S->m2 : 1, sizeof (CXS_ENTRY)) ;    /* get workspace */
        ok = (AT && S && N && x) ;
        if (ok)
        {
            cxs_pvec (S->q, b, x, m) ;       /* x(q(0:m-1)) = b(0:m-1) */
            cxs_utsolve (N->U, x) ;          /* x = R'\x */
            for (k = m-1 ; k >= 0 ; k--)    /* apply Householder refl. to x */
            {
                cxs_happly (N->L, k, N->B [k], x) ;
            }
            cxs_pvec (S->pinv, x, b, n) ;    /* b(0:n-1) = x(p(0:n-1)) */
        }
    }
    cxs_free (x) ;
    cxs_sfree (S) ;
    cxs_nfree (N) ;
    cxs_spfree (AT) ;
    return (ok) ;
}
