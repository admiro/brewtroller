#ifndef Vessel_h
#define Vessel_h

#include "BrewTrollerApplication.h"
#include "LOCAL_PID_v1.h"
#include "Outputs.h"
#include "LOCAL_PID_AutoTune_v0.h"

struct Calibration {
  unsigned int inputValue;
  unsigned long outputValue;
};

extern OutputSystem *outputs;

class Vessel {
  private:
    PID *pid;
    double PIDInput, PIDOutput, setpoint;
    byte ATuneModeRemember;
    boolean tuning;
    PID_ATune *aTune;
    analogOutput *pwmOutput;
    byte pwmActiveProfile, heatProfile, idleProfile;
    int *temperature;
    byte vSensor;
    unsigned long targetVolume, volume;
    long flowRate;  //Thousandths of gal/l per minute
    struct Calibration volumeCalibration[10];
    byte hysteresis;
    boolean heatStatus;
    unsigned int volumeReadings[VOLUME_READ_COUNT], lastFlowrateVolume;
    unsigned long lastVolumeRead, lastFlowrateRead;
    byte volumeReadCursor;

    void updateHeat(void);
    void setHeatStatus(boolean status);
    void updatePIDHeat(void);
    void updateVolume(void);
    void updateFlowRate(void);
    unsigned long calibrateVolume(unsigned int aValue);

  public:
    Vessel(int *t, byte pwmActive, byte heat, byte idle);
    ~Vessel(void);
    void update(void);
    PID* getPID(void);
    analogOutput* getPWMOutput(void);
    void setPWMOutput(analogOutput *aout);
    byte getVolumeInput(void);
    void setVolumeInput(byte pin);
    double getSetpoint(void);
    double setSetpoint(double value);
    byte getHysteresis(void);
    void setHysteresis(byte value);
    unsigned long getTemperature(void);
    unsigned long getVolume(void);
    unsigned long getTargetVolume(void);
    void setTargetVolume(unsigned long target);
    long getFlowRate(void);
    byte getHeatPower(void);
    void startAutoTune(int useDerivative, unsigned long aTuneStartValue, unsigned long aTuneStep, double aTuneNoise, unsigned int aTuneLookBack);
    void stopAutoTune();
    boolean isTuning();
    PID_ATune* getPIDAutoTune();
    struct Calibration getVolumeCalibration(byte index);
    void setVolumeCalibration(byte index, struct Calibration calibration);
    unsigned int getRawVolumeValue(void);
};

#endif