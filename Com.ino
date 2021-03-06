/*  
   Copyright (C) 2009, 2010 Matt Reba, Jeremiah Dillingham

    This file is part of BrewTroller.

    BrewTroller is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    BrewTroller is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with BrewTroller.  If not, see <http://www.gnu.org/licenses/>.


BrewTroller - Open Source Brewing Computer
Software Lead: Matt Reba (matt_AT_brewtroller_DOT_com)
Hardware Lead: Jeremiah Dillingham (jeremiah_AT_brewtroller_DOT_com)

Documentation, Forums and more information available at http://www.brewtroller.com

*/

//**********************************************************************************
//Code Shared by all Schemas
//**********************************************************************************
void comInit() {
  #ifdef COM_SERIAL0
    Serial.begin(SERIAL0_BAUDRATE);
  #endif
}

void updateCom() {
  #ifdef COM_SERIAL0
    #if COM_SERIAL0 == BTNIC
      updateS0BTnic();
    #endif
  #endif
  #ifdef BTNIC_EMBEDDED
    updateI2CBTnic();
  #endif
  //BTPD Support
  #ifdef BTPD_SUPPORT
    updateBTPD();
  #endif
}

/********************************************************************************************************************
 * BTnic Instances
 ********************************************************************************************************************/
#ifdef BTNIC_PROTOCOL
  #include "Com_BTnic.h"
  
  #ifdef BTNIC_EMBEDDED
    BTnic btnicI2C;
    
    void updateI2CBTnic() {
      switch (btnicI2C.getState()) {
        case BTNIC_STATE_IDLE:
        case BTNIC_STATE_RX:
          while (1) {
            Wire.requestFrom(BTNIC_I2C_ADDR, 1);
            if (! Wire.available())
              break; //No data
            byte data = Wire.read();
            if (!data)
              break; //Null return: No data
            #ifdef DEBUG_BTNIC
              Serial.print("btnicEmb RX: ");
              Serial.print(data);
              Serial.print("(0x");
              Serial.print(data, HEX);
              Serial.print(", ");
              Serial.print(data, DEC);
              Serial.println(")");
            #endif
            btnicI2C.rx(data);
            if(btnicI2C.getState() != BTNIC_STATE_RX) break;
          }
          
        case BTNIC_STATE_TX:
          //TX Ready
          while(btnicI2C.getState() == BTNIC_STATE_TX) {
            byte maxLength = 32;
            Wire.beginTransmission(BTNIC_I2C_ADDR);
            while(maxLength-- && btnicI2C.getState() == BTNIC_STATE_TX) {
              byte data = btnicI2C.tx();
              #ifdef DEBUG_BTNIC
                Serial.print("btnicEmb TX: ");
                Serial.print(data);
                Serial.print("(0x");
                Serial.print(data, HEX);
                Serial.print(", ");
                Serial.print(data, DEC);
                Serial.println(")");
              #endif
              Wire.write(data);
            }
            Wire.endTransmission();
          }
          break;
      }
    }
  #endif
  
  #ifdef COM_SERIAL0
    #if COM_SERIAL0 == BTNIC /* BTnic over Serial0 */
      BTnic btnicS0;
      void updateS0BTnic() {
        switch (btnicS0.getState()) {
          case BTNIC_STATE_IDLE:
          case BTNIC_STATE_RX:
            while (Serial.available()) {
              btnicS0.rx(Serial.read());
              if(btnicS0.getState() != BTNIC_STATE_RX) break;
            }
            if(btnicS0.getState() != BTNIC_STATE_TX) break;
          case BTNIC_STATE_TX:
            //TX Ready
            while(btnicS0.getState() == BTNIC_STATE_TX) Serial.write(btnicS0.tx());
        }
      }
    #endif
  #endif
#endif

void comEvent(byte eventID, int eventParam) {
  #ifdef BTNIC_PROTOCOL
    #ifdef BTNIC_EMBEDDED
      btnicI2C.eventHandler(eventID, eventParam);
    #endif
    #ifdef COM_SERIAL0
      #if COM_SERIAL0 == BTNIC /* BTnic over Serial0 */
        btnicS0.eventHandler(eventID, eventParam);
      #endif
    #endif
  #endif
}
