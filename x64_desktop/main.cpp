/* 
 * File:   main.cpp
 * Author: jnortiz
 *
 * Created on October 30, 2017, 1:50 PM
 * 
 * Compiling: g++ -lm -lgmp -lntl -I/usr/local/include/NTL main.cpp EncryptionScheme.cpp Sampling.cpp -o main
 * Executing: LD_LIBRARY_PATH=/usr/local/lib ./main
 * 
 * Requirements: GMP and NTL libraries. In the compiling and executing example above, 
 * it assumes the default destination paths in the installation process.
 * 
 */

#include <iostream>
#include <time.h>

#include <NTL/ZZX.h>
#include <NTL/ZZ_p.h>

#include "EncryptionScheme.h"

#define BENCH_LOOPS 10
//#define DEBUG
#define NTRU

#ifdef RLWE
/* Regular Ring-LWE */
#define P 1024
#define Q 11289
#define SIGMA 3.19
#endif

#ifdef ALTERNATE
/* Alternate */
#define P 1024
#define Q 11289
#define SIGMA 319
#endif

#ifdef NTRU
/* NTRU Prime parameters */
#define P 761
#define Q 4591
#define SIGMA 2.0
#endif

using namespace std;
using namespace NTL;

void RandomPoly(ZZX& a) {
    a.SetLength(P);
    for(int i = 0; i < P; i++)
        a[i] = NTL::RandomBits_ulong(log(Q));
}

void RandomMessage(int32_t m[]) {
    for(int i = 0; i < P; i++)
        m[i] = to_int(NTL::RandomBits_long(1));    
}

int main(int argc, char** argv) {
    
    ZZ_p::init(to_ZZ(Q));

    cout << "------------------------\nRing-LWE encryption scheme\n------------------------\n" << endl;
    cout << "Running the scheme " << BENCH_LOOPS << " times.\n\n";
    
    long precision = 128;
    RR center = to_RR(0.0);
    float tailcut = 13.2;
    
    EncryptionScheme *es = new EncryptionScheme(P, Q, precision, tailcut, to_RR(SIGMA), center);

    ZZX a, c1, c2, moriginal, mprime, r2, p1;
    int32_t m[P], mdecoded[P];    
    
    int total_errors = 0;

    /* Key generation */
    RandomPoly(a);            
    es->KeyGeneration(a, r2, p1);
    
    for(int k = 0; k < BENCH_LOOPS; k++) {        
        
        RandomMessage(m);

    #ifdef DEBUG
        cout << "Message being encrypted: {";
        for(int i = 0; i < P-1; i++) {
            cout << m[i] << ",";
        }
        cout << m[P-1] << "}\n" << endl;
    #endif

        /* Encryption */
        es->Encode(mprime, m);    
        es->Encryption(c1, c2, a, p1, mprime);

    #ifdef DEBUG
        cout << "Encrypted message: " << c1 << endl << endl;
    #endif

        /* Decryption */
        es->Decryption(moriginal, c1, c2, r2);
        es->Decode(mdecoded, moriginal);

    #ifdef DEBUG
        cout << "Decrypted message: {";
        for(int i = 0; i < P-1; i++) {
            cout << mdecoded[i] << ",";
        }
        cout << m[P-1] << "}\n\n";
    #endif

        int counter = 0;
        for(int i = 0; i < P; i++)
            if(! (mdecoded[i] == m[i])) {
#ifdef DEBUG
                cout << "Error in the " << i << "-th coordinate. Respective to: " << moriginal[i] << endl;
#endif
                counter++;
            }        
#ifdef DEBUG
        cout << "Number of incorrect decoding: " << counter << endl << endl;
#endif
        if(counter > 0)
            total_errors++;
        
    }
    
    if(total_errors == 0)
        cout << "OK.\n";
    else 
        cout << total_errors << " executions have failed.\n";
    
    if(total_errors == BENCH_LOOPS)
        cout << "\nAll executions have failed.\n\n";
    
    return 0;
    
}
