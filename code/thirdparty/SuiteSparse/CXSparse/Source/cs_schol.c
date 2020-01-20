#include "cxs.h"
/* ordering and symbolic analysis for a Cholesky factorization */
cxss *cxs_schol (CXS_INT order, const cxs *A)
{
    CXS_INT n, *c, *post, *P ;
    cxs *C ;
    cxss *S ;
    if (!CXS_CSC (A)) return (NULL) ;        /* check inputs */
    n = A->n ;
    S = cxs_calloc (1, sizeof (cxss)) ;       /* allocate result S */
    if (!S) return (NULL) ;                 /* out of memory */
    P = cxs_amd (order, A) ;                 /* P = amd(A+A'), or natural */
    S->pinv = cxs_pinv (P, n) ;              /* find inverse permutation */
    cxs_free (P) ;
    if (order && !S->pinv) return (cxs_sfree (S)) ;
    C = cxs_symperm (A, S->pinv, 0) ;        /* C = spones(triu(A(P,P))) */
    S->parent = cxs_etree (C, 0) ;           /* find etree of C */
    post = cxs_post (S->parent, n) ;         /* postorder the etree */
    c = cxs_counts (C, S->parent, post, 0) ; /* find column counts of chol(C) */
    cxs_free (post) ;
    cxs_spfree (C) ;
    S->cp = cxs_malloc (n+1, sizeof (CXS_INT)) ; /* allocate result S->cp */
    S->unz = S->lnz = cxs_cumsum (S->cp, c, n) ; /* find column pointers for L */
    cxs_free (c) ;
    return ((S->lnz >= 0) ? S : cxs_sfree (S)) ;
}
