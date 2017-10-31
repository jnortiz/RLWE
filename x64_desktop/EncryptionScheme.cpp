/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EncryptionScheme.cpp
 * Author: jnortiz
 * 
 * Created on October 30, 2017, 3:02 PM
 */

#include "EncryptionScheme.h"

EncryptionScheme::EncryptionScheme(const int& p, const int& q) {
    this->P = p;
    this->Q = q;
    this->f.SetLength(this->P+1); //Prime 761
    this->f[this->P] = to_ZZ(1); //x^p-x-1
    //this->f[1] = to_ZZ(-1);
    this->f[0] = to_ZZ(1);
    this->gauss = new Sampling();
}

EncryptionScheme::EncryptionScheme(const EncryptionScheme& orig) {
}

EncryptionScheme::~EncryptionScheme() {
    delete this->gauss;
}

void EncryptionScheme::Mod(ZZX& a) {
    for(int i = 0; i < this->P; i++)
        a[i] = this->mod(a[i], to_ZZ(this->Q));
}

inline ZZ EncryptionScheme::mod(ZZ i, ZZ n) {
    return (i % n + n) % n;
}

void EncryptionScheme::PolySampling(ZZX& a) {
    a.SetLength(this->P);
    for(int i = 0; i < this->P; i++)
        a[i] = to_ZZ(this->gauss->knuth_yao_ct_fast_32(13, 3.1915, 0));
}

void EncryptionScheme::KeyGeneration(const ZZX& a, ZZX& r2, ZZX& p1) {
    ZZX c, r1;
    
    c.SetLength(this->P);
    p1.SetLength(this->P);
    
    PolySampling(r1); 
    PolySampling(r2);
    
    NTL::MulMod(c, a, r2, f);
    NTL::sub(p1, r1, c);
    
    Mod(p1);    
}

void EncryptionScheme::Encryption(ZZX& c1, ZZX& c2, const ZZX& a, 
        const ZZX& p1, const ZZX& m) {    
    ZZX e1, e2, e3;
    ZZX add, mult;
    
    c1.SetLength(this->P);
    c2.SetLength(this->P);
    add.SetLength(this->P);
    mult.SetLength(this->P);
    
    PolySampling(e1);
    PolySampling(e2);
    PolySampling(e3);
    
    NTL::add(add, e3, m);
    NTL::MulMod(mult, p1, e1, this->f);
    NTL::add(c2, mult, add);
    NTL::MulMod(mult, a, e1, f);
    NTL::add(c1, mult, e2);
    
    Mod(c1); Mod(c2);
}

void EncryptionScheme::Decryption(ZZX& m, const ZZX& c1, const ZZX& c2, const ZZX& r2) {
    ZZX mult;
    
    mult.SetLength(this->P);
    m.SetLength(this->P);
    
    NTL::MulMod(mult, c1, r2, f);
    NTL::add(m, mult, c2);
    
    Mod(m);
}

void EncryptionScheme::Encode(ZZX& aprime, const int32_t a[]) {    
    aprime.SetLength(this->P);
    
    int32_t bound = (int32_t) (this->Q-1)/2;

    for(int i = 0; i < this->P; i++)
        aprime[i] = to_ZZ(a[i]*bound);    
}

void EncryptionScheme::Decode(int32_t a[], const ZZX& aprime) {

    ZZ lbound = to_ZZ((this->Q-1)/4);
    ZZ ubound = to_ZZ(3*lbound);
    
    for(int i = 0; i < this->P; i++) {        
        if(aprime[i] >= lbound && aprime[i] < ubound)
            a[i] = (int32_t) 1;
        else
            a[i] = (int32_t) 0;
    }

}

