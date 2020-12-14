#pragma once

#include "strainRuntimeData.h"

class Calculate {
public:
  bool invoke();

  const embot::dsp::q15::matrix handleCalibMatrixQ15_; // Matrice di trasformazione
  const embot::dsp::q15::matrix handleCalibTareQ15_; // Vettore tara

private:
  StrainRuntimeData runtimedata_;
};