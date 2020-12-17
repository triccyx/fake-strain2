#include "gtest/gtest.h"
#include "calculate.h"
#include "embot_dsp.h"

#include <algorithm>
#include <iostream>

constexpr uint16_t offset = 32768;
constexpr double scaleFactor = 3000;

void convertOnLinuxMachine(embot::core::utils::Triple<std::uint16_t>& force)
{
    std::cout<<"Raw::"<< force.x << "::" << force.y << "::" << force.z << "::" << std::endl;
    std::cout<<"Real::"<< (force.x-offset)*scaleFactor/offset << "::" << (force.y-offset)*scaleFactor/offset << "::" << (force.z-offset)*scaleFactor/offset << "::" << std::endl;
}

TEST(Calc, Calc_001)
{
    //*********
    //CalibMatrix
    Calculate calc;
    uint16_t tmpCalib[] = {
        0x2b9,
        0xfff8,
        0x1bad,
        0xd,
        0x1d2,
        0xe477,
        0x676,
        0x1e87,
        0xf0cb,
        0x95,
        0x58c,
        0xf0ac,
        0x50f8,
        0xad,
        0x1ac,
        0x3f90,
        0x3f45,
        0x15f,
        0x12,
        0x34,
        0xffe6,
        0xff12,
        0xf9,
        0xffe9,
        0xfebc,
        0x7,
        0xffca,
        0x9c,
        0xae,
        0x32,
        0xfffe,
        0x3f,
        0x4d,
        0xfff9,
        0x3,
        0x52};
    embot::dsp::Q15 calibQ15[36];
    for (int t = 0; t < 36; ++t)
        calibQ15[t] =  /*embot::dsp::q15::U16toQ15*/(tmpCalib[t]);
    calc.handleCalibMatrixQ15_.load(6, 6, calibQ15);

    //*********
    //TareMatrix
    uint16_t tmpTare[] = { 168, 368, 65248, 560, 24, 65360};
    embot::dsp::Q15 tareQ15[6];
    for (int t = 0; t < 6; ++t)
        tareQ15[t] = /*embot::dsp::q15::U16toQ15*/(tmpTare[t]);
    calc.handleCalibTareQ15_.load(6, 1, tareQ15);

    StrainRuntimeData runtimedata;
    //*********
    //ADC value
    std::int16_t tmp[] = {1000,3416,-592,-1592,152,-3104};//72 127 5.7 1.5 -1.5 -437
    //std::int16_t tmp[] = {-9968, 8472, 4048, 13928, -5624, -1120};
    //std::int16_t tmp[] = {264, 3096, 808, -1624, 400 ,-4360 };
    std::copy(tmp, tmp + 6, runtimedata.data.adcvalue);
    for (int t = 0; t < 6; ++t)
        runtimedata.data.q15value[t] = /*embot::dsp::q15::U16toQ15*/(runtimedata.data.adcvalue[t]);
    runtimedata.adcvalueQ15vector.load(6, 1, runtimedata.data.q15value);

    //*********
    //Calc
    calc.invoke(runtimedata);

    convertOnLinuxMachine(runtimedata.data.force);

    //EXPECT_EQ(0x1,runtime.data.torque.x);
}
