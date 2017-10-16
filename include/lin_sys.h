#ifndef LINSYS_H_GUARD
#define LINSYS_H_GUARD

#ifdef __cplusplus
extern "C" {
#endif

/* YOUR LINEAR SYSTEM SOLVER MUST IMPLEMENT THESE METHODS AND SCS_LIN_SYS_WORK
 * STRUCT */

/* private data structs (that you define) containing any necessary data to solve
 * linear system, etc. */
/* this defines the matrix A, only the linear system solver interacts with this
 * struct */
typedef struct SCS_A_DATA_MATRIX  ScsMatrix;
/* stores the necessary private workspace, only the linear system solver
 * interacts with this struct */
typedef struct SCS_LIN_SYS_WORK ScsLinSysScsWork;

/* initialize ScsLinSysScsWork structure and perform any necessary preprocessing */
ScsLinSysScsWork *init_priv(const ScsMatrix *A, const ScsSettings *stgs);
/* solves [d->RHO_X * I  A' ; A  -I] x = b for x, stores result in b, s contains
 * warm-start, iter is current scs iteration count */
scs_int solve_lin_sys(const ScsMatrix *A, const ScsSettings *stgs, ScsLinSysScsWork *p,
                    scs_float *b, const scs_float *s, scs_int iter);
/* frees ScsLinSysScsWork structure and allocated memory in ScsLinSysScsWork */
void free_priv(ScsLinSysScsWork *p);

/* forms y += A'*x */
void accum_by_atrans(const ScsMatrix *A, ScsLinSysScsWork *p, const scs_float *x, scs_float *y);
/* forms y += A*x */
void accum_by_a(const ScsMatrix *A, ScsLinSysScsWork *p, const scs_float *x, scs_float *y);

/* returns negative num if input data is invalid */
scs_int validate_lin_sys(const ScsMatrix *A);

/* returns string describing method, can return null, if not null free will be
 * called on output */
char *get_lin_sys_method(const ScsMatrix *A, const ScsSettings *stgs);
/* returns string containing summary information about linear system solves, can
 * return null, if not null free will be called on output */
char *get_lin_sys_summary(ScsLinSysScsWork *p, const ScsInfo *info);

/* Normalization routines, used if d->NORMALIZE is true */
/* normalizes A matrix, sets w->E and w->D diagonal scaling matrices, Anew =
 * d->SCALE * (D^-1)*A*(E^-1) (different to paper which is D*A*E)
 * D and E must be all positive entries, D must satisfy cone boundaries
 * must set (w->mean_norm_row_a = mean of norms of rows of normalized A) THEN scale
 * resulting A by d->SCALE */
void normalize_a(ScsMatrix *A, const ScsSettings *stgs, const ScsCone *k, ScsScaling *scal);
/* unnormalizes A matrix, unnormalizes by w->D and w->E and d->SCALE */
void un_normalize_a(ScsMatrix *A, const ScsSettings *stgs, const ScsScaling *scal);
/* to free the memory allocated in ScsMatrix */
void free_a_matrix(ScsMatrix *A);

#ifdef COPYAMATRIX
/* copies A (instead of in-place normalization), returns 0 for failure,
 * allocates memory for dstp	*/
scs_int copy_a_matrix(ScsMatrix **dstp, const ScsMatrix *src);
#endif

#ifdef __cplusplus
}
#endif

#endif