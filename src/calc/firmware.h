#pragma once

#include "strainRuntimeData.h"

class Firmware
{
public:
  Firmware(const embot::dsp::q15::matrix& handleCalibMatrixQ15,const embot::dsp::q15::matrix& handleCalibTareQ15);
  bool invoke(StrainRuntimeData &runtimedata);

private:
  embot::dsp::q15::matrix handleCalibMatrixQ15_; // Matrice di trasformazione
  embot::dsp::q15::matrix handleCalibTareQ15_;   // Vettore tara

};