#include "cxs.h"
/* find nonzero pattern of Cholesky L(k,1:k-1) using etree and triu(A(:,k)) */
CXS_INT cxs_ereach (const cxs *A, CXS_INT k, const CXS_INT *parent, CXS_INT *s, CXS_INT *w)
{
    CXS_INT i, p, n, len, top, *Ap, *Ai ;
    if (!CXS_CSC (A) || !parent || !s || !w) return (-1) ;   /* check inputs */
    top = n = A->n ; Ap = A->p ; Ai = A->i ;
    CXS_MARK (w, k) ;                /* mark node k as visited */
    for (p = Ap [k] ; p < Ap [k+1] ; p++)
    {
        i = Ai [p] ;                /* A(i,k) is nonzero */
        if (i > k) continue ;       /* only use upper triangular part of A */
        for (len = 0 ; !CXS_MARKED (w,i) ; i = parent [i]) /* traverse up etree*/
        {
            s [len++] = i ;         /* L(k,i) is nonzero */
            CXS_MARK (w, i) ;        /* mark i as visited */
        }
        while (len > 0) s [--top] = s [--len] ; /* push path onto stack */
    }
    for (p = top ; p < n ; p++) CXS_MARK (w, s [p]) ;    /* unmark all nodes */
    CXS_MARK (w, k) ;                /* unmark node k */
    return (top) ;                  /* s [top..n-1] contains pattern of L(k,:)*/
}
