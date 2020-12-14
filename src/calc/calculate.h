#pragma once

#include "strainConfigData.h"
#include "strainRuntimeData.h"

class Calculate {
public:
  bool invoke();

private:
  StrainRuntimeData runtimedata;
  StrainConfigData configdata;
};