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
char* ciphertext_to_hex(GEN ciphertext);
int generate_cl_keys(GEN *sk, GEN *pk, cl_params* params);
GEN encrypt_cl(GEN plaintext, GEN public_key, cl_params* params);
GEN decrypt_cl(GEN ciphertext, GEN secret_key, cl_params* params);

#endif