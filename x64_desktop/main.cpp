/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: jnortiz
 *
 * Created on October 30, 2017, 1:50 PM
 */

#include <iostream>
#include <time.h>

#include <NTL/ZZX.h>
#include <NTL/ZZ_p.h>

#include "EncryptionScheme.h"

#define P 512
#define Q 12289
#define BENCH_LOOPS 1000
//#define DEBUG 1

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
    
    EncryptionScheme *es = new EncryptionScheme(P, Q);

    ZZX a, c1, c2, moriginal, mprime, r2, p1;
    int32_t m[P], mdecoded[P];    
    
    for(int k = 0; k < BENCH_LOOPS; k++) {        
        
        /* Key generation */
        RandomPoly(a);            
        es->KeyGeneration(a, r2, p1);

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
        cout << "Decrypted message: ";
        for(int i = 0; i < P-1; i++) {
            cout << mdecoded[i] << ",";
        }
        cout << m[P-1] << "}\n\n";
    #endif

        for(int i = 0; i < P; i++)
            assert(mdecoded[i] == m[i]);
        
    }
    
    cout << "OK.\n\n";
    
    return 0;
    
}
