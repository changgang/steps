#include "cxs.h"
/* breadth-first search for coarse decomposition (C0,C1,R1 or R0,R3,C3) */
static CXS_INT cxs_bfs (const cxs *A, CXS_INT n, CXS_INT *wi, CXS_INT *wj, CXS_INT *queue,
    const CXS_INT *imatch, const CXS_INT *jmatch, CXS_INT mark)
{
    CXS_INT *Ap, *Ai, head = 0, tail = 0, j, i, p, j2 ;
    cxs *C ;
    for (j = 0 ; j < n ; j++)           /* place all unmatched nodes in queue */
    {
        if (imatch [j] >= 0) continue ; /* skip j if matched */
        wj [j] = 0 ;                    /* j in set C0 (R0 if transpose) */
        queue [tail++] = j ;            /* place unmatched col j in queue */
    }
    if (tail == 0) return (1) ;         /* quick return if no unmatched nodes */
    C = (mark == 1) ? ((cxs *) A) : cxs_transpose (A, 0) ;
    if (!C) return (0) ;                /* bfs of C=A' to find R3,C3 from R0 */
    Ap = C->p ; Ai = C->i ;
    while (head < tail)                 /* while queue is not empty */
    {
        j = queue [head++] ;            /* get the head of the queue */
        for (p = Ap [j] ; p < Ap [j+1] ; p++)
        {
            i = Ai [p] ;
            if (wi [i] >= 0) continue ; /* skip if i is marked */
            wi [i] = mark ;             /* i in set R1 (C3 if transpose) */
            j2 = jmatch [i] ;           /* traverse alternating path to j2 */
            if (wj [j2] >= 0) continue ;/* skip j2 if it is marked */
            wj [j2] = mark ;            /* j2 in set C1 (R3 if transpose) */
            queue [tail++] = j2 ;       /* add j2 to queue */
        }
    }
    if (mark != 1) cxs_spfree (C) ;      /* free A' if it was created */
    return (1) ;
}

/* collect matched rows and columns into p and q */
static void cxs_matched (CXS_INT n, const CXS_INT *wj, const CXS_INT *imatch, CXS_INT *p, CXS_INT *q,
    CXS_INT *cc, CXS_INT *rr, CXS_INT set, CXS_INT mark)
{
    CXS_INT kc = cc [set], j ;
    CXS_INT kr = rr [set-1] ;
    for (j = 0 ; j < n ; j++)
    {
        if (wj [j] != mark) continue ;      /* skip if j is not in C set */
        p [kr++] = imatch [j] ;
        q [kc++] = j ;
    }
    cc [set+1] = kc ;
    rr [set] = kr ;
}

/* collect unmatched rows into the permutation vector p */
static void cxs_unmatched (CXS_INT m, const CXS_INT *wi, CXS_INT *p, CXS_INT *rr, CXS_INT set)
{
    CXS_INT i, kr = rr [set] ;
    for (i = 0 ; i < m ; i++) if (wi [i] == 0) p [kr++] = i ;
    rr [set+1] = kr ;
}

/* return 1 if row i is in R2 */
static CXS_INT cxs_rprune (CXS_INT i, CXS_INT j, CXS_ENTRY aij, void *other)
{
    CXS_INT *rr = (CXS_INT *) other ;
    return (i >= rr [1] && i < rr [2]) ;
}

