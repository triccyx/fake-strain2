#include "gtest/gtest.h"
#include "calculate.h"

#include <algorithm>
#include <iostream>

TEST(Calc, Calc_001) {
    Calculate calc;
    embot::dsp::Q15 calibQ15[]={0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1};    
    embot::dsp::Q15 tareQ15[]={0x1,0x1,0x1,0x1,0x1,0x1};
    calc.handleCalibMatrixQ15_.load(6,6,calibQ15);
    calc.handleCalibTareQ15_.load(1,6,tareQ15);


    StrainRuntimeData runtime;
    embot::dsp::Q15 tmp[]={0x1,0x1,0x1,0x1,0x1,0x1};//ADC Q15 format
    embot::dsp::Q15 q15value[6];
    std::copy(tmp,tmp+6,q15value);
    calc.invoke(runtime);

    std::cout<<runtime.data.torque.x<<std::endl;

    EXPECT_EQ(0x1,runtime.data.torque.x);
}