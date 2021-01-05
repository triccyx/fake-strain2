#include "gtest/gtest.h"
#include "firmware.h"
#include "embot_dsp.h"

#include <algorithm>
#include <iostream>

constexpr uint16_t offset = 32768;
constexpr double scaleFactor = 3000;

void convertOnLinuxMachine(embot::core::utils::Triple<std::uint16_t>& force,embot::core::utils::Triple<std::uint16_t>& torque)
{
    std::cout<<"Raw force out of FT via CAN::"<<std::hex<< force.x << "::" << force.y << "::" << force.z << "::" << std::endl;
    std::cout<<"Raw torque out of FT via CAN::"<<std::hex<< torque.x << "::" << torque.y << "::" << torque.z << "::" << std::endl;
    std::cout<<"Real force::"<< (force.x-offset)*scaleFactor/offset << "::" << (force.y-offset)*scaleFactor/offset << "::" << (force.z-offset)*scaleFactor/offset << "::" << std::endl;
    std::cout<<"Real torque::"<< (torque.x-offset)*scaleFactor/offset << "::" << (torque.y-offset)*scaleFactor/offset << "::" << (torque.z-offset)*scaleFactor/offset << "::" << std::endl;
}

TEST(Calc, Calc_001)
{
    //*********
    //CalibMatrix
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
        calibQ15[t] =  (int16_t)(tmpCalib[t]);
    embot::dsp::q15::matrix matrix;
    matrix.load(6, 6, calibQ15);


    //*********
    //TareMatrix
    uint16_t tmpTare[] = { 0x00A8,0x0170,0xFFE0,0x0230,0x0018,0xFF50};// --> {168,368,65248,560,24,65360}

    embot::dsp::Q15 tareQ15[6];
    for (int t = 0; t < 6; ++t)
        tareQ15[t] = (int16_t)(tmpTare[t]);
    embot::dsp::q15::matrix tare;
    tare.load(6, 1, tareQ15);

    //*********
    //ADC value
    StrainRuntimeData runtimedata;
    std::int16_t tmp[] = {1187,2710,488,-1438,750,-3712};// equal to --> std::uint16_t tmp[] = {0x04A3,0x0A96,0x01E8,0xFA62,0x02EE,0xF180};
    for (int t = 0; t < 6; ++t)
        runtimedata.data.q15value[t] = (int16_t)(tmp[t]);
    runtimedata.adcvalueQ15vector.load(6, 1, runtimedata.data.q15value);

    //*********
    //Calc
    Firmware calc(matrix,tare);
    calc.invoke(runtimedata);

    //*********
    //PC104
    std::cout<<runtimedata.forcetorqueQ15vector.get(0,0)<<std::endl;
    convertOnLinuxMachine(runtimedata.data.force,runtimedata.data.torque);

    //EXPECT_EQ(0x1,runtime.data.torque.x);
}

