#include "RGBIO8.h"

OutputSystem* RGBIO8::outputs;
uint16_t RGBIO8::output_recipes[RGBIO8_MAX_OUTPUT_RECIPES][RGBIORECIPE_MODECOUNT];

RGBIO8::RGBIO8(byte i2c_address) {
  this->i2c_address = i2c_address;
  for (int i = 0; i < 8; i++)
    assignments[i].index = RGBIO8_UNASSIGNED;
}

void RGBIO8::setup(OutputSystem* o) {
    outputs = o;
}

void RGBIO8::setOutputRecipe(
  byte recipe_id, 
  uint16_t off_rgb,
  uint16_t auto_off_rgb,
  uint16_t auto_on_rgb,
  uint16_t on_rgb) {
    output_recipes[recipe_id][RGBIORECIPE_OFF] = off_rgb;
    output_recipes[recipe_id][RGBIORECIPE_AUTOOFF] = auto_off_rgb;
    output_recipes[recipe_id][RGBIORECIPE_AUTOON] = auto_on_rgb;
    output_recipes[recipe_id][RGBIORECIPE_ON] = on_rgb;
}

void RGBIO8::assign(byte assignment, byte outputIndex, byte recipe_id) {
  assignments[assignment].index = outputIndex;
  assignments[assignment].recipe_id = recipe_id;
}

void RGBIO8::update(void) {
  // Get the state of the 8 inputs first
  getInputs();
  
  // Update any assigned inputs
  for (int i = 0; i < 8; i++) {
    unsigned long mask = (unsigned long) 1 << i;
    
    RGBIO8_assignment *a = &assignments[i];
    if (a->index != RGBIO8_UNASSIGNED) {
      //Update input
      if (inputs_manual & mask) {
        outputs->setProfileMaskBit(OUTPUTPROFILE_RGBIO, a->index, 1);
        outputs->setOutputEnable(OUTPUTENABLE_RGBIO, a->index, 1);
        setOutput(i, output_recipes[a->recipe_id][RGBIORECIPE_ON]);
      } else if (inputs_auto & mask) {
        outputs->setProfileMaskBit(OUTPUTPROFILE_RGBIO, a->index, 0);
        outputs->setOutputEnable(OUTPUTENABLE_RGBIO, a->index, 1);
        if (outputs->getOutputState(a->index))
          setOutput(i, output_recipes[a->recipe_id][RGBIORECIPE_AUTOON]);
        else
          setOutput(i, output_recipes[a->recipe_id][RGBIORECIPE_AUTOOFF]);
      } else {
        outputs->setProfileMaskBit(OUTPUTPROFILE_RGBIO, a->index, 0);
        outputs->setOutputEnable(OUTPUTENABLE_RGBIO, a->index, 0);
        setOutput(i, output_recipes[a->recipe_id][RGBIORECIPE_OFF]);
      }
    }
  }
}

void RGBIO8::restart() {
  Wire.beginTransmission(i2c_address);
  Wire.write(0xfd);
  Wire.endTransmission();
}

void RGBIO8::setIdMode(byte id_mode) {
  Wire.beginTransmission(i2c_address);
  Wire.write(0xfe);
  Wire.write(id_mode);
  Wire.endTransmission();
}

void RGBIO8::setAddress(byte a) {
  Wire.beginTransmission(i2c_address);
  Wire.write(0xff);
  Wire.write(a);
  Wire.endTransmission();
}

int RGBIO8::getInputs(void) {
  Wire.requestFrom(i2c_address, (uint8_t) 3);
  uint8_t inputs_m = Wire.read();
  uint8_t inputs_a = Wire.read();
  uint8_t crc = Wire.read();
  
  uint8_t crc_comp = '*';
  crc_comp = crc8(crc_comp, inputs_m);
  crc_comp = crc8(crc_comp, inputs_a);
  
  if (crc == crc_comp) {
    inputs_manual = inputs_m;
    inputs_auto = inputs_a;
    return 1;
  }
  else {
    return 0;
  }
}

void RGBIO8::setOutput(byte output, uint16_t rgb) {
  Wire.beginTransmission(i2c_address);
  Wire.write(0x01);
  Wire.write(output);
  Wire.write((uint8_t*) &rgb, 2);
  Wire.endTransmission();
}

uint8_t RGBIO8::crc8(uint8_t inCrc, uint8_t inData ) {
  uint8_t i;
  uint8_t data;

  data = inCrc ^ inData;
  
  for (i = 0; i < 8; i++) {
    if ((data & 0x80) != 0) {
      data <<= 1;
      data ^= 0x07;
    }
    else {
      data <<= 1;
    }
  }

  return data;
}