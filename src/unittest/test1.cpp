#include "gtest/gtest.h"
#include "calculate.h"

#include <algorithm>
#include <iostream>

TEST(Calc, Calc_001) {
    Calculate calc;
    embot::dsp::Q15 calibQ15[]={
                    (short int)0x02b9,
                    (short int)0xfff8,
                    (short int)0x1bad,
                    (short int)0x000d,
                    (short int)0x01d2,
                    (short int)0xe477,
                    (short int)0x0676,
                    (short int)0x1e87,
                    (short int)0xf0cb,
                    (short int)0x0095,
                    (short int)0x058c,
                    (short int)0xf0ac,
                    (short int)0x50f8,
                    (short int)0x00ad,
                    (short int)0x01ac,
                    (short int)0x3f90,
                    (short int)0x3f45,
                    (short int)0x015f,
                    (short int)0x0012,
                    (short int)0x0034,
                    (short int)0xffe6,
                    (short int)0xff12,
                    (short int)0x00f9,
                    (short int)0xffe9,
                    (short int)0xfebc,
                    (short int)0x0007,
                    (short int)0xffca,
                    (short int)0x009c,
                    (short int)0x00ae,
                    (short int)0x0032,
                    (short int)0xfffe,
                    (short int)0x003f,
                    (short int)0x004d,
                    (short int)0xfff9,
                    (short int)0x0003,
                    (short int)0x0052
                    };    

    embot::dsp::Q15 tareQ15[]={  168,
                            368,
                            -288,
                            560,
                            24,
                            -160};

    calc.handleCalibMatrixQ15_.load(6,6,calibQ15);
    calc.handleCalibTareQ15_.load(1,6,tareQ15);


    StrainRuntimeData runtime;
    embot::dsp::Q15 tmp[]={ 1,
                            2,
                            3,
                            4,
                            5,
                            6};
    embot::dsp::Q15 q15value[6];
    std::copy(tmp,tmp+6,q15value);

    calc.invoke(runtime);

    std::cout<<runtime.data.torque.x<<std::endl;

    EXPECT_EQ(0x1,runtime.data.torque.x);
}
