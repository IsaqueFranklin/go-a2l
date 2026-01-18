#include <pari/pari.h>

int main() {
    // Inicializa a stack PARI
    pari_init(4000000, 500000); 
    pariprintf("PARI library version %s is working!\n", paricfg_version);
    
    return 0;
}