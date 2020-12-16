#pragma once

#include "embot_core_utils.h"
#include "embot_dsp.h"
#include "enum.h"
#include <cinttypes>
#include <cstring>
#include <iostream>

// it is a holder of volatile data used in runtime. we use the same approach as
// for config data
struct StrainRuntimeData_t {

  // embot::core::Time         txperiod; Luca

  std::uint16_t dmabuffer[6];
  std::uint16_t
      adcvalue[6]; // the values as acquired by the adc... but scaled to full
                   // 64k range. original strain had 16-bit adc

  embot::dsp::Q15 q15value[6]; // same as adcvalue but in q15 format
  embot::dsp::Q15 tare[6];     // the tare applied as a last correction. it is
                           // always ::zero unless changed with SET_CURR_TARE
  embot::dsp::Q15 forcetorque[6];

  // the calibrated values for torque and force. they are raw values, not in
  // dsp::Q15 format
  embot::core::utils::Triple<std::uint16_t> torque;
  embot::core::utils::Triple<std::uint16_t> force;

  // embot::prot::can::analog::polling::Message_SET_TXMODE::StrainMode strainmode; Luca

  bool adcsaturation;
  embot::prot::can::analog::periodic::ADCsaturation saturationonchannel[6];

  std::uint8_t adcfailures;
  std::uint8_t adciszero;

  std::uint8_t set2use;

  // methods
  void clear() {
    std::memset(q15value, 0, sizeof(q15value));
    std::memset(tare, 0, sizeof(tare));
    std::memset(dmabuffer, 0, sizeof(dmabuffer));
    std::memset(adcvalue, 0, sizeof(adcvalue));
    std::memset(forcetorque, 0, sizeof(forcetorque));
    force.clear();
    torque.clear();
    //strainmode =embot::prot::can::analog::polling::Message_SET_TXMODE::StrainMode::none; Luca
    adcsaturation = false;
    adcfailures = 0;
    adciszero = 0;
    std::memset(saturationonchannel,
                static_cast<std::uint8_t>(
                    embot::prot::can::analog::periodic::ADCsaturation::NONE),
                sizeof(saturationonchannel));
    set2use = 0;
  }
  StrainRuntimeData_t() { clear(); }
};

class StrainRuntimeData {
public:
  StrainRuntimeData_t data;

  // used for the sake of matrix-based calculation
  embot::dsp::q15::matrix currtareQ15vector;
  embot::dsp::q15::matrix forcetorqueQ15vector;
  embot::dsp::q15::matrix adcvalueQ15vector;

  static const std::uint16_t ADCsaturationLow = 1000;
  static const std::uint16_t ADCsaturationHigh = 64000;

  // methods

  bool check_adcsaturation() {
    data.adcsaturation = false;
    std::memset(data.saturationonchannel,
                static_cast<std::uint8_t>(
                    embot::prot::can::analog::periodic::ADCsaturation::NONE),
                sizeof(data.saturationonchannel));

    for (int channel = 0; channel < numOfChannels; channel++) {
      data.saturationonchannel[channel] =
          embot::prot::can::analog::periodic::ADCsaturation::NONE;

      if (data.adcvalue[channel] < ADCsaturationLow) {
        data.saturationonchannel[channel] =
            embot::prot::can::analog::periodic::ADCsaturation::LOW;
        data.adcsaturation = true;
      } else if (data.adcvalue[channel] > ADCsaturationHigh) {
        data.saturationonchannel[channel] =
            embot::prot::can::analog::periodic::ADCsaturation::HIGH;
        data.adcsaturation = true;
      }
    }

    return data.adcsaturation;
  }

  void tare_fill(embot::dsp::Q15 value) { currtareQ15vector.fill(value); }

  void tare_set(std::uint8_t channel, embot::dsp::Q15 value) {
    if (channel < numOfChannels) {
      data.tare[channel] = value;
    }
  }

  embot::dsp::Q15 tare_get(std::uint8_t channel) {
    if (channel < numOfChannels) {
      return data.tare[channel];
    }

    return embot::dsp::q15::zero;
  }

  bool clear() {
    data.clear();
    return true;
  }

  void set2use_set(std::uint8_t set) {
    if (set < numOfSets) {
      data.set2use = set;
    }
  }

  std::uint8_t set2use_get() { return data.set2use; }

  StrainRuntimeData() {
    clear();

    forcetorqueQ15vector.load(6, 1, data.forcetorque);
    currtareQ15vector.load(6, 1, data.tare);
    adcvalueQ15vector.load(6, 1, data.q15value);
  }

  ~StrainRuntimeData() {}

  void dump()
  {
      std::cout<<(float)(data.force.x-32768)*0.45<<"::"<<(float)(data.force.y-32768)*0.45<<"::"<<(float)(data.force.z-32768)*0.45<<"::"<<std::endl;
      std::cout<<(float)(data.torque.x-32768)*0.45<<"::"<<(float)(data.torque.y-32768)*0.45<<"::"<<(float)(data.torque.z-32768)*0.45<<"::"<<std::endl;

      std::cout<<(float)data.force.x<<"::"<<(float)data.force.y<<"::"<<(float)data.force.z<<"::"<<std::endl;
      std::cout<<(float)data.torque.x<<"::"<<(float)data.torque.y<<"::"<<(float)data.torque.z<<"::"<<std::endl;
  }

  bool setdefault() {
    clear();
    return true;
  }
};