#ifndef DIGITIZER_DEFS_H
#define DIGITIZER_DEFS_H

#include <CAENDigitizerType.h>
#include <string>
#include <vector>

namespace Daqromancy {

    namespace Data
    {
        static constexpr uint64_t dataSize = 24; //size of writable data in bytes (no waveforms)
    }

    enum class DigitizerAccessType
    {
        All,
        Single,
        None
    };

    //Unified Daqromancy Data structure (similar to CAEN CoMPASS)
    struct DYData
    {
        uint16_t board;
        uint16_t channel;
        uint64_t timestamp;
        uint32_t energy;
        uint32_t energyShort;
        uint32_t flags;
        uint32_t waveSize;
        std::vector<uint16_t> trace1Samples; //For waves: traces are actual data to be saved potentially, digital is result of algorithms
        std::vector<uint16_t> trace2Samples;
        std::vector<uint8_t> digitalTrace1Samples;
        std::vector<uint8_t> digitalTrace2Samples; //Technically DPP supports 4 in docs? But all examples only seem to support 2...
    };

    enum DPPAcqMode
    {
        List = 1,
        Waves = 2,
        None = 0
    };

    enum class SyncStartType
    {
        HardwareControlled,
        SoftwareControlled,
        None
    };

    //Add more later, right now only accept SPS standard
    enum class SyncMethod
    {
        SIn_TrigOut,
        None
    };

    struct SyncArgs
    {
        SyncStartType startType = SyncStartType::SoftwareControlled;
        SyncMethod syncMethod = SyncMethod::SIn_TrigOut;
    };

    //These need to be plain enums cause we're gonna cast them to int, so just be careful
    enum DynamicRange
    {
        Volt_2 = 0, //2Volt
        MilliVolt_500 = 1//0.5Volt
    };

    enum PHAVirtualProbe1Options
    {
        PHAVP1_Input = CAEN_DGTZ_DPP_VIRTUALPROBE_Input,
        PHAVP1_Delta = CAEN_DGTZ_DPP_VIRTUALPROBE_Delta,
        PHAVP1_Delta2 = CAEN_DGTZ_DPP_VIRTUALPROBE_Delta2,
        PHAVP1_Trapezoid = CAEN_DGTZ_DPP_VIRTUALPROBE_Trapezoid
    };

    enum PHAVirtualProbe2Options
    {
        PHAVP2_Input = CAEN_DGTZ_DPP_VIRTUALPROBE_Input,
        PHAVP2_Threshold = CAEN_DGTZ_DPP_VIRTUALPROBE_Threshold,
        PHAVP2_Baseline = CAEN_DGTZ_DPP_VIRTUALPROBE_Baseline,
        PHAVP2_TrapezoidReduced = CAEN_DGTZ_DPP_VIRTUALPROBE_TrapezoidReduced,
        PHAVP2_None = CAEN_DGTZ_DPP_VIRTUALPROBE_None
    };

    enum PHADigitalProbe1Options
    {
        PHADP_TriggerWindow = CAEN_DGTZ_DPP_DIGITALPROBE_TRGWin,
        PHADP_Armed = CAEN_DGTZ_DPP_DIGITALPROBE_Armed,
        PHADP_PkRun = CAEN_DGTZ_DPP_DIGITALPROBE_PkRun,
        PHADP_PileUp = CAEN_DGTZ_DPP_DIGITALPROBE_PileUp,
        PHADP_Peaking = CAEN_DGTZ_DPP_DIGITALPROBE_Peaking,
        PHADP_CoincidenceWindow = CAEN_DGTZ_DPP_DIGITALPROBE_CoincWin,
        PHADP_BaselineFreeze = CAEN_DGTZ_DPP_DIGITALPROBE_BLFreeze,
        PHADP_TriggerHoldoff = CAEN_DGTZ_DPP_DIGITALPROBE_TRGHoldoff,
        PHADP_TriggerValue = CAEN_DGTZ_DPP_DIGITALPROBE_TRGVal,
        PHADP_ACQVeto = CAEN_DGTZ_DPP_DIGITALPROBE_ACQVeto,
        PHADP_BFMVeto = CAEN_DGTZ_DPP_DIGITALPROBE_BFMVeto,
        PHADP_ExternalTrig = CAEN_DGTZ_DPP_DIGITALPROBE_ExtTRG,
        PHADP_Busy = CAEN_DGTZ_DPP_DIGITALPROBE_Busy,
        PHADP_PrgVeto = CAEN_DGTZ_DPP_DIGITALPROBE_PrgVeto
    };

    enum PSDVirtualProbe1Options
    {
        PSDVP1_Input = CAEN_DGTZ_DPP_VIRTUALPROBE_Input,
        PSDVP1_CFD = CAEN_DGTZ_DPP_VIRTUALPROBE_CFD
    };

    enum PSDVirtualProbe2Options
    {
        PSDVP2_Baseline = CAEN_DGTZ_DPP_VIRTUALPROBE_Baseline,
        PSDVP2_CFD = CAEN_DGTZ_DPP_VIRTUALPROBE_CFD,
        PSDVP2_None = CAEN_DGTZ_DPP_VIRTUALPROBE_None
    };

    enum PSDDigitalProbe1Options
    {
        PSDDP1_Gate = CAEN_DGTZ_DPP_DIGITALPROBE_Gate,
        PSDDP1_OverThreshold = CAEN_DGTZ_DPP_DIGITALPROBE_OverThr,
        PSDDP1_TriggerOut = CAEN_DGTZ_DPP_DIGITALPROBE_TRGOut,
        PSDDP1_CoincidenceWindow = CAEN_DGTZ_DPP_DIGITALPROBE_CoincWin,
        PSDDP1_PileUp = CAEN_DGTZ_DPP_DIGITALPROBE_PileUp,
        PSDDP1_Coincidence = CAEN_DGTZ_DPP_DIGITALPROBE_Coincidence,
        PSDDP1_Trigger = CAEN_DGTZ_DPP_DIGITALPROBE_Trigger
    };

    enum PSDDigitalProbe2Options
    {
        PSDDP2_GateShort = CAEN_DGTZ_DPP_DIGITALPROBE_GateShort,
        PSDDP2_OverThreshold = CAEN_DGTZ_DPP_DIGITALPROBE_OverThr,
        PSDDP2_TriggerVal = CAEN_DGTZ_DPP_DIGITALPROBE_TRGVal,
        PSDDP2_TriggerHoldoff = CAEN_DGTZ_DPP_DIGITALPROBE_TRGHoldoff,
        PSDDP2_PileUp = CAEN_DGTZ_DPP_DIGITALPROBE_PileUp,
        PSDDP2_Coincidence = CAEN_DGTZ_DPP_DIGITALPROBE_Coincidence,
        PSDDP2_Trigger = CAEN_DGTZ_DPP_DIGITALPROBE_Trigger 
    };

    //Data vital to status/exsistance of digitizer
    struct DigitizerArgs
    {
        CAEN_DGTZ_ConnectionType type = CAEN_DGTZ_ConnectionType::CAEN_DGTZ_OpticalLink; //default (makes sense for FSU)
        int linkNumber = -1;
        int conetNode = -1;
        uint32_t vmeAddress = 0;
        int handle = -1;
        CAEN_DGTZ_BoardModel_t model = CAEN_DGTZ_V1730; //Find way for default?
        CAEN_DGTZ_DPPFirmware_t firmware = CAEN_DGTZ_NotDPPFirmware;
        std::string name = "None";
        int channels = 0; //number of channels in board
        int status = CAEN_DGTZ_NotYetImplemented;

        bool operator==(const DigitizerArgs& rhs)
        {
            if (this->type == rhs.type && this->linkNumber == rhs.linkNumber && this->conetNode == rhs.conetNode && this->vmeAddress == rhs.vmeAddress
                && this->handle == rhs.handle && this->model == rhs.model && this->firmware == rhs.firmware && this->name == rhs.name && this->channels == rhs.channels)
            {
                return true;
            }

            return false;
        }

        bool operator!=(const DigitizerArgs& rhs)
        {
            return !(*this == rhs);
        }
    };

    //Parameters that are applied to digitizer as a whole
    struct DigitizerParameters
    {
        uint32_t recordLength = 600; //nanoseconds
        uint32_t channelMask = 0xffff;
        int eventAggr = 0; //Allow CAEN to optimize
        CAEN_DGTZ_AcqMode_t acqMode = CAEN_DGTZ_SW_CONTROLLED;
        CAEN_DGTZ_DPP_AcqMode_t dppAcqMode = CAEN_DGTZ_DPP_ACQ_MODE_List;
        CAEN_DGTZ_DPP_SaveParam_t dppSaveMode = CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime;
        CAEN_DGTZ_IOLevel_t IOlevel = CAEN_DGTZ_IOLevel_NIM;
        CAEN_DGTZ_TriggerMode_t triggerMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
        CAEN_DGTZ_RunSyncMode_t syncMode = CAEN_DGTZ_RUN_SYNC_Disabled;
    };

    //Per channel PHA settings
    struct PHAParameters
    {
        bool isEnabled = true;
        uint32_t preTriggerTime = 120; //nanoseconds
        float dcOffset = 0.2f;
        CAEN_DGTZ_PulsePolarity_t pulsePolarity = CAEN_DGTZ_PulsePolarityPositive;
        DynamicRange dynamicRange = DynamicRange::Volt_2;
        int decayTimeConst = 100000; //nanoseconds
        int trapFlatTop = 1984; //nanoseconds
        int trapRiseTime = 384; //nanoseconds
        int flatTopDelay = 1000; //nanoseconds
        int triggerFilterSmoothing = 16; // Valid powers of 2 up to 32
        int inputRiseTime = 192; //nanoseconds
        int triggerThreshold = 300; //LSB
        int samplesBaseLineMean = 3; //Options: 1->16 samples; 2->64 samples; 3->256 samples; 4->1024 samples; 5->4096 samples; 6->16384 samples
        int samplesPeakMean = 3; //Options: 0-> 1 sample; 1->4 samples; 2->16 samples; 3->64 samples
        int peakHoldOff = 1600; //nanoseconds
        int baseLineHoldOff = 2000; //nanoseconds
        int otrej= 0; // n/a
        int triggerHoldOff = 1984; //nanoseconds
        int riseTimeValidationWindow = 400;//nanoseconds
        int riseTimeDiscrimination = 0; // 0 off 1 on
        int digitalProbeGain = 0; //Options: 0->DigitalGain=1; 1->DigitalGain=2 (only with decimation >= 2samples); 2->DigitalGain=4 (only with decimation >= 4samples); 3->DigitalGain=8( only with decimation = 8samples)
        float energyNormalizationFactor = 1.0; //Default 1.0
        int inputDecimation = 0; // decimation (the input signal samples are averaged within this number of samples): 0 ->disabled; 1->2 samples; 2->4 samples; 3->8 samples
    };

    //Per channel PSD settings
    struct PSDParameters
    {
        bool isEnabled = true;
        uint32_t preTriggerTime = 120; //nanoseconds
        float dcOffset = 0.2f;
        CAEN_DGTZ_PulsePolarity_t pulsePolarity = CAEN_DGTZ_PulsePolarityPositive;
        DynamicRange dynamicRange = DynamicRange::Volt_2;
        int baselineThreshold = 0; //Defined with fixed baseline only (LSB)
        int bltmo = 0;//n/a
        int triggerHoldOff = 1984; //nanoseconds
        int triggerThreshold = 300; //LSB   
        int selfTrigger = 1; //0-Disabled 1-Enabled 
        int chargeSensitivity = 1; //Comments only for 1720 & 1751? Check compass 
        int shortGate = 96; //nanoseconds
        int longGate = 400; //nanoseconds
        int preGate = 32; //nanoseconds
        int triggerValidationWindow = 4000; //nanoseconds
        int samplesBasline = 4; //Again only 1720 and 1751  
        int discrminatorType = CAEN_DGTZ_DPP_DISCR_MODE_CFD;
        int cfdFraction = 0; //0 25% 1 50% 2 75% 3 100%
        int cfdDelay = 16; //nanoseconds
        CAEN_DGTZ_DPP_TriggerConfig_t triggerConfig = CAEN_DGTZ_DPP_TriggerConfig_Threshold;
        CAEN_DGTZ_DPP_PUR_t pileUpRejection = CAEN_DGTZ_DPP_PSD_PUR_DetectOnly;
        int purgap = 100; //Purity Gap LSB
    };

    //Option of dual analog, which types (digital probe 2 is always trigger for PHA)
    struct PHAWaveParameters
    {
        CAEN_DGTZ_DPP_VirtualProbe_t isDual = CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL; //Default to a single analog trace
        PHAVirtualProbe1Options analogProbe1 = PHAVirtualProbe1Options::PHAVP1_Delta2; //Main analog trace defaults to delta2;
        PHAVirtualProbe2Options analogProbe2 = PHAVirtualProbe2Options::PHAVP2_Input; //Default val; in default config wont be displayed
        PHADigitalProbe1Options digitalProbe1 = PHADigitalProbe1Options::PHADP_Peaking; //Idk guess this is good default
    };

    struct PSDWaveParameters
    {
        CAEN_DGTZ_DPP_VirtualProbe_t isDual = CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL; //Default to a single analog trace
        PSDVirtualProbe1Options analogProbe1 = PSDVirtualProbe1Options::PSDVP1_Input; //Main trace defaults to input
        PSDVirtualProbe2Options analogProbe2 = PSDVirtualProbe2Options::PSDVP2_Baseline; //Defaults to off
        PSDDigitalProbe1Options digitalProbe1 = PSDDigitalProbe1Options::PSDDP1_Gate; //Defaults to long gate
        PSDDigitalProbe2Options digitalProbe2 = PSDDigitalProbe2Options::PSDDP2_GateShort; //Defaults to short gate
    };

    //String conversion helper functions//
    std::string BoardIOToString(int value);
    std::string PolarityToString(int value);
    std::string DynamicRangeToString(DynamicRange range);
    std::string IntSwitchToString(int value);
    std::string DiscriminatorModeToString(int mode);
    std::string CFDFractionToString(int val);
    std::string TriggerConfigToString(int val);
    std::string PileUpModeToString(int val);
    //Requires both dynamic range and charge sens. value (different ranges = different values)
    std::string ChargeSensToString(int cs_val, DynamicRange range);
    std::string PHAVirtualProbe1ToString(int val);
    std::string PHAVirtualProbe2ToString(int val);
    std::string PHADigitalProbeToString(int val);
    std::string PSDVirtualProbe1ToString(int val);
    std::string PSDVirtualProbe2ToString(int val);
    std::string PSDDigitalProbe1ToString(int val);
    std::string PSDDigitalProbe2ToString(int val);
    std::string SyncModeToString(int val);
    std::string AcqModeToString(int val);
    std::string FirmwareTypeToString(int val);
    std::string SyncMethodToString(SyncMethod method);
    
    //Simple integer power calc
    int Power(int base, int order);

    //Get the time per sample for a type of board (returned in nanoseconds)
    double GetSamplingPeriod(CAEN_DGTZ_BoardModel_t model);

}

#endif