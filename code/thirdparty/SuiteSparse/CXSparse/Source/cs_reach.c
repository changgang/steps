#include "cxs.h"
/* xi [top...n-1] = nodes reachable from graph of G*P' via nodes in B(:,k).
 * xi [n...2n-1] used as workspace */
CXS_INT cxs_reach (cxs *G, const cxs *B, CXS_INT k, CXS_INT *xi, const CXS_INT *pinv)
{
    CXS_INT p, n, top, *Bp, *Bi, *Gp ;
    if (!CXS_CSC (G) || !CXS_CSC (B) || !xi) return (-1) ;    /* check inputs */
    n = G->n ; Bp = B->p ; Bi = B->i ; Gp = G->p ;
    top = n ;
    for (p = Bp [k] ; p < Bp [k+1] ; p++)
    {
        if (!CXS_MARKED (Gp, Bi [p]))    /* start a dfs at unmarked node i */
        {
            top = cxs_dfs (Bi [p], G, top, xi, xi+n, pinv) ;
        }
    }
    for (p = top ; p < n ; p++) CXS_MARK (Gp, xi [p]) ;  /* restore G */
    return (top) ;
}
