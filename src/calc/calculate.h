#pragma once

#include "StrainRuntimeData.h"

class Calculate {
public:
  bool invoke();

private:
  StrainRuntimeData runtimedata;
};