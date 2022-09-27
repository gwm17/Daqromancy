#include "Digitizer.h"
#include "CAENDigitizer.h"

//Mask a specific bit
#define BIT(x) (1<<x)

namespace Daqromancy {

    /////////////////////// Open Function ///////////////////////
    Digitizer::Ref OpenDigitizer(DigitizerArgs& args)
    {
        int code = CAEN_DGTZ_OpenDigitizer(args.type, args.linkNumber, args.conetNode, args.vmeAddress, &args.handle);
        if(code != CAEN_DGTZ_ErrorCode::CAEN_DGTZ_Success)
        {
            //report error
            return nullptr;
        }
        CAEN_DGTZ_BoardInfo_t info;
        CAEN_DGTZ_DPPFirmware_t firmware;
        code |= CAEN_DGTZ_GetInfo(args.handle, &info);
        code |= CAEN_DGTZ_GetDPPFirmwareType(args.handle, &firmware);
        if(code != CAEN_DGTZ_ErrorCode::CAEN_DGTZ_Success)
        {
            //report errors
            return nullptr;
        }

        switch(firmware)
        {
            case CAEN_DGTZ_DPPFirmware_PHA: return std::make_shared<DigitizerPHA>(args, info, code);
            case CAEN_DGTZ_DPPFirmware_PSD: return std::make_shared<DigitizerPSD>(args, info, code);
            case CAEN_DGTZ_DPPFirmware_DAW: return nullptr;
            case CAEN_DGTZ_DPPFirmware_CI: return nullptr;
	        case CAEN_DGTZ_DPPFirmware_ZLE: return nullptr;
	        case CAEN_DGTZ_DPPFirmware_QDC: return nullptr;
        }

        return nullptr;
    }
    /////////////////////// Open Function ///////////////////////


    /////////////////////// DigitizerPHA ///////////////////////
    DigitizerPHA::DigitizerPHA(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec) :
        Digitizer(), m_eventData(nullptr), m_waveData(nullptr)
    {
        Init(args, info, ec);
    }

    DigitizerPHA::~DigitizerPHA() { Close(); }

