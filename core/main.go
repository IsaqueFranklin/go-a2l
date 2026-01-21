package main

/*
#cgo LDFLAGS: -lpari
#include <pari/pari.h>

// cleaning pari memory
void init_pari() {
    pari_init(4000000, 500000); // Inicializa stack
}

// composing forms: Z = X * Y
GEN compose_forms(GEN X, GEN Y, GEN D) {
    return qfbcomp(X, Y, D);
}
*/
import "C"
import (
	"fmt"
)

func main() {
	C.init_pari()

	fmt.Println("PARI intialized. qfbcomp e qfbpow are available for use.")
}
