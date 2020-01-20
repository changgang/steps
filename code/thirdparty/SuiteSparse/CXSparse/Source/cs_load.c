#include "cxs.h"
/* load a triplet matrix from a file */
cxs *cxs_load (FILE *f)
{
    double i, j ;   /* use double for integers to avoid cxsi conflicts */
    double x ;
#ifdef CXS_COMPLEX
    double xi ;
#endif
    cxs *T ;
    if (!f) return (NULL) ;                             /* check inputs */
    T = cxs_spalloc (0, 0, 1, 1, 1) ;                    /* allocate result */
#ifdef CXS_COMPLEX
    while (fscanf (f, "%lg %lg %lg %lg\n", &i, &j, &x, &xi) == 4)
#else
    while (fscanf (f, "%lg %lg %lg\n", &i, &j, &x) == 3)
#endif
    {
#ifdef CXS_COMPLEX
        if (!cxs_entry (T, (CXS_INT) i, (CXS_INT) j, x + xi*I)) return (cxs_spfree (T)) ;
#else
        if (!cxs_entry (T, (CXS_INT) i, (CXS_INT) j, x)) return (cxs_spfree (T)) ;
#endif
    }
    return (T) ;
}
