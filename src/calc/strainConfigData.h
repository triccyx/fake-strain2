#pragma once

#include "embot_dsp.h"
#include "enum.h"
#include <cinttypes>
#include <cstring>

 
    // the new amplifier is the pga308 by Texas Instruments. It has a completly different configuration
    // we use for it the compact form of the can message
    
    struct amplifierConfig_v00_t
    { 
        embot::prot::can::analog::polling::PGA308cfg1 pga308cfg1 {};   
        
        void factoryreset() {
            embot::hw::pga308::TransferFunctionConfig tfc {};
            tfc.setDefault();
            std::uint16_t       GD;
            std::uint8_t        GI; 
            std::uint8_t        S;
            std::uint8_t        GO; 
            std::uint8_t        Voffsetcoarse;
            std::uint16_t       Vzerodac;
            tfc.get(GD, GI, S, GO, Voffsetcoarse, Vzerodac);
            pga308cfg1 = {GD, GI, S, GO, Voffsetcoarse, Vzerodac};
        }
        
        amplifierConfig_v00_t() { factoryreset(); }
    }; 
    // _VERIFYsizeof(amplifierConfig_v00_t, 6)Luca
    
    struct amplifierConfig_t
    {   // it is _v01_t 
        embot::prot::can::analog::polling::PGA308cfg1 pga308cfg1 {}; 
        std::uint8_t ffu[2] {0};        
        
        void factoryreset() {
            embot::hw::pga308::TransferFunctionConfig tfc {};
            tfc.setDefault();
            std::uint16_t       GD;
            std::uint8_t        GI; 
            std::uint8_t        S;
            std::uint8_t        GO; 
            std::uint8_t        Voffsetcoarse;
            std::uint16_t       Vzerodac;
            tfc.get(GD, GI, S, GO, Voffsetcoarse, Vzerodac);
            pga308cfg1 = {GD, GI, S, GO, Voffsetcoarse, Vzerodac};
            ffu[0] = ffu[1] = 0;
        }
        
        amplifierConfig_t() { factoryreset(); }
        
        void import(const amplifierConfig_v00_t& v00)
        {
            std::memmove(&pga308cfg1, &v00.pga308cfg1, sizeof(pga308cfg1));
            std::memset(ffu, 0, sizeof(ffu));
        }
        
    };
    // _VERIFYsizeof(amplifierConfig_t, 8) Luuca
    
    

    // the matrix transformation which is done after adc acquisition    
    // tf = M * (adc + tare)
    // then tf can also be corrected with an extra tare which is set in runtime: TF = tf + tare_set_in_runtime
    // the 6 values of TF are those transmitted to the host.
    // in here are also stored fullscale (and gain?) which are queried and used by the remote host to compensate the received TF values
    
    struct transformerConfig_v00_t
    {
        std::uint8_t                dummy;          // just to remove a hole
        std::uint8_t                gain;           // the gain managed with GET_MATRIX_G / SET_MATRIX_G which is NOT USED INSIDE THE MPU        
        std::uint16_t               fullscale[6];   // the fullscale of the measurements: NOT USED INSIDE THE MPU
        embot::dsp::Q15             tare[6];        // in format 1Q15: the stored tare
        embot::dsp::Q15             matrix[6*6];    // in format 1Q15: transformation matrix from the 6 strain-gauge values to the 6 force-torque values        
    };
    //  _VERIFYsizeof(transformerConfig_v00_t, 98)  Luca
    
    struct transformerConfig_t
    {
        std::uint8_t                gain;           // the gain managed with GET_MATRIX_G / SET_MATRIX_G which is NOT USED INSIDE THE MPU   
        std::uint8_t                ffu[7];
        std::uint16_t               fullscale[6];   // the fullscale of the measurements: NOT USED INSIDE THE MPU
        embot::dsp::Q15             tare[6];        // in format 1Q15: the stored tare
        embot::dsp::Q15             matrix[6*6];    // in format 1Q15: transformation matrix from the 6 strain-gauge values to the 6 force-torque values 
        
        void import(const transformerConfig_v00_t& v00)
        {
            gain = v00.gain;
            std::memset(ffu, 0, sizeof(ffu));
            std::memmove(fullscale, v00.fullscale, sizeof(fullscale));
            std::memmove(tare, v00.tare, sizeof(tare));
            std::memmove(matrix, v00.matrix, sizeof(matrix));
        }
        
    }; 
    // _VERIFYsizeof(transformerConfig_t, 104)  Luca
    
    
    
    // the complete configuration for the transformer and the six amplifiers
    
    struct setOfConfig_v00_t
    {
        transformerConfig_v00_t transformer;            // the config of the transformer from dacs to forcetorque array
        amplifierConfig_v00_t   amplifiers[6];          // the NEW config of the amplifiers from strain gauge to values given to dacs          
    }; 
    // _VERIFYsizeof(setOfConfig_v00_t, 134) Luca
    

struct strainConfigData_v00_t {
  std::uint32_t key;
  char serial[8]; // the serial number of the strain. it start at offset +4
  setOfConfig_v00_t
      set[numOfSets]; // the three configurations sets. they start at offset +12
};
// _VERIFYsizeof(strainConfigData_v00_t, 416) Luca

    // it is version v01 and with respect to version v00 has a remapping of teh
    // same data into multiples of 16. plus it has permanentregulationset.
    struct strainConfigData_t {
  std::uint32_t key;
  char serial[8]; // the serial number of the strain  (at offset +4)
  std::uint8_t
      permanentregulationset;   // the set to use at bootstrap (at offset +12)
  std::uint8_t ffu[3];          // a useful hole in the eeprom (at offset +13)
  setOfConfig_t set[numOfSets]; // the three configurations sets (at offset +16)

  void import(const strainConfigData_v00_t &v00) {
    key = validityKey;
    std::memmove(serial, v00.serial, sizeof(serial));
    permanentregulationset = 0;
    std::memset(ffu, 0, sizeof(ffu));
    for (std::uint8_t s = 0; s < numOfSets; s++) {
      set[s].import(v00.set[s]);
    }
  }
};
//_VERIFYsizeof(strainConfigData_t, 472) Luca

    // the class which manages the configuration data which is also stored in
    // eeprom
    class StrainConfigData {
private:
  // data contains what is also stored in eeprom. only eepromread() and
  // eepromwrite() operate on eeprom and take care to set synched to true. there
  // are methods used to modify data which also set synched to false. HENCE: DO
  // NOT CHANGE data by hand.
  strainConfigData_t data; // we save it in eeprom when requested
  bool synched;            // true if data is equal to what is in eeprom

private:
  embot::dsp::q15::matrix transfQ15matrix[numOfSets];
  embot::dsp::q15::matrix transfQ15tare[numOfSets];

public:
  // methods

  bool isEEPROMsynched() { return synched; }

  bool migrate(std::uint32_t fromkey) {
    if (validityKey == fromkey) {
      return true;
    }

    if (validityKey_v00 != fromkey) {
      return false;
    }

    // ok, ... we do business; strainConfigData_v00_t is smaller than
    // strainConfigData_t
    strainConfigData_v00_t datav00;
    std::memmove(&datav00, &data, sizeof(strainConfigData_v00_t));
    // now we get prepare data and then... we copy old stuff inside
    def_values();
    data.import(datav00);

    return true;
  }

  bool EEPROM_read() {
#if defined(STRAIN2_APP_AT_64K)
    embot::app::theCANboardInfo &canbrdinfo =
        embot::app::theCANboardInfo::getInstance();
    canbrdinfo.userdataread(0, sizeof(data), &data);
#else
    embot::app::theStorage &storage = embot::app::theStorage::getInstance();
    storage.read(0, sizeof(data), &data);
#endif
    synched = true;

    if (validityKey != data.key) { // if it is not valid, then
      // 1. attempt to migrate.
      // 2. if operation fails ... set default and impose validity key
      if (false == migrate(data.key)) {
        def_values();
        data.key = validityKey;
      }
      EEPROM_write();
    }

    return synched;
  }

  bool EEPROM_write() {
    data.key = validityKey;
#if defined(STRAIN2_APP_AT_64K)
    embot::app::theCANboardInfo &canbrdinfo =
        embot::app::theCANboardInfo::getInstance();
    canbrdinfo.userdatawrite(0, sizeof(data), &data);
#else
    embot::app::theStorage &storage = embot::app::theStorage::getInstance();
    storage.write(0, sizeof(data), &data);
#endif
    synched = true;
    return synched;
  }

  bool clear() {
    std::memset(&data, 0, sizeof(data));
    data.key = validityKey;
    synched = false;
    return synched;
  }

  StrainConfigData() {
#if defined(STRAIN2_APP_AT_64K)
#else
    embot::app::theStorage &storage = embot::app::theStorage::getInstance();
    storage.init(
        embot::hw::flash::getpartition(embot::hw::FLASH::applicationstorage)
            .address,
        1024);
#endif
    clear();

    for (int s = 0; s < numOfSets; s++) {
      transfQ15matrix[s].load(6, 6, data.set[s].transformer.matrix);
      transfQ15tare[s].load(6, 1, data.set[s].transformer.tare);
    }
  }

  ~StrainConfigData() {}

  void serial_set(const char *str) {
    std::snprintf(data.serial, sizeof(data.serial), "%s", str);
    synched = false;
  }

  const char *serial_get() { return data.serial; }

  void permanentregulationset_set(uint8_t set) {
    if (set < numOfSets) {
      data.permanentregulationset = set;
      synched = false;
    }
  }

  uint8_t permanentregulationset_get() { return data.permanentregulationset; }

  void
  amplifiers_set(std::uint8_t set, std::uint8_t channel,
                 const embot::prot::can::analog::polling::PGA308cfg1 &cfg1) {
    if ((channel < numOfChannels) && (set < numOfSets)) {
      data.set[set].amplifiers[channel].pga308cfg1 = cfg1;
      synched = false;
    }
  }

  bool amplifiers_get(std::uint8_t set, std::uint8_t channel,
                      embot::prot::can::analog::polling::PGA308cfg1 &cfg1) {
    bool ret = false;
    if ((channel < numOfChannels) && (set < numOfSets)) {
      cfg1 = data.set[set].amplifiers[channel].pga308cfg1;
      ret = true;
    }

    return ret;
  }

  void amplifiers_reset(std::uint8_t set, std::uint8_t channel) {
    if ((channel < numOfChannels) && (set < numOfSets)) {
      data.set[set].amplifiers[channel].factoryreset();
      synched = false;
    }
  }

  const embot::dsp::q15::matrix &transformer_matrix_handle(std::uint8_t set) {
    if (set >= numOfSets) {
      set = 0;
    }
    return transfQ15matrix[set];
  }

  const embot::dsp::q15::matrix &transformer_tare_handle(std::uint8_t set) {
    if (set >= numOfSets) {
      set = 0;
    }
    return transfQ15tare[set];
  }

  void transformer_matrix_fill_identity(std::uint8_t set) {
    if (set < numOfSets) {
      transfQ15matrix[set].diagonal(embot::dsp::q15::posOneNearly);
      synched = false;
    }
  }

  void transformer_matrix_fill_diagonal(std::uint8_t set,
                                        embot::dsp::Q15 value) {
    if (set < numOfSets) {
      transfQ15matrix[set].diagonal(value);
      synched = false;
    }
  }

  void transformer_gain_set(std::uint8_t set, std::uint8_t g) {
    if (set < numOfSets) {
      data.set[set].transformer.gain = g;
      synched = false;
    }
  }

  std::uint8_t transformer_gain_get(std::uint8_t set) {
    std::uint8_t g = 0;
    if (set < numOfSets) {
      g = data.set[set].transformer.gain;
    }
    return g;
  }

  void transformer_matrix_set(std::uint8_t set, std::uint8_t r, std::uint8_t c,
                              embot::dsp::Q15 value) {
    if ((set < numOfSets) && (r < numOfChannels) && (c < numOfChannels)) {
      transfQ15matrix[set].set(r, c, value);
      synched = false;
    }
  }

  embot::dsp::Q15 transformer_matrix_get(std::uint8_t set, std::uint8_t r,
                                         std::uint8_t c) {
    embot::dsp::Q15 v = embot::dsp::q15::zero;

    if ((set < numOfSets) && (r < numOfChannels) && (c < numOfChannels)) {
      v = transfQ15matrix[set].get(r, c);
    }

    return v;
  }

  void transformer_tare_fill(std::uint8_t set, embot::dsp::Q15 value) {
    if (set < numOfSets) {
      transfQ15tare[set].fill(value);
      synched = false;
    }
  }

  void transformer_tare_set(std::uint8_t set, std::uint8_t channel,
                            embot::dsp::Q15 value) {
    if ((set < numOfSets) && (channel < numOfChannels)) {
      transfQ15tare[set].set(channel, 0, value);
      synched = false;
    }
  }

  embot::dsp::Q15 transformer_tare_get(std::uint8_t set, std::uint8_t channel) {
    embot::dsp::Q15 v = embot::dsp::q15::zero;
    if ((set < numOfSets) && (channel < numOfChannels)) {
      v = transfQ15tare[set].get(channel, 0);
    }
    return v;
  }

  bool def_values() {
    clear();

    data.key = validityKey;
    std::snprintf(data.serial, sizeof(data.serial), "SN0000");
    data.permanentregulationset = 0;
    // and now the set
    for (int s = 0; s < numOfSets; s++) {
      // transformer
      transformer_gain_set(s, 1);
      transformer_fullscale_fill(s, 0x7fff);
      transformer_tare_fill(s, embot::dsp::q15::zero);
      transformer_matrix_fill_diagonal(s, embot::dsp::q15::posOneNearly);
      transformer_gain_set(s, 1);

      for (int chn = 0; chn < numOfChannels; chn++) {
        amplifiers_reset(s, chn);
      }
    }

    synched = false;
    return synched;
  }

  std::uint16_t transformer_fullscale_get(std::uint8_t set,
                                          std::uint8_t channel) {
    if ((set < numOfSets) && (channel < numOfChannels)) {
      return data.set[set].transformer.fullscale[channel];
    } else {
      return 0;
    }
  }

  void transformer_fullscale_set(std::uint8_t set, std::uint8_t channel,
                                 std::uint16_t value) {
    if ((set < numOfSets) && (channel < numOfChannels)) {
      data.set[set].transformer.fullscale[channel] = value;
      synched = false;
    }
  }

  void transformer_fullscale_fill(std::uint8_t set, std::uint16_t value) {
    for (int channel = 0; channel < numOfChannels; channel++) {
      transformer_fullscale_set(set, channel, value);
    }
  }
};