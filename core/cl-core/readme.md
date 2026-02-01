# CL quadratic class groups using PARI/GP

This module contains a C implementation of Castagnos-Laguillaumie, utilizing the **PARI/GP** library. The focus is on implementing Class Group cryptography.

## Overview

The module implements specialized mathematical operations such as working with binary quadratic forms and modular arithmetic. It is designed to be efficient, leveraging PARI's low-level C API for performance-critical tasks.

## Core PARI/GP Reference

- **`negi`**: It's a prefix used for negative numbers.
- **`strtoi`**: String to integer. Converts text to the PARI internal format.
- **`GEN`**: It's a long* pointer.
- **`avma`**: Address of value on main stack. Pointer to the top of the stack (useful to clean the intermediary calculations at the end of the function).
- **`randomi`**: Get a random number in a pool (bound), 0 <= r < bound.
- **`nupow`**: Binary exponentiation of quadratic forms (faster than gpow).
- **`Fp_inv`**: Modular inversion a^-1 (mod m), used for the m.
- **`mpodd`**: Verifies if is an odd number, returns 1 if odd. Using a "!" inverts it.
- **`subii`**: Subtracts 2 integers
- **`Qfb`**: Quadratic form initialization
- **`deviiexact`**: Exact integer division (the rest needs to be 0).
- **`mkvec2`**: Creates a PARI vector containing 2 elements.