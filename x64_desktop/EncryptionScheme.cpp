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
#include <vector>
#include <sstream>

//#define NTRU
#define ALTERNATE

void EncryptionScheme::SetF() {
    
    this->f.SetLength(this->P+1);

    string after, before, term, get_coeff, get_expo;
    int pos, expo;
    ZZ coeff;

    term = "not empty";
    pos = 1;

    while(1) {

      cin >> term;

      if(term == "/")
        break;

      if(term == "+" || term == "-") {
          if(term == "+") {
            pos = 1;
          } else {
            pos = 0;
          }
      } else {

        //cout << "\nTerm: " << term << ". [";

        stringstream ss(term);

        getline(ss, before, 'x');
        getline(ss, after, 'x');

        //cout << before << "], [" << after << "]\n";

        if(before.empty()) {

          if(!pos) {
            coeff = -1;
          } else coeff = 1;

        } else {            
          stringstream ff(before);
          getline(ff, get_coeff, '*');
          //cout << "Before *: " << get_coeff << ". ";
          stringstream cf(get_coeff);
          cf >> coeff;
          if(!pos)
              coeff *= -1;
          getline(ff, get_coeff, '*');
          //cout << "Before *: " << get_coeff << ". ";
        }

        if(after.empty()) {
            if(before.empty()) { // term x has no coefficient
                expo = 1;
                coeff = 1;
                if(!pos) coeff *= -1;
            } 
            expo = 0;
            if(get_coeff.empty()) expo = 1;
        } else {
          stringstream xp(after);
          getline(xp, get_expo, '^');
          getline(xp, get_expo, '^');
          stringstream xx(get_expo);
          xx >> expo;
          //cout << "Exponent: " << expo << endl;
        }

        //poly.push_back(coeff);
        //cout << "Expo: " << expo << ", Coeff: " << coeff << endl;
        this->f[expo] = to_ZZ(coeff);

      }
    }  
}

EncryptionScheme::EncryptionScheme(const int& p, const int& q, long precision, float tailcut, RR sigma, RR center) {
    
    RR::SetPrecision(to_long(precision));
    this->P = p;
    this->Q = q;

#ifdef ALTERNATE    
    SetF();
#endif
    
#ifdef NTRU
    this->f.SetLength(this->P+1);
    
    this->f[this->P] = 1;
    this->f[1] = -1;
    this->f[0] = -1;
#endif
    
    /*
    this->f[6] = to_ZZ(1);
    this->f[5] = to_ZZ(1);
    this->f[4] = to_ZZ(-5);
    this->f[3] = to_ZZ(-4);
    this->f[2] = to_ZZ(6);
    this->f[1] = to_ZZ(3);
    this->f[0] = to_ZZ(1);*/    
        
    this->tailcut = tailcut;
    this->sigma = sigma;
    this->center = center;
    
    this->gauss = new Sampling(precision, tailcut, sigma, center);
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
    int bound, sample;
    
    bound = ((int)tailcut)*to_int(sigma);
    center = to_int(center);

    a.SetLength(this->P);
    for(int i = 0; i < this->P; i++) {
        do{
            sample = this->gauss->KnuthYao();
        } while(sample >= (center + bound) || sample <= (center - bound));                
        a[i] = to_ZZ(sample);
    }    
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

