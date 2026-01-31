// cl_pari.h
#ifndef CL_PARI_H
#define CL_PARI_H

#include <pari/pari.h>

typedef struct {
    GEN Delta_K;
    GEN bound;
    GEN q;
    GEN g_q;
} cl_params;

void init_pari();
cl_params* setup_params();
GEN encrypt_cl(GEN plaintext, GEN public_key, cl_params* params);

#endif