#include "cxs.h"
/* apply the ith Householder vector to x */
CXS_INT cxs_happly (const cxs *V, CXS_INT i, double beta, CXS_ENTRY *x)
{
    CXS_INT p, *Vp, *Vi ;
    CXS_ENTRY *Vx, tau = 0 ;
    if (!CXS_CSC (V) || !x) return (0) ;     /* check inputs */
    Vp = V->p ; Vi = V->i ; Vx = V->x ;
    for (p = Vp [i] ; p < Vp [i+1] ; p++)   /* tau = v'*x */
    {
        tau += CXS_CONJ (Vx [p]) * x [Vi [p]] ;
    }
    tau *= beta ;                           /* tau = beta*(v'*x) */
    for (p = Vp [i] ; p < Vp [i+1] ; p++)   /* x = x - v*tau */
    {
        x [Vi [p]] -= Vx [p] * tau ;
    }
    return (1) ;
}