/* Given A, compute coarse and then fine dmperm */
cxsd *cxs_dmperm (const cxs *A, CXS_INT seed)
{
    CXS_INT m, n, i, j, k, cnz, nc, *jmatch, *imatch, *wi, *wj, *pinv, *Cp, *Ci,
        *ps, *rs, nb1, nb2, *p, *q, *cc, *rr, *r, *s, ok ;
    cxs *C ;
    cxsd *D, *scc ;
    /* --- Maximum matching ------------------------------------------------- */
    if (!CXS_CSC (A)) return (NULL) ;            /* check inputs */
    m = A->m ; n = A->n ;
    D = cxs_dalloc (m, n) ;                      /* allocate result */
    if (!D) return (NULL) ;
    p = D->p ; q = D->q ; r = D->r ; s = D->s ; cc = D->cc ; rr = D->rr ;
    jmatch = cxs_maxtrans (A, seed) ;            /* max transversal */
    imatch = jmatch + m ;                       /* imatch = inverse of jmatch */
    if (!jmatch) return (cxs_ddone (D, NULL, jmatch, 0)) ;
    /* --- Coarse decomposition --------------------------------------------- */
    wi = r ; wj = s ;                           /* use r and s as workspace */
    for (j = 0 ; j < n ; j++) wj [j] = -1 ;     /* unmark all cols for bfs */
    for (i = 0 ; i < m ; i++) wi [i] = -1 ;     /* unmark all rows for bfs */
    cxs_bfs (A, n, wi, wj, q, imatch, jmatch, 1) ;       /* find C1, R1 from C0*/
    ok = cxs_bfs (A, m, wj, wi, p, jmatch, imatch, 3) ;  /* find R3, C3 from R0*/
    if (!ok) return (cxs_ddone (D, NULL, jmatch, 0)) ;
    cxs_unmatched (n, wj, q, cc, 0) ;                    /* unmatched set C0 */
    cxs_matched (n, wj, imatch, p, q, cc, rr, 1, 1) ;    /* set R1 and C1 */
    cxs_matched (n, wj, imatch, p, q, cc, rr, 2, -1) ;   /* set R2 and C2 */
    cxs_matched (n, wj, imatch, p, q, cc, rr, 3, 3) ;    /* set R3 and C3 */
    cxs_unmatched (m, wi, p, rr, 3) ;                    /* unmatched set R0 */
    cxs_free (jmatch) ;
    /* --- Fine decomposition ----------------------------------------------- */
    pinv = cxs_pinv (p, m) ;         /* pinv=p' */
    if (!pinv) return (cxs_ddone (D, NULL, NULL, 0)) ;
    C = cxs_permute (A, pinv, q, 0) ;/* C=A(p,q) (it will hold A(R2,C2)) */
    cxs_free (pinv) ;
    if (!C) return (cxs_ddone (D, NULL, NULL, 0)) ;
    Cp = C->p ;
    nc = cc [3] - cc [2] ;          /* delete cols C0, C1, and C3 from C */
    if (cc [2] > 0) for (j = cc [2] ; j <= cc [3] ; j++) Cp [j-cc[2]] = Cp [j] ;
    C->n = nc ;
    if (rr [2] - rr [1] < m)        /* delete rows R0, R1, and R3 from C */
    {
        cxs_fkeep (C, cxs_rprune, rr) ;
        cnz = Cp [nc] ;
        Ci = C->i ;
        if (rr [1] > 0) for (k = 0 ; k < cnz ; k++) Ci [k] -= rr [1] ;
    }
    C->m = nc ;
    scc = cxs_scc (C) ;              /* find strongly connected components of C*/
    if (!scc) return (cxs_ddone (D, C, NULL, 0)) ;
    /* --- Combine coarse and fine decompositions --------------------------- */
    ps = scc->p ;                   /* C(ps,ps) is the permuted matrix */
    rs = scc->r ;                   /* kth block is rs[k]..rs[k+1]-1 */
    nb1 = scc->nb  ;                /* # of blocks of A(R2,C2) */
    for (k = 0 ; k < nc ; k++) wj [k] = q [ps [k] + cc [2]] ;
    for (k = 0 ; k < nc ; k++) q [k + cc [2]] = wj [k] ;
    for (k = 0 ; k < nc ; k++) wi [k] = p [ps [k] + rr [1]] ;
    for (k = 0 ; k < nc ; k++) p [k + rr [1]] = wi [k] ;
    nb2 = 0 ;                       /* create the fine block partitions */
    r [0] = s [0] = 0 ;
    if (cc [2] > 0) nb2++ ;         /* leading coarse block A (R1, [C0 C1]) */
    for (k = 0 ; k < nb1 ; k++)     /* coarse block A (R2,C2) */
    {
        r [nb2] = rs [k] + rr [1] ; /* A (R2,C2) splits into nb1 fine blocks */
        s [nb2] = rs [k] + cc [2] ;
        nb2++ ;
    }
    if (rr [2] < m)
    {
        r [nb2] = rr [2] ;          /* trailing coarse block A ([R3 R0], C3) */
        s [nb2] = cc [3] ;
        nb2++ ;
    }
    r [nb2] = m ;
    s [nb2] = n ;
    D->nb = nb2 ;
    cxs_dfree (scc) ;
    return (cxs_ddone (D, C, NULL, 1)) ;
}
