#include "firmware.h"
#include "embot_dsp.h"

bool Firmware::invoke(StrainRuntimeData& runtimedata)
{
    // use variable set2use, originally initted w/ eeprom value but possibly changed by can message
    //std::uint8_t set2use = runtimedata.set2use_get();
    
    // in adcvalue[] we performed adc acquisition, in value[] we put the q15 values and we also have checkd vs adc saturation
    
    
    // now prepare calib data: it will be inside the triples: runtimedata.data.torque and runtimedata.data.force 
    
        
    // apply formula: forcetorque = M * (adcvalue + calibtare) + currtare
    // default values are: M = IdentityMatrix(0x7fff); calibtare = Vector(0); currtare = Vector(0)
    
    bool q15saturated =  false;
        
    embot::dsp::Q15 q15tmpvector[6];
    embot::dsp::q15::matrix tmpQ15vector(6, 1, q15tmpvector);
    
    embot::dsp::q15::add(runtimedata.adcvalueQ15vector, handleCalibTareQ15_, tmpQ15vector, q15saturated);
    embot::dsp::q15::multiply(handleCalibMatrixQ15_, tmpQ15vector, runtimedata.forcetorqueQ15vector, q15saturated);
    embot::dsp::q15::add(runtimedata.forcetorqueQ15vector, runtimedata.currtareQ15vector, runtimedata.forcetorqueQ15vector, q15saturated);
    
    // copy 
    
    // now in forcetorqueQ15vector (runtimedata.data.forcetorque[]) we have the result ... 
    
    // if we did have saturation ... we send old safe data.
   
    // if we are ok, we use what in runtimedata.data.forcetorque[] to compute safe data and data to send
    // data2send = safedata = 0x8000 + forcetorque
   
    if(false == runtimedata.data.adcsaturation)
    {
        runtimedata.data.force.set(embot::dsp::q15::Q15toU16(runtimedata.data.forcetorque[0]), embot::dsp::q15::Q15toU16(runtimedata.data.forcetorque[1]), embot::dsp::q15::Q15toU16(runtimedata.data.forcetorque[2]));
        runtimedata.data.torque.set(embot::dsp::q15::Q15toU16(runtimedata.data.forcetorque[3]), embot::dsp::q15::Q15toU16(runtimedata.data.forcetorque[4]), embot::dsp::q15::Q15toU16(runtimedata.data.forcetorque[5]));
    }
    else
    {
        // we dont update so that in runtimedata.data.torque and runtimedata.data.force there are always valid values
    }
        
    
    return true;

}