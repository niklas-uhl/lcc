/* katric branch: LCC (Strausz, Vella, Di Girolamo, Besta, Hoefler, IPDPS'22) as a library, for katric's competitor
 * adapter. The calls replace LCC's main(): lcc_setup() (process mesh, communicators), lcc_preprocess() (2D
 * partitioning, normalization, CSC), lcc_count() (the kernel lcc_func_bin_simd plus a global triangle count) and
 * lcc_free(). All of them are collective on MPI_COMM_WORLD, which LCC uses throughout. */
#ifndef LCC_API_H
#define LCC_API_H

#include <mpi.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lcc_options {
  /* Bytes per rank for CLaMPI's caches (index + adjacency); LCC hard-coded 8 GiB (8589934592). */
  uint64_t clampi_cache_size;
  /* LCC's -H 1: remove degree-1 vertices before partitioning (off in LCC by default). */
  int degree_reduction;
} lcc_options;

/* n: global number of vertices (ids 0..n-1), below 2^32 (LCC's local and remote ids are 32-bit). LCC runs on a
 * 1 x p process mesh (a block of consecutive vertex ids per rank), with n rounded up to a multiple of p. */
void lcc_setup(uint64_t n, const lcc_options *options);

/* Takes ownership of `edges` (malloc'ed, 2 * nedges entries, pairs (u, v) with both directions of every undirected
 * edge, any distribution over the ranks). At most INT_MAX edges per rank. */
void lcc_preprocess(uint64_t *edges, uint64_t nedges);

/* Runs LCC's kernel and returns the global number of triangles on every rank (the sum of the per-vertex counts,
 * divided by 3). */
uint64_t lcc_count(void);

/* Frees what lcc_setup() and lcc_preprocess() allocated. */
void lcc_free(void);

#ifdef __cplusplus
}
#endif

#endif /* LCC_API_H */
