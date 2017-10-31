/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sampling.cpp
 * Author: jnortiz
 * 
 * Created on October 30, 2017, 2:55 PM
 */

#include "Sampling.h"

Sampling::Sampling() {
    this->rows = 128;
    this->cols = 40;
}

Sampling::Sampling(const Sampling& orig) {
}

Sampling::~Sampling() {
}

uint32_t Sampling::ct_lt_u32(uint32_t x, uint32_t y) {
    return (x^((x^y)|((x-y)^y))) >> 31;
}

// bit = 0 then return a
int32_t Sampling::Select(int32_t a, int32_t b, unsigned bit) {
  unsigned mask = -bit;
  return ((mask & (a ^ b)) ^ a);
}

int32_t Sampling::knuth_yao_ct_fast_32(int tailcut, int sigma, int center) {

  int32_t signal;
  int32_t _d, d, S, pNumRows; // may be negative numbers
  uint32_t bound, col, invalidSample, pNumCols, r, t;
  unsigned enable, hit;

  bound = tailcut * sigma;
  signal = 1 - 2 * (rand() & 1);
  invalidSample = bound + 1;
  pNumRows = ROWS;
  pNumCols = COLS;

  uint32_t k; // < 255
  int32_t index, row;

  index = 0;
  while(index < pNumRows) {
    r = rand();
    for (k = 0; k < 32; k++, r >>= 1)
      this->randomBits[index++] = (int8_t)(r & 0x1);
 }//end-while

  int8_t aux_hw;

  S = 0; t = 0; d = 0; hit = 0; _d = 0;
  for (row = 1; row < pNumRows; row++) {
    aux_hw = this->_hw[row];
    _d = 2 * _d + this->randomBits[row];	// Distance calculus
    enable = ct_lt_u32(_d, aux_hw); // _d < _hw[row]?
    _d = Select(_d - aux_hw, _d, enable & !hit);
    d = Select(d, _d, enable & !hit);
    t = Select(t, row, enable & !hit); // t \in {0, 127 = ROWS}
    hit += (enable & !hit);
  }

  uint32_t aux_p32 = this->_p32_t[t];

  hit = 0;
  for (col = 0; col <= 31; col++) {
    d = d - ((aux_p32 >> (uint32_t)(32 - col - 1)) & 1);

    enable = (unsigned)(d + 1); // "enable" turns 0 iff d = -1
    enable = (1 ^ ((enable | -enable) >> 31)) & 1; // "enable" turns 1 iff "enable" was 0

    S += Select(invalidSample, col, (enable & !hit));
    hit += (enable & !hit);

  }

  uint8_t aux_p8 = this->_p8_t[t];

  for (col = 32; col < pNumCols; col++) {
    d = d - ((aux_p8 >> (uint8_t)(pNumCols - col - 1)) & 1);

    enable = (unsigned)(d + 1); // "enable" turns 0 iff d = -1
    enable = (1 ^ ((enable | -enable) >> 31)) & 1; // "enable" turns 1 iff "enable" was 0

    S += Select(invalidSample, col, (enable & !hit));
    hit += (enable & !hit);

  }

  /* Note: the "col" value is in [0, bound]. So, the invalid sample must be
    * greater than bound. */
  S %= invalidSample;
  S = S - bound + center;
  S *= signal;

  return S;

}

