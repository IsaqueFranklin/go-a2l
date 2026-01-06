# Go A2L: Anonymous Atomic Locks PoC

A minimal Proof of Concept implementation of the Anonymous Atomic Locks protocol in Go, demonstrating unlinkable conditional payments via adaptor signatures.

Using the following libs:
- [PARI/GP](https://pari.math.u-bordeaux.fr/): For using class groups for implementing Castagnos-Laguillaumie encryption scheme.
- [btcec/schnorr](https://github.com/btcsuite/btcd/tree/master/btcec/schnorr): For the curve of the A in the puzzle ($A = g^\alpha$ ).