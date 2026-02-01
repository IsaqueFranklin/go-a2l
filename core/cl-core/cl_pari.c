#include <pari/pari.h>
#include "cl_pari.h"

// Initializes the PARI stack
void init_pari() {
    pari_init(2000000000, 2); // 2GB stack, 2 threads
}

char* ciphertext_to_hex(GEN ciphertext) {
    return pari_sprintf("%Ps", ciphertext);
}

cl_params* setup_params() {
    cl_params* p = malloc(sizeof(cl_params));

    // Converting the giant strings do GEN numbers (PARI stuff)
    // negi is for negative numbers
    p->Delta_K = negi(strtoi("7917297328878683784842235952488620683924100338715963369693275768732162831834859052302716918416013031853265985178593375655994934704463023676296364363803257769443921988228513012040548137047446483986199954435962221122006965317176921759968659376932101987729556148116190707955808747136944623277094531007901655971804163515065712136708172984834192213773138039179492400722665370317221867505959207212674207052581946756527848674480328854830559945140752059719739492686061412113598389028096554833252668553020964851121112531561161799093718416247246137641387797659"));
    p->q = strtoi("115792089237316195423570985008687907852837564279074904382605163141518161494337");

    // Creating the quadratic form g_q (a, b, c)
    GEN a = strtoi("4008431686288539256019978212352910132512184203702279780629385896624473051840259706993970111658701503889384191610389161437594619493081376284617693948914940268917628321033421857293703008209538182518138447355678944124861126384966287069011522892641935034510731734298233539616955610665280660839844718152071538201031396242932605390717004106131705164194877377");
    GEN b = negi(strtoi("3117991088204303366418764671444893060060110057237597977724832444027781815030207752301780903747954421114626007829980376204206959818582486516608623149988315386149565855935873517607629155593328578131723080853521348613293428202727746191856239174267496577422490575311784334114151776741040697808029563449966072264511544769861326483835581088191752567148165409"));
    GEN c = strtoi("7226982982667784284607340011220616424554394853592495056851825214613723615410492468400146084481943091452495677425649405002137153382700126963171182913281089395393193450415031434185562111748472716618186256410737780813669746598943110785615647848722934493732187571819575328802273312361412673162473673367423560300753412593868713829574117975260110889575205719");
    p->g_q = Qfb0(a, b, c);

    p->bound = strtoi("25413151665722220203610173826311975594790577398151861612310606875883990655261658217495681782816066858410439979225400605895077952191850577877370585295070770312182177789916520342292660169492395314400288273917787194656036294620169343699612953311314935485124063580486497538161801803224580096");

    return p;
}

int generate_cl_keys(GEN *sk, GEN *pk, cl_params* params) {
    pari_sp av = avma; // Save the current PARI stack pointer (for further cleaning)

    // Compute CL encryption secret/public key pair for the tumbler.
    *sk = randomi(params->bound);
    *pk = nupow(params->g_q, *sk, NULL);

    return 0;
}

GEN encrypt_cl(GEN plaintext, GEN public_key, cl_params* params) {
    pari_sp av = avma; // Save the current PARI stack pointer (for further cleaning)

    // r = random(bound)
    GEN r = randomi(params->bound);

    // c1 = g_q ^ r
    GEN c1 = nupow(params->g_q, r, NULL);

    // Preparing fm(f^m)
    GEN L = Fp_inv(plaintext, params->q); // L = f^(-m) mod q
    if (!mpodd(L)) {
        L = subii(L, params->q); // If L is even, make it odd by subtracting q
    }

    // f^m = (q^m, L*q, (L^2 - Delta_K)/4)
    GEN q2 = sqri(params->q);
    GEN Lq = mulii(L, params->q);
    GEN num = subii(sqri(L), params->Delta_K);
    GEN den = strtoi("4");
    GEN f_m = Qfb0(q2, Lq, diviiexact(num, den));

    // c2 = f^m * public_key ^ r
    GEN pk_r = nupow(public_key, r, NULL);
    GEN c2 = gmul(pk_r, f_m); // gmul does the Gauss composition

    GEN result = mkvec2(c1, c2); // Return the ciphertext as a vector (c1, c2)

    return gerepilecopy(av, result); // Clean the PARI stack and return the result => Necessary to avoid memory leaks, 
    //meaning each time Go calls this function, the memory used inside it is freed and does not acumulate. 
}

GEN decrypt_cl(GEN ciphertext, GEN secret_key, cl_params* params) {
    pari_sp av = avma; // Save the current PARI stack pointer (for further cleaning)

    // Extract c1 and c2 from the ciphertext vector
    GEN c1 = gel(ciphertext, 1);
    GEN c2 = gel(ciphertext, 2);

    // Compute c1 ^ secret_key
    GEN c1_sk = nupow(c1, secret_key, NULL);

    // Compute f^m = c2 * (c1 ^ secret_key)^(-1)
    // ginv does the Gauss inversion (inverse of the form)
    GEN fm = gmul(c2, ginv(c1_sk)); // gmul does the Gauss composition

    // Extract plaintext, L=fm*b/params.q
    // gel(fm, 2) extracts the 'b' coefficient from the quadratic form f_m
    GEN L = diviiexact(gel(fm, 2), params->q); // L = b / q

    // m = L^-1 mod q
    GEN plaintext = Fp_inv(L, params->q);

    return gerepilecopy(av, plaintext);
}