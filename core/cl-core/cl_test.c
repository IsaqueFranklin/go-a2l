#include <stdio.h>
#include "cl_pari.h"

int main() {
    init_pari();

    printf("Setting parameters...\n");
    cl_params* params = setup_params();

    GEN sk, pk;
    printf("Generating CL keys...\n");
    generate_cl_keys(&sk, &pk, params);

    pari_printf("Secret Key: %Ps\n", sk);
    pari_printf("Public Key: %Ps\n", pk);

    GEN plaintext = strtoi("123456789");
    printf("Encrypting plaintext %Ps...\n", plaintext);
    GEN ciphertext = encrypt_cl(plaintext, pk, params);

    pari_printf("Ciphertext: %Ps\n", ciphertext);

    printf("\nDecrypting...\n");
    GEN decrypted_m = decrypt_cl(ciphertext, sk, params);
    pari_printf("Decrypted result: %Ps\n", decrypted_m);

    pari_printf("Done.\n");
    
    return 0;
}