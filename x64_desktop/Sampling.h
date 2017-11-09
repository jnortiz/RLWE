/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sampling.h
 * Author: jnortiz
 *
 * Created on October 30, 2017, 2:55 PM
 */

#include <inttypes.h>
#include <stdlib.h>
#include <string>
#include <NTL/vec_RR.h>
#include <NTL/RR.h>

#ifndef SAMPLING_H
#define SAMPLING_H

#define ROWS 128
#define COLS 40

using namespace NTL;
using namespace std;

class Sampling {
public:
    Sampling(const int& precision, const float& tailcut, const RR& sigma, const RR& center);
    virtual ~Sampling();
    int KnuthYao();    
private:    
    Vec< Vec<int> > P;
    Vec<int> begin;
    int precision;
    float tailcut;
    RR sigma;
    RR c;
    void BuildProbabilityMatrix();
    RR Probability(RR x, RR sigma, RR c);
    void BinaryExpansion(Vec< Vec<int> >& auxP, RR probability, int precision, int index);
    int32_t Select(int32_t a, int32_t b, unsigned bit);
};

#endif /* SAMPLING_H */

