/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EncryptionScheme.h
 * Author: jnortiz
 *
 * Created on October 30, 2017, 3:02 PM
 */

#include <assert.h>
#include <inttypes.h>
#include <iostream>
#include <NTL/ZZX.h>
#include <NTL/ZZX.h>
#include <NTL/ZZ_p.h>
#include "Sampling.h"

#ifndef ENCRYPTIONSCHEME_H
#define ENCRYPTIONSCHEME_H

using namespace NTL;
using namespace std;

class EncryptionScheme {
public:
    EncryptionScheme(const int& p, const int& q, long precision, float tailcut, RR sigma, RR center);
    EncryptionScheme(const EncryptionScheme& orig);
    virtual ~EncryptionScheme();
    
    void KeyGeneration(const ZZX& a, ZZX& r2, ZZX& p1);
    void Encode(ZZX& aprime, const int32_t a[]);
    void Decode(int32_t a[], const ZZX& aprime);    
    void Encryption(ZZX& c1, ZZX& c2, const ZZX& a, const ZZX& p1, const ZZX& m);
    void Decryption(ZZX& m, const ZZX& c1, const ZZX& c2, const ZZX& r2);
private:
    /* Ring parameters */
    int P;
    int Q;
    ZZX f;
    
    /* Knuth-Yao discrete Gaussian sampler parameters */
    float tailcut;
    RR sigma;
    RR center;
    
    Sampling *gauss;
    void PolySampling(ZZX& a);
    void Mod(ZZX& a);
    inline ZZ mod(ZZ i, ZZ n);
};

#endif /* ENCRYPTIONSCHEME_H */

