#pragma once

#include "strainRuntimeData.h"

class Firmware {
public:
  bool invoke(StrainRuntimeData& runtimedata);

  embot::dsp::q15::matrix handleCalibMatrixQ15_; // Matrice di trasformazione
  embot::dsp::q15::matrix handleCalibTareQ15_;   // Vettore tara

private:
  
};