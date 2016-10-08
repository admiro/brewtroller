  /*
Arduino Mega 1280 Admiro
*/

#ifndef BT_HWPROFILE
#define BT_HWPROFILE

  //**********************************************************************************
  // ENCODER TYPE
  //**********************************************************************************
  // You must uncomment one and only one of the following ENCODER_ definitions
  // Use ENCODER_ALPS for ALPS and Panasonic Encoders
  // Use ENCODER_CUI for older CUI encoders
  //
  //#define ENCODER_TYPE ALPS
  #define ENCODER_TYPE CUI
  //**********************************************************************************

  #define ENCODER_OLD_CONSTRUCTOR   //Admiro Mega profile
  #define ENCA_PIN 21               
  #define ENCB_PIN 20               
  #define ENTER_PIN 19              
  #define ENTER_INT 4               
  #define ENCA_INT 2                
  #define ENCODER_ACTIVELOW
  #define ENCODER_DEBOUNCE 200  

  #define OUTPUTBANK_GPIO           //Admiro Mega profile  
  #define OUTPUTBANK_GPIO_BANKNAME "MEGA"
  #define OUTPUTBANK_GPIO_COUNT 7
  #define OUTPUTBANK_GPIO_PINS {8, 9, 10, 11, 14, 15, 16}
  #define OUTPUTBANK_GPIO_OUTPUTNAMES "HLT\0Mash\0Kettle\0Alarm\0Led1\0Led2\0Led3"
  #define OUTPUTBANK_GPIO_ACTIVELOW_PINS {false, false ,false, false, false, false, false} //Admiro Added active low output configured in HWProfile.w (ie. relay module)

  #define HLTVOL_APIN 3
  #define MASHVOL_APIN 2
  #define KETTLEVOL_APIN 1
  #define STEAMPRESS_APIN 0
  
  #define ANALOGINPUTS_GPIO
  #define ANALOGINPUTS_GPIO_COUNT 3
  #define ANALOGINPUTS_GPIO_PINS {3, 2, 1}
  #define ANALOGINPUTS_GPIO_NAMES "HLT\0Mash\0Kettle"

  #define UI_LCD_4BIT               //Admiro Mega profile
  #define LCD_RS_PIN 2
  #define LCD_ENABLE_PIN 3
  #define LCD_DATA4_PIN 4
  #define LCD_DATA5_PIN 5
  #define LCD_DATA6_PIN 6
  #define LCD_DATA7_PIN 7

  #define HLT_AS_KETTLE //Admiro remaps the Kettle temp sensor to the HLT's device
 
  // BTPD_SUPPORT: Enables use of BrewTroller PID Display devices on I2C bus
 // #define BTPD_SUPPORT
  
  // BTNIC_EMBEDDED: Enables use of on-board I2C Ethernet module
//  #define BTNIC_EMBEDDED
  
//  #define HEARTBEAT
//  #define HEARTBEAT_PIN 0

  //**********************************************************************************
  // OneWire Temperature Sensor Options
  //**********************************************************************************
  // TS_ONEWIRE: Enables use of OneWire Temperature Sensors (Future logic may
  // support alternatives temperature sensor options.)
  #define TS_ONEWIRE
  #define TS_ONEWIRE_GPIO
  #define TEMP_PIN 12
  
  // TS_ONEWIRE_PPWR: Specifies whether parasite power is used for OneWire temperature
  // sensors. Parasite power allows sensors to obtain their power from the data line
  // but significantly increases the time required to read the temperature (94-750ms
  // based on resolution versus 10ms with dedicated power).
  #define TS_ONEWIRE_PPWR 0

  // TS_ONEWIRE_RES: OneWire Temperature Sensor Resolution (9-bit - 12-bit). Valid
  // options are: 9, 10, 11, 12). Unless parasite power is being used the recommended
  // setting is 12-bit (for DS18B20 sensors). DS18S20 sensors can only operate at a max
  // of 9 bit. When using parasite power decreasing the resolution reduces the 
  // temperature conversion time: 
  //   12-bit (0.0625C / 0.1125F) = 750ms 
  //   11-bit (0.125C  / 0.225F ) = 375ms 
  //   10-bit (0.25C   / 0.45F  ) = 188ms 
  //    9-bit (0.5C    / 0.9F   ) =  94ms   
  #define TS_ONEWIRE_RES 11

  // TS_ONEWIRE_FASTREAD: Enables faster reads of temperatures by reading only the first
  // 2 bytes of temperature data and ignoring CRC check.
  #define TS_ONEWIRE_FASTREAD

  // DS2482_ADDR: I2C Address of DS2482 OneWire Master (used for TS_OneWire_I2C)
  // Should be 0x18, 0x19, 0x1A, 0x1B
  #define DS2482_ADDR 0x1B
  //**********************************************************************************


  //**********************************************************************************
  // Serial0 Communication Options
  //**********************************************************************************
  // COM_SERIAL0: Specifies the communication type being used (Pick One):
  //  ASCII  = Original BrewTroller serial command protocol used with BTRemote and BTLog
  //  BTNIC  = BTnic (Lighterweight implementation of ASCII protocol using single-byte
  //           commands. This protocol is used with BTnic Modules and software for
  //           network connectivity.
  //  BINARY = Binary Messages
  //**********************************************************************************
  
  //#define COM_SERIAL0  ASCII
  #define COM_SERIAL0  BTNIC
  //#define COM_SERIAL0 BINARY
  
  // BAUD_RATE: The baud rate for the Serial0 connection. Previous to BrewTroller 2.0
  // Build 419 this was hard coded to 9600. Starting with Build 419 the default rate
  // was increased to 115200 but can be manually set using this compile option.
  #define SERIAL0_BAUDRATE 115200
  
#endif
