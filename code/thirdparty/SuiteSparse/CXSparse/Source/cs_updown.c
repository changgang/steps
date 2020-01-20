#include "cxs.h"
/* sparse Cholesky update/downdate, L*L' + sigma*w*w' (sigma = +1 or -1) */
CXS_INT cxs_updown (cxs *L, CXS_INT sigma, const cxs *C, const CXS_INT *parent)
{
    CXS_INT n, p, f, j, *Lp, *Li, *Cp, *Ci ;
    CXS_ENTRY *Lx, *Cx, alpha, gamma, w1, w2, *w ;
    double beta = 1, beta2 = 1, delta ;
#ifdef CXS_COMPLEX
    cxs_complex_t phase ;
#endif
    if (!CXS_CSC (L) || !CXS_CSC (C) || !parent) return (0) ;  /* check inputs */
    Lp = L->p ; Li = L->i ; Lx = L->x ; n = L->n ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    if ((p = Cp [0]) >= Cp [1]) return (1) ;        /* return if C empty */
    w = cxs_malloc (n, sizeof (CXS_ENTRY)) ;          /* get workspace */
    if (!w) return (0) ;                            /* out of memory */
    f = Ci [p] ;
    for ( ; p < Cp [1] ; p++) f = CXS_MIN (f, Ci [p]) ;  /* f = min (find (C)) */
    for (j = f ; j != -1 ; j = parent [j]) w [j] = 0 ;  /* clear workspace w */
    for (p = Cp [0] ; p < Cp [1] ; p++) w [Ci [p]] = Cx [p] ; /* w = C */
    for (j = f ; j != -1 ; j = parent [j])          /* walk path f up to root */
    {
        p = Lp [j] ;
        alpha = w [j] / Lx [p] ;                    /* alpha = w(j) / L(j,j) */
        beta2 = beta*beta + sigma*alpha*CXS_CONJ(alpha) ;
        if (beta2 <= 0) break ;                     /* not positive definite */
        beta2 = sqrt (beta2) ;
        delta = (sigma > 0) ? (beta / beta2) : (beta2 / beta) ;
        gamma = sigma * CXS_CONJ(alpha) / (beta2 * beta) ;
        Lx [p] = delta * Lx [p] + ((sigma > 0) ? (gamma * w [j]) : 0) ;
        beta = beta2 ;
#ifdef CXS_COMPLEX
        phase = CXS_ABS (Lx [p]) / Lx [p] ;  /* phase = abs(L(j,j))/L(j,j)*/
        Lx [p] *= phase ;                   /* L(j,j) = L(j,j) * phase */
#endif
        for (p++ ; p < Lp [j+1] ; p++)
        {
            w1 = w [Li [p]] ;
            w [Li [p]] = w2 = w1 - alpha * Lx [p] ;
            Lx [p] = delta * Lx [p] + gamma * ((sigma > 0) ? w1 : w2) ;
#ifdef CXS_COMPLEX
            Lx [p] *= phase ;               /* L(i,j) = L(i,j) * phase */
#endif
        }
    }
    cxs_free (w) ;
    return (beta2 > 0) ;
}
