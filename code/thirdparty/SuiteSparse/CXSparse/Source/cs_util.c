#include "cxs.h"
/* allocate a sparse matrix (triplet form or compressed-column form) */
cxs *cxs_spalloc (CXS_INT m, CXS_INT n, CXS_INT nzmax, CXS_INT values, CXS_INT triplet)
{
    cxs *A = cxs_calloc (1, sizeof (cxs)) ;    /* allocate the cxs struct */
    if (!A) return (NULL) ;                 /* out of memory */
    A->m = m ;                              /* define dimensions and nzmax */
    A->n = n ;
    A->nzmax = nzmax = CXS_MAX (nzmax, 1) ;
    A->nz = triplet ? 0 : -1 ;              /* allocate triplet or comp.col */
    A->p = cxs_malloc (triplet ? nzmax : n+1, sizeof (CXS_INT)) ;
    A->i = cxs_malloc (nzmax, sizeof (CXS_INT)) ;
    A->x = values ? cxs_malloc (nzmax, sizeof (CXS_ENTRY)) : NULL ;
    return ((!A->p || !A->i || (values && !A->x)) ? cxs_spfree (A) : A) ;
}

/* change the max # of entries sparse matrix */
CXS_INT cxs_sprealloc (cxs *A, CXS_INT nzmax)
{
    CXS_INT ok, oki, okj = 1, okx = 1 ;
    if (!A) return (0) ;
    if (nzmax <= 0) nzmax = (CXS_CSC (A)) ? (A->p [A->n]) : A->nz ;
    nzmax = CXS_MAX (nzmax, 1) ;
    A->i = cxs_realloc (A->i, nzmax, sizeof (CXS_INT), &oki) ;
    if (CXS_TRIPLET (A)) A->p = cxs_realloc (A->p, nzmax, sizeof (CXS_INT), &okj) ;
    if (A->x) A->x = cxs_realloc (A->x, nzmax, sizeof (CXS_ENTRY), &okx) ;
    ok = (oki && okj && okx) ;
    if (ok) A->nzmax = nzmax ;
    return (ok) ;
}

/* free a sparse matrix */
cxs *cxs_spfree (cxs *A)
{
    if (!A) return (NULL) ;     /* do nothing if A already NULL */
    cxs_free (A->p) ;
    cxs_free (A->i) ;
    cxs_free (A->x) ;
    return ((cxs *) cxs_free (A)) ;   /* free the cxs struct and return NULL */
}

/* free a numeric factorization */
cxsn *cxs_nfree (cxsn *N)
{
    if (!N) return (NULL) ;     /* do nothing if N already NULL */
    cxs_spfree (N->L) ;
    cxs_spfree (N->U) ;
    cxs_free (N->pinv) ;
    cxs_free (N->B) ;
    return ((cxsn *) cxs_free (N)) ;  /* free the cxsn struct and return NULL */
}

/* free a symbolic factorization */
cxss *cxs_sfree (cxss *S)
{
    if (!S) return (NULL) ;     /* do nothing if S already NULL */
    cxs_free (S->pinv) ;
    cxs_free (S->q) ;
    cxs_free (S->parent) ;
    cxs_free (S->cp) ;
    cxs_free (S->leftmost) ;
    return ((cxss *) cxs_free (S)) ;  /* free the cxss struct and return NULL */
}

/* allocate a cxs_dmperm or cxs_scc result */
cxsd *cxs_dalloc (CXS_INT m, CXS_INT n)
{
    cxsd *D ;
    D = cxs_calloc (1, sizeof (cxsd)) ;
    if (!D) return (NULL) ;
    D->p = cxs_malloc (m, sizeof (CXS_INT)) ;
    D->r = cxs_malloc (m+6, sizeof (CXS_INT)) ;
    D->q = cxs_malloc (n, sizeof (CXS_INT)) ;
    D->s = cxs_malloc (n+6, sizeof (CXS_INT)) ;
    return ((!D->p || !D->r || !D->q || !D->s) ? cxs_dfree (D) : D) ;
}

/* free a cxs_dmperm or cxs_scc result */
cxsd *cxs_dfree (cxsd *D)
{
    if (!D) return (NULL) ;     /* do nothing if D already NULL */
    cxs_free (D->p) ;
    cxs_free (D->q) ;
    cxs_free (D->r) ;
    cxs_free (D->s) ;
    return ((cxsd *) cxs_free (D)) ;  /* free the cxsd struct and return NULL */
}

/* free workspace and return a sparse matrix result */
cxs *cxs_done (cxs *C, void *w, void *x, CXS_INT ok)
{
    cxs_free (w) ;                       /* free workspace */
    cxs_free (x) ;
    return (ok ? C : cxs_spfree (C)) ;   /* return result if OK, else free it */
}

/* free workspace and return CXS_INT array result */
CXS_INT *cxs_idone (CXS_INT *p, cxs *C, void *w, CXS_INT ok)
{
    cxs_spfree (C) ;                     /* free temporary matrix */
    cxs_free (w) ;                       /* free workspace */
    return (ok ? p : (CXS_INT *) cxs_free (p)) ; /* return result, or free it */
}

/* free workspace and return a numeric factorization (Cholesky, LU, or QR) */
cxsn *cxs_ndone (cxsn *N, cxs *C, void *w, void *x, CXS_INT ok)
{
    cxs_spfree (C) ;                     /* free temporary matrix */
    cxs_free (w) ;                       /* free workspace */
    cxs_free (x) ;
    return (ok ? N : cxs_nfree (N)) ;    /* return result if OK, else free it */
}

/* free workspace and return a cxsd result */
cxsd *cxs_ddone (cxsd *D, cxs *C, void *w, CXS_INT ok)
{
    cxs_spfree (C) ;                     /* free temporary matrix */
    cxs_free (w) ;                       /* free workspace */
    return (ok ? D : cxs_dfree (D)) ;    /* return result if OK, else free it */
}