    void DigitizerPHA::Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec)
    {
        m_args = args;
        m_internalData = info;

        m_args.status = ec;
        if(info.Model == 1730)
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_V1730;
        else if(info.Model == 1725)
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_V1725;
        else if(info.Model == 1740)
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_V1740;
        else
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_DT5720;
        m_args.name = info.ModelName + std::to_string(info.SerialNumber);
        m_args.firmware = CAEN_DGTZ_DPPFirmware_PHA;
        m_args.channels = info.Channels;

        m_channelParams.resize(info.Channels);
        m_eventCountsPerChannel = new uint32_t[info.Channels];
        m_eventData = new CAEN_DGTZ_DPP_PHA_Event_t*[info.Channels];
        m_waveData = new CAEN_DGTZ_DPP_PHA_Waveforms_t*[info.Channels];

        for (int i = 0; i < info.Channels; i++)
        {
            m_scalars.push_back(ScalarDistributor::Bind(m_args.name + std::to_string(i)));
        }

        m_samplingTime = GetSamplingPeriod(m_args.model);

        LoadDigitizerParameters();
        LoadChannelParameters();
        //Must load default parameters here to generate a buffer 
        AllocateMemory(); //More specifically: CAEN memory

        m_isConnected = true;
    }

    void DigitizerPHA::Close()
    {
        StopAquisition(); //Stop aquisition if needed

        DeallocateMemory();

        delete[] m_eventCountsPerChannel;

        m_lowBuffer = nullptr;
        m_eventData = nullptr;
        m_eventCountsPerChannel = nullptr;
        m_waveData = nullptr;

        m_isConnected = false;

        //Explicitly wipe-out scalars
        for (auto& scalar : m_scalars)
        {
            ScalarDistributor::UnBind(scalar->name);
        }
        m_scalars.clear();
    }

    void DigitizerPHA::SetDigitizerParameters(const DigitizerParameters& params)
    {
        if(!m_isConnected || m_isActive)
            return;

        m_digitizerParams = params;
    }

    void DigitizerPHA::InternalSetChannelParameters(const std::vector<PHAParameters>& params)
    {
        if(!m_isConnected || m_isActive)
            return;

        m_channelParams = params;
    }

    void DigitizerPHA::InternalSetWaveformParameters(const PHAWaveParameters& params)
    {
        if(!m_isConnected || m_isActive)
            return;

        m_waveParams = params;
    }

    //This cannot possibly be the correct method for cases where digitizers are chained.
    //July 2022: Is not. See Synchronize.h/.cpp
    void DigitizerPHA::StartAquisition()
    {
        if(!m_isActive && m_isConnected)
        {
            m_args.status |= CAEN_DGTZ_SWStartAcquisition(m_args.handle);
            m_isActive = true;
        }
    }

    //This cannot possibly be the correct method for cases where digitizers are chained.
    //July 2022: Is not. See Synchronize.h/.cpp
    void DigitizerPHA::StopAquisition()
    {
        if(m_isActive && m_isConnected)
        {
            m_args.status |= CAEN_DGTZ_SWStopAcquisition(m_args.handle);
            m_isActive = false;
        }
    }

    void DigitizerPHA::LoadSettings()
    {
        DeallocateMemory();

        LoadDigitizerParameters();
        LoadChannelParameters();
        LoadWaveformParameters();

        AllocateMemory();
    }

    //Set digitizer wide parameters
    void DigitizerPHA::LoadDigitizerParameters()
    {
        m_args.status |= CAEN_DGTZ_SetAcquisitionMode(m_args.handle, m_digitizerParams.acqMode);
        m_args.status |= CAEN_DGTZ_SetDPPAcquisitionMode(m_args.handle, m_digitizerParams.dppAcqMode, m_digitizerParams.dppSaveMode);
        m_args.status |= CAEN_DGTZ_SetRecordLength(m_args.handle, m_digitizerParams.recordLength / m_samplingTime);
        m_args.status |= CAEN_DGTZ_SetIOLevel(m_args.handle, m_digitizerParams.IOlevel);

        m_args.status |= CAEN_DGTZ_SetExtTriggerInputMode(m_args.handle, m_digitizerParams.triggerMode);
        m_args.status |= CAEN_DGTZ_SetDPPEventAggregation(m_args.handle, m_digitizerParams.eventAggr, 0);
        m_args.status |= CAEN_DGTZ_SetRunSynchronizationMode(m_args.handle, m_digitizerParams.syncMode);
        m_args.status |= CAEN_DGTZ_SetChannelEnableMask(m_args.handle, m_digitizerParams.channelMask);
    }

    //Set per channel data
    void DigitizerPHA::LoadChannelParameters()
    {
        m_digitizerParams.channelMask = 0;
        for(std::size_t i=0; i<m_channelParams.size(); i++)
        {
            m_args.status |= CAEN_DGTZ_SetChannelDCOffset(m_args.handle, i, uint32_t(0xffff * m_channelParams[i].dcOffset)); //Max range is 0xffff
            m_args.status |= CAEN_DGTZ_SetDPPPreTriggerSize(m_args.handle, i, m_channelParams[i].preTriggerTime / m_samplingTime);
            m_args.status |= CAEN_DGTZ_SetChannelPulsePolarity(m_args.handle, i, m_channelParams[i].pulsePolarity);
            m_args.status |= CAEN_DGTZ_WriteRegister(m_args.handle, 0x1028 +  (i<<8), m_channelParams[i].dynamicRange);
            
            if(m_channelParams[i].isEnabled)
            {
                m_digitizerParams.channelMask |= (BIT(i)); //flip channel bit to 1 scince its enabled
            }
            
            //Write data to garbage CAEN style structs

            m_caenParams.M[i] = m_channelParams[i].decayTimeConst / m_samplingTime;
            m_caenParams.m[i] = m_channelParams[i].trapFlatTop / m_samplingTime;
            m_caenParams.k[i] = m_channelParams[i].trapRiseTime / m_samplingTime;
            m_caenParams.ftd[i] = m_channelParams[i].flatTopDelay / m_samplingTime;
            m_caenParams.a[i] = m_channelParams[i].triggerFilterSmoothing;
            m_caenParams.b[i] = m_channelParams[i].inputRiseTime / m_samplingTime;
            m_caenParams.thr[i] = m_channelParams[i].triggerThreshold;
            m_caenParams.nsbl[i] = m_channelParams[i].samplesBaseLineMean;
            m_caenParams.nspk[i] = m_channelParams[i].samplesPeakMean;
            m_caenParams.pkho[i] = m_channelParams[i].peakHoldOff / m_samplingTime;
            m_caenParams.blho[i] = m_channelParams[i].baseLineHoldOff / m_samplingTime;
            m_caenParams.trgho[i] = m_channelParams[i].triggerHoldOff / m_samplingTime;
            m_caenParams.twwdt[i] = m_channelParams[i].riseTimeValidationWindow / m_samplingTime;
            m_caenParams.trgwin[i] = m_channelParams[i].riseTimeDiscrimination / m_samplingTime;
            m_caenParams.dgain[i] = m_channelParams[i].digitalProbeGain;
            m_caenParams.enf[i] = m_channelParams[i].energyNormalizationFactor;
            m_caenParams.decimation[i] = m_channelParams[i].inputDecimation;
            //So far as I can tell these are not used
	        m_caenParams.enskim[i] = 0;
	        m_caenParams.eskimlld[i] = 0;
            m_caenParams.eskimuld[i] = 0;
	        m_caenParams.blrclip[i] = 0;
            m_caenParams.dcomp[i] = 0;
            m_caenParams.trapbsl[i] = 0;
            m_caenParams.otrej[i] = 0;
        }

        /*
            Something not immediately clear... there is no GetDPPParameters function
            I assume that if a value is passed that is invalid (which we know can happen) the value that is used by 
            the digitizer is written to the data at the pointer passed to the SetDPPParameters function...
            I proceed under this assumption
        */
        m_args.status |= CAEN_DGTZ_SetDPPParameters(m_args.handle, m_digitizerParams.channelMask, &m_caenParams);

        //Retrieve corrected value for the channel parameters
        for (std::size_t i = 0; i < m_channelParams.size(); i++)
        {
            if (!m_channelParams[i].isEnabled)
            {
                continue;
            }
            m_channelParams[i].decayTimeConst = m_caenParams.M[i] * m_samplingTime;
            m_channelParams[i].trapFlatTop = m_caenParams.m[i] * m_samplingTime;
            m_channelParams[i].trapRiseTime = m_caenParams.k[i] * m_samplingTime;
            m_channelParams[i].flatTopDelay = m_caenParams.ftd[i] * m_samplingTime;
            m_channelParams[i].triggerFilterSmoothing = m_caenParams.a[i];
            m_channelParams[i].inputRiseTime = m_caenParams.b[i] * m_samplingTime;
            m_channelParams[i].triggerThreshold = m_caenParams.thr[i];
            m_channelParams[i].samplesBaseLineMean = m_caenParams.nsbl[i];
            m_channelParams[i].samplesPeakMean = m_caenParams.nspk[i];
            m_channelParams[i].peakHoldOff = m_caenParams.pkho[i] * m_samplingTime;
            m_channelParams[i].baseLineHoldOff = m_caenParams.blho[i] * m_samplingTime;
            m_channelParams[i].triggerHoldOff = m_caenParams.trgho[i] * m_samplingTime;
            m_channelParams[i].riseTimeValidationWindow = m_caenParams.twwdt[i] * m_samplingTime;
            m_channelParams[i].riseTimeDiscrimination = m_caenParams.trgwin[i] * m_samplingTime;
            m_channelParams[i].digitalProbeGain = m_caenParams.dgain[i];
            m_channelParams[i].energyNormalizationFactor = m_caenParams.enf[i];
            m_channelParams[i].inputDecimation = m_caenParams.decimation[i];
        }
    }

    void DigitizerPHA::LoadWaveformParameters()
    {
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, ANALOG_TRACE_1, m_waveParams.analogProbe1);
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, ANALOG_TRACE_2, m_waveParams.analogProbe2);
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, DIGITAL_TRACE_1, m_waveParams.digitalProbe1);
    }

    //Note: Even though function signatures of MallocDPPEvents and MallocDPPWaveforms are identical
    //they behave quite differently. MallocDPPEvents allocates the entire channel-buffer matrix in one call,
    //while MallocDPPWaveforms allocates only a single buffer for a single channel at a time.
    //This is why void* are dangerous!
    void DigitizerPHA::AllocateMemory()
    {
        m_args.status |= CAEN_DGTZ_MallocReadoutBuffer(m_args.handle, &m_lowBuffer, &m_lowBufferSize);
        //void casts are soooo bad .... but required by CAEN API
        m_args.status |= CAEN_DGTZ_MallocDPPEvents(m_args.handle, (void**)(m_eventData), &m_eventBufferSize); 
        for(int channel=0; channel<m_internalData.Channels; channel++)
            m_args.status |= CAEN_DGTZ_MallocDPPWaveforms(m_args.handle, (void**)(&m_waveData[channel]), &m_waveBufferSize);
    }

    void DigitizerPHA::DeallocateMemory()
    {
        m_args.status |= CAEN_DGTZ_FreeReadoutBuffer(&m_lowBuffer);
        m_args.status |= CAEN_DGTZ_FreeDPPEvents(m_args.handle, (void**)(m_eventData));
        for(int i=0; i<m_internalData.Channels; i++)
            m_args.status |= CAEN_DGTZ_FreeDPPWaveforms(m_args.handle, (void*)(m_waveData[i]));
    }

    void DigitizerPHA::ReadData(std::vector<DYData>& buffer)
    {
        if(!m_isActive || !m_isConnected)
            return;

        m_args.status |= CAEN_DGTZ_ReadData(m_args.handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_lowBuffer, &m_lowBufferSize);
        if (m_lowBufferSize == 0)
        {
            return;
        }

        m_args.status |= CAEN_DGTZ_GetDPPEvents(m_args.handle, m_lowBuffer, m_lowBufferSize, (void**)(&m_eventData), m_eventCountsPerChannel);
        size_t waveSize;
        DYData tempData;
        tempData.board = m_args.handle;
        for(int i=0; i<m_internalData.Channels; i++)
        {
            tempData.channel = i;
            //Increment scalars
            m_scalars[i]->value += m_eventCountsPerChannel[i];
            for(int j=0; j<m_eventCountsPerChannel[i]; j++)
            {
                tempData.energy = m_eventData[i][j].Energy;
                tempData.timestamp = m_eventData[i][j].TimeTag;
                tempData.flags = m_eventData[i][j].Extras;

                if(m_digitizerParams.dppAcqMode != CAEN_DGTZ_DPP_ACQ_MODE_List)
                {
                    CAEN_DGTZ_DecodeDPPWaveforms(m_args.handle, (void*)&(m_eventData[i][j]), m_waveData);
                    tempData.waveSize = m_waveData[i]->Ns;
                    waveSize = m_waveData[i]->Ns;
                    if(waveSize != 0)
                    {
                        //Copy the data to our vectors PHA supports 2 analog traces and 2 digital traces
                        tempData.trace1Samples.assign(m_waveData[i]->Trace1, m_waveData[i]->Trace1 + waveSize);
                        tempData.trace2Samples.assign(m_waveData[i]->Trace2, m_waveData[i]->Trace2 + waveSize); //This is all zero if in single analog trace mode
                        tempData.digitalTrace1Samples.assign(m_waveData[i]->DTrace1, m_waveData[i]->DTrace1 + waveSize);
                        tempData.digitalTrace2Samples.assign(m_waveData[i]->DTrace2, m_waveData[i]->DTrace2 + waveSize);
                    }
                }
                buffer.push_back(tempData);
            }
        }
    }
    /////////////////////// DigitizerPHA ///////////////////////

    /////////////////////// DigitizerPSD ///////////////////////
    DigitizerPSD::DigitizerPSD(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int code) :
        Digitizer(), m_eventData(nullptr), m_waveData(nullptr)
    {
        Init(args, info, code);
    }

    DigitizerPSD::~DigitizerPSD() { Close(); }

    void DigitizerPSD::Init(const DigitizerArgs& args, const CAEN_DGTZ_BoardInfo_t& info, int ec)
    {
        m_args = args;
        m_internalData = info;

        m_args.status = ec;
        if(info.Model == 1730)
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_V1730;
        else if(info.Model == 1725)
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_V1725;
        else if(info.Model == 1740)
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_V1740;
        else
            m_args.model = CAEN_DGTZ_BoardModel_t::CAEN_DGTZ_DT5720;
        m_args.name = info.ModelName + std::to_string(info.SerialNumber);
        m_args.firmware = CAEN_DGTZ_DPPFirmware_PSD;
        m_args.channels = info.Channels;

        m_channelParams.resize(info.Channels);
        m_eventCountsPerChannel = new uint32_t[info.Channels];
        m_eventData = new CAEN_DGTZ_DPP_PSD_Event_t*[info.Channels];
        m_waveData = new CAEN_DGTZ_DPP_PSD_Waveforms_t*[info.Channels];
        LoadDigitizerParameters();
        LoadChannelParameters();
        //Must load default parameters here to generate a buffer 
        AllocateMemory(); //More specifically: CAEN memory

        for (int i = 0; i < info.Channels; i++)
        {
            m_scalars.push_back(ScalarDistributor::Bind(m_args.name + std::to_string(i)));
        }

        m_samplingTime = GetSamplingPeriod(m_args.model);

        m_isConnected = true;
    }

    void DigitizerPSD::Close()
    {
        StopAquisition(); //Stop aquisition if needed

        DeallocateMemory();

        delete[] m_eventCountsPerChannel;

        m_lowBuffer = nullptr;
        m_eventData = nullptr;
        m_eventCountsPerChannel = nullptr;
        m_waveData = nullptr;

        m_isConnected = false;

        //Explicitly wipe-out scalars
        for (auto& scalar : m_scalars)
        {
            ScalarDistributor::UnBind(scalar->name);
        }
        m_scalars.clear();
    }

    void DigitizerPSD::SetDigitizerParameters(const DigitizerParameters& params)
    {
        if(!m_isConnected || m_isActive)
            return;

        m_digitizerParams = params;
    }

    void DigitizerPSD::InternalSetChannelParameters(const std::vector<PSDParameters>& params)
    {
        if(!m_isConnected || m_isActive)
            return;

        m_channelParams = params;
    }

    void DigitizerPSD::InternalSetWaveformParameters(const PSDWaveParameters& params)
    {
        if(!m_isConnected || m_isActive)
            return;

        m_waveParams = params;
    }

    //This cannot possibly be the correct method for cases where digitizers are chained.
    //July 2022: Is not. See Synchronize.h/.cpp
    void DigitizerPSD::StartAquisition()
    {
        if(!m_isActive && m_isConnected)
        {
            m_args.status |= CAEN_DGTZ_SWStartAcquisition(m_args.handle);
            m_isActive = true;
        }
    }

    //This cannot possibly be the correct method for cases where digitizers are chained.
    //July 2022: Is not. See Synchronize.h/.cpp
    void DigitizerPSD::StopAquisition()
    {
        if(m_isActive && m_isConnected)
        {
            m_args.status |= CAEN_DGTZ_SWStopAcquisition(m_args.handle);
            m_isActive = false;
        }
    }

    void DigitizerPSD::LoadSettings()
    {
        DeallocateMemory();

        LoadDigitizerParameters();
        LoadChannelParameters();
        LoadWaveformParameters();

        AllocateMemory();
    }

    //Set digitizer wide parameters
    void DigitizerPSD::LoadDigitizerParameters()
    {
        m_args.status |= CAEN_DGTZ_SetAcquisitionMode(m_args.handle, m_digitizerParams.acqMode);
        m_args.status |= CAEN_DGTZ_SetDPPAcquisitionMode(m_args.handle, m_digitizerParams.dppAcqMode, m_digitizerParams.dppSaveMode); //why would you ever not want one of these?
        m_args.status |= CAEN_DGTZ_SetRecordLength(m_args.handle, m_digitizerParams.recordLength / m_samplingTime);
        m_args.status |= CAEN_DGTZ_SetIOLevel(m_args.handle, m_digitizerParams.IOlevel);

        m_args.status |= CAEN_DGTZ_SetExtTriggerInputMode(m_args.handle, m_digitizerParams.triggerMode);
        m_args.status |= CAEN_DGTZ_SetDPPEventAggregation(m_args.handle, m_digitizerParams.eventAggr, 0);
        m_args.status |= CAEN_DGTZ_SetRunSynchronizationMode(m_args.handle, m_digitizerParams.syncMode);
        m_args.status |= CAEN_DGTZ_SetChannelEnableMask(m_args.handle, m_digitizerParams.channelMask);
    }

    //Set per channel data
    void DigitizerPSD::LoadChannelParameters()
    {
        m_digitizerParams.channelMask = 0;
        for(std::size_t i=0; i<m_channelParams.size(); i++)
        {
            m_args.status |= CAEN_DGTZ_SetChannelDCOffset(m_args.handle, i, uint32_t(0xffff * m_channelParams[i].dcOffset)); //Max range is 0xffff
            m_args.status |= CAEN_DGTZ_SetDPPPreTriggerSize(m_args.handle, i, m_channelParams[i].preTriggerTime / m_samplingTime);
            m_args.status |= CAEN_DGTZ_SetChannelPulsePolarity(m_args.handle, i, m_channelParams[i].pulsePolarity);
            m_args.status |= CAEN_DGTZ_WriteRegister(m_args.handle, 0x1028 +  (i<<8), m_channelParams[i].dynamicRange);
            
            if(m_channelParams[i].isEnabled)
            {
                m_digitizerParams.channelMask |= (BIT(i)); //flip channel bit to 1 scince its enabled
            }
            
            //Write data to garbage CAEN style structs
            m_caenParams.thr[i] = m_channelParams[i].triggerThreshold;
            m_caenParams.selft[i] = m_channelParams[i].selfTrigger;
            m_caenParams.csens[i] = m_channelParams[i].chargeSensitivity;
            m_caenParams.sgate[i] = m_channelParams[i].shortGate / m_samplingTime;
            m_caenParams.lgate[i] = m_channelParams[i].longGate / m_samplingTime;
            m_caenParams.pgate[i] = m_channelParams[i].preGate / m_samplingTime;
            m_caenParams.tvaw[i] = m_channelParams[i].triggerValidationWindow / m_samplingTime;
            m_caenParams.nsbl[i] = m_channelParams[i].samplesBasline;
            m_caenParams.discr[i] = m_channelParams[i].discrminatorType;
            m_caenParams.cfdf[i] = m_channelParams[i].cfdFraction;
            m_caenParams.cfdd[i] = m_channelParams[i].cfdDelay / m_samplingTime;
            m_caenParams.trgc[i] = (CAEN_DGTZ_DPP_TriggerConfig_t)1; // Deprecated, must be set to one according to docs
        }
        //These are like global digitizer params but PSD specific. Here we treat first channel as "global" setting (similar to CoMPASS)
        

        /*
            Something not immediately clear... there is no GetDPPParameters function
            I assume that if a value is passed that is invalid (which we know can happen) the value that is used by
            the digitizer is written to the data at the pointer passed to the SetDPPParameters function...
            I proceed under this assumption
        */
        m_args.status |= CAEN_DGTZ_SetDPPParameters(m_args.handle, m_digitizerParams.channelMask, &m_caenParams);

        //Retrieve corrected value for the channel parameters
        for (std::size_t i = 0; i < m_channelParams.size(); i++)
        {
            if (!m_channelParams[i].isEnabled)
            {
                continue;
            }

            m_channelParams[i].triggerThreshold = m_caenParams.thr[i];
            m_channelParams[i].selfTrigger = m_caenParams.selft[i];
            m_channelParams[i].chargeSensitivity = m_caenParams.csens[i];
            m_channelParams[i].shortGate = m_caenParams.sgate[i] * m_samplingTime;
            m_channelParams[i].longGate = m_caenParams.lgate[i] * m_samplingTime;
            m_channelParams[i].preGate = m_caenParams.pgate[i] * m_samplingTime;
            m_channelParams[i].triggerValidationWindow = m_caenParams.tvaw[i] * m_samplingTime;
            m_channelParams[i].samplesBasline = m_caenParams.nsbl[i];
            m_channelParams[i].discrminatorType = m_caenParams.discr[i];
            m_channelParams[i].cfdFraction = m_caenParams.cfdf[i];
            m_channelParams[i].cfdDelay = m_caenParams.cfdd[i] * m_samplingTime;
        }
        m_channelParams[0].baselineThreshold = m_caenParams.blthr;
        m_channelParams[0].bltmo = m_caenParams.bltmo;
        m_channelParams[0].triggerHoldOff = m_caenParams.trgho;
        m_channelParams[0].pileUpRejection = m_caenParams.purh;
        m_channelParams[0].purgap = m_caenParams.purgap;
    }

    void DigitizerPSD::LoadWaveformParameters()
    {
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, ANALOG_TRACE_1, m_waveParams.analogProbe1);
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, ANALOG_TRACE_2, m_waveParams.analogProbe2);
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, DIGITAL_TRACE_1, m_waveParams.digitalProbe1);
        m_args.status |= CAEN_DGTZ_SetDPP_VirtualProbe(m_args.handle, DIGITAL_TRACE_2, m_waveParams.digitalProbe2);
    }

    //Note: Even though function signatures of MallocDPPEvents and MallocDPPWaveforms are identical
    //they behave quite differently. MallocDPPEvents allocates the entire channel-buffer matrix in one call,
    //while MallocDPPWaveforms allocates only a single buffer for a single channel at a time.
    //This is why void* are dangerous!
    void DigitizerPSD::AllocateMemory()
    {
        m_args.status |= CAEN_DGTZ_MallocReadoutBuffer(m_args.handle, &m_lowBuffer, &m_lowBufferSize);
        //void casts are soooo bad .... but required by CAEN API
        m_args.status |= CAEN_DGTZ_MallocDPPEvents(m_args.handle, (void**)(m_eventData), &m_eventBufferSize); 
        for(int channel=0; channel<m_internalData.Channels; channel++)
            m_args.status |= CAEN_DGTZ_MallocDPPWaveforms(m_args.handle, (void**)(&m_waveData[channel]), &m_waveBufferSize);
    }

    void DigitizerPSD::DeallocateMemory()
    {
        m_args.status |= CAEN_DGTZ_FreeReadoutBuffer(&m_lowBuffer);
        m_args.status |= CAEN_DGTZ_FreeDPPEvents(m_args.handle, (void**)(m_eventData));
        for(int i=0; i<m_internalData.Channels; i++)
            m_args.status |= CAEN_DGTZ_FreeDPPWaveforms(m_args.handle, (void*)(m_waveData[i]));
    }

    void DigitizerPSD::ReadData(std::vector<DYData>& buffer)
    {
        if (!m_isActive || !m_isConnected)
            return;

        m_args.status |= CAEN_DGTZ_ReadData(m_args.handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_lowBuffer, &m_lowBufferSize);
        if (m_lowBufferSize == 0)
        {
            return;
        }

        m_args.status |= CAEN_DGTZ_GetDPPEvents(m_args.handle, m_lowBuffer, m_lowBufferSize, (void**)(&m_eventData), m_eventCountsPerChannel);
        size_t waveSize;
        DYData tempData;
        tempData.board = m_args.handle;
        for(int i=0; i<m_internalData.Channels; i++)
        {
            tempData.channel = i;
            //Increment scalars
            m_scalars[i]->value += m_eventCountsPerChannel[i];
            for(int j=0; j<m_eventCountsPerChannel[i]; j++)
            {
                tempData.energy = m_eventData[i][j].ChargeLong;
                tempData.energyShort = m_eventData[i][j].ChargeShort;
                tempData.timestamp = m_eventData[i][j].TimeTag;
                tempData.flags = m_eventData[i][j].Extras;

                if(m_digitizerParams.dppAcqMode != CAEN_DGTZ_DPP_ACQ_MODE_List)
                {
                    CAEN_DGTZ_DecodeDPPWaveforms(m_args.handle, (void*)&(m_eventData[i][j]), m_waveData);
                    tempData.waveSize = m_waveData[i]->Ns;
                    waveSize = m_waveData[i]->Ns;
                    if(tempData.waveSize != 0)
                    {
                        //Copy the data to our vectors PHA supports 2 analog traces and 2 digital traces
                        tempData.trace1Samples.assign(m_waveData[i]->Trace1, m_waveData[i]->Trace1 + waveSize);
                        tempData.trace2Samples.assign(m_waveData[i]->Trace2, m_waveData[i]->Trace2 + waveSize); //This is all zero if in single analog trace mode
                        tempData.digitalTrace1Samples.assign(m_waveData[i]->DTrace1, m_waveData[i]->DTrace1 + waveSize);
                        tempData.digitalTrace2Samples.assign(m_waveData[i]->DTrace2, m_waveData[i]->DTrace2 + waveSize);
                    }
                }
                buffer.push_back(tempData);
            }
        }
    }
}