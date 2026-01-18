package mypari

/*
#include <pari/pari.h>
#include <stdlib.h> // for free
*/
import "C"
import "unsafe"

func init() {
	C.pari_init(10000000, 0) // Initialize PARI stack (adjust size as needed)
}

func StrToI(s string) unsafe.Pointer { // Returns GEN as unsafe.Pointer
	cs := C.CString(s)
	defer C.free(unsafe.Pointer(cs))
	return unsafe.Pointer(C.strtoi(cs))
}
