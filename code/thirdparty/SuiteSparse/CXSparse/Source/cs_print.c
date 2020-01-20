#include "cxs.h"
/* print a sparse matrix; use %g for integers to avoid differences with CXS_INT */
CXS_INT cxs_print (const cxs *A, CXS_INT brief)
{
    CXS_INT p, j, m, n, nzmax, nz, *Ap, *Ai ;
    CXS_ENTRY *Ax ;
    if (!A) { printf ("(null)\n") ; return (0) ; }
    m = A->m ; n = A->n ; Ap = A->p ; Ai = A->i ; Ax = A->x ;
    nzmax = A->nzmax ; nz = A->nz ;
    printf ("CXSparse Version %d.%d.%d, %s.  %s\n", CXS_VER, CXS_SUBVER,
        CXS_SUBSUB, CXS_DATE, CXS_COPYRIGHT) ;
    if (nz < 0)
    {
        printf ("%g-by-%g, nzmax: %g nnz: %g, 1-norm: %g\n", (double) m,
            (double) n, (double) nzmax, (double) (Ap [n]), cxs_norm (A)) ;
        for (j = 0 ; j < n ; j++)
        {
            printf ("    col %g : locations %g to %g\n", (double) j,
                (double) (Ap [j]), (double) (Ap [j+1]-1)) ;
            for (p = Ap [j] ; p < Ap [j+1] ; p++)
            {
                printf ("      %g : ", (double) (Ai [p])) ;
#ifdef CXS_COMPLEX
                printf ("(%g, %g)\n",
                    Ax ? CXS_REAL (Ax [p]) : 1, Ax ? CXS_IMAG (Ax [p]) : 0) ;
#else
                printf ("%g\n", Ax ? Ax [p] : 1) ;
#endif
                if (brief && p > 20) { printf ("  ...\n") ; return (1) ; }
            }
        }
    }
    else
    {
        printf ("triplet: %g-by-%g, nzmax: %g nnz: %g\n", (double) m,
            (double) n, (double) nzmax, (double) nz) ;
        for (p = 0 ; p < nz ; p++)
        {

            printf ("    %g %g : ", (double) (Ai [p]), (double) (Ap [p])) ;
#ifdef CXS_COMPLEX
            printf ("(%g, %g)\n",
                Ax ? CXS_REAL (Ax [p]) : 1, Ax ? CXS_IMAG (Ax [p]) : 0) ;
#else
            printf ("%g\n", Ax ? Ax [p] : 1) ;
#endif
            if (brief && p > 20) { printf ("  ...\n") ; return (1) ; }
        }
    }
    return (1) ;
}
