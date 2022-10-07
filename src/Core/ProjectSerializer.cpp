#include "ProjectSerializer.h"

#include <fstream>
#include "yaml-cpp/yaml.h"


namespace YAML {

	template<>
	struct convert<Daqromancy::DigitizerArgs>
	{
		static Node encode(const Daqromancy::DigitizerArgs& args)
		{
			Node node(NodeType::Map);
			node.force_insert<std::string, int>("ConnectionType", args.type);
			node.force_insert<std::string, int>("LinkNumber", args.linkNumber);
			node.force_insert<std::string, int>("CONETNode", args.conetNode);
			node.force_insert<std::string, uint32_t>("VMEAddress", args.vmeAddress);
			node.force_insert<std::string, int>("Handle", args.handle);
			node.force_insert<std::string, int>("Model", args.model);
			node.force_insert<std::string, int>("Firmware", args.firmware);
			node.force_insert<std::string, int>("Channels", args.channels);

			return node;
		}

		static bool decode(const Node& node, Daqromancy::DigitizerArgs& args)
		{
			if (!node.IsMap())
				return false;

			args.type = (CAEN_DGTZ_ConnectionType) node["ConnectionType"].as<int>();
			args.linkNumber = node["LinkNumber"].as<int>();
			args.conetNode = node["CONETNode"].as<int>();
			args.vmeAddress = node["VMEAddress"].as<uint32_t>();
			args.handle = node["Handle"].as<int>();
			args.model = (CAEN_DGTZ_BoardModel_t) node["Model"].as<int>();
			args.firmware = (CAEN_DGTZ_DPPFirmware_t) node["Firmware"].as<int>();
			args.channels = node["Channels"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<Daqromancy::DigitizerParameters>
	{
		static Node encode(const Daqromancy::DigitizerParameters& params)
		{
			Node node(NodeType::Map);
			node.force_insert<std::string, int>("RecordLength", params.recordLength);
			node.force_insert<std::string, uint32_t>("ChannelMask", params.channelMask);
			node.force_insert<std::string, int>("EventAggr", params.eventAggr);
			node.force_insert<std::string, int>("AcqMode", params.acqMode);
			node.force_insert<std::string, int>("DPPAcqMode", params.dppAcqMode);
			node.force_insert<std::string, int>("DPPSaveMode", params.dppSaveMode);
			node.force_insert<std::string, int>("IOLevel", params.IOlevel);
			node.force_insert<std::string, int>("TriggerMode", params.triggerMode);
			node.force_insert<std::string, int>("SyncMode", params.syncMode);

			return node;
		}

		static bool decode(const Node& node, Daqromancy::DigitizerParameters& params)
		{
			if (!node.IsMap())
				return false;

			params.recordLength = node["RecordLength"].as<uint32_t>();
			params.channelMask = node["ChannelMask"].as<uint32_t>();
			params.eventAggr = node["EventAggr"].as<int>();
			params.acqMode = (CAEN_DGTZ_AcqMode_t)node["AcqMode"].as<int>();
			params.dppAcqMode = (CAEN_DGTZ_DPP_AcqMode_t)node["DPPAcqMode"].as<int>();
			params.dppSaveMode = (CAEN_DGTZ_DPP_SaveParam_t)node["DPPSaveMode"].as<int>();
			params.IOlevel = (CAEN_DGTZ_IOLevel_t)node["IOLevel"].as<int>();
			params.triggerMode = (CAEN_DGTZ_TriggerMode_t)node["TriggerMode"].as<int>();
			params.syncMode = (CAEN_DGTZ_RunSyncMode_t)node["SyncMode"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<Daqromancy::PHAParameters>
	{
		static Node encode(const Daqromancy::PHAParameters& params)
		{
			Node node(NodeType::Map);
			node.force_insert<std::string, bool>("IsEnabled", params.isEnabled);
			node.force_insert<std::string, uint32_t>("PreTrigger", params.preTriggerTime);
			node.force_insert<std::string, float>("DCOffset", params.dcOffset);
			node.force_insert<std::string, int>("PulsePolarity", params.pulsePolarity);
			node.force_insert<std::string, int>("DynamicRange", params.dynamicRange);
			node.force_insert<std::string, int>("DecayTimeConst", params.decayTimeConst);
			node.force_insert<std::string, int>("TrapFlatTop", params.trapFlatTop);
			node.force_insert<std::string, int>("TrapRiseTime", params.trapRiseTime);
			node.force_insert<std::string, int>("FlatTopDelay",  params.flatTopDelay);
			node.force_insert<std::string, int>("TriggerSmoothing", params.triggerFilterSmoothing);
			node.force_insert<std::string, int>("InputRiseTime", params.inputRiseTime);
			node.force_insert<std::string, int>("TriggerThreshold", params.triggerThreshold);
			node.force_insert<std::string, int>("SamplesBaseline", params.samplesBaseLineMean);
			node.force_insert<std::string, int>("SamplesPeak", params.samplesPeakMean);
			node.force_insert<std::string, int>("PeakHoldoff", params.peakHoldOff);
			node.force_insert<std::string, int>("BaselineHoldoff", params.baseLineHoldOff);
			node.force_insert<std::string, int>("TriggerHoldoff", params.triggerHoldOff);
			node.force_insert<std::string, int>("RiseTimeWindow", params.riseTimeValidationWindow);
			node.force_insert<std::string, int>("RiseTimeDiscrimination", params.riseTimeDiscrimination);
			node.force_insert<std::string, int>("DigitalProbeGain", params.digitalProbeGain);
			node.force_insert<std::string, int>("EnergyNormFactor", params.energyNormalizationFactor);
			node.force_insert<std::string, int>("InputDecimation", params.inputDecimation);

			return node;
		}

		static bool decode(const Node& node, Daqromancy::PHAParameters& params)
		{
			if (!node.IsMap())
				return false;

			params.isEnabled = node["IsEnabled"].as<bool>();
			params.preTriggerTime = node["PreTrigger"].as<uint32_t>();
			params.dcOffset = node["DCOffset"].as<float>();
			params.pulsePolarity = (CAEN_DGTZ_PulsePolarity_t)node["PulsePolarity"].as<int>();
			params.dynamicRange = (Daqromancy::DynamicRange)node["DynamicRange"].as<int>();
			params.decayTimeConst = node["DecayTimeConst"].as<int>();
			params.trapFlatTop = node["TrapFlatTop"].as<int>();
			params.trapRiseTime = node["TrapRiseTime"].as<int>();
			params.flatTopDelay = node["FlatTopDelay"].as<int>();
			params.triggerFilterSmoothing = node["TriggerSmoothing"].as<int>();
			params.inputRiseTime = node["InputRiseTime"].as<int>();
			params.triggerThreshold = node["TriggerThreshold"].as<int>();
			params.samplesBaseLineMean = node["SamplesBaseline"].as<int>();
			params.samplesPeakMean = node["SamplesPeak"].as<int>();
			params.peakHoldOff = node["PeakHoldoff"].as<int>();
			params.baseLineHoldOff = node["BaselineHoldoff"].as<int>();
			params.triggerHoldOff = node["TriggerHoldoff"].as<int>();
			params.riseTimeValidationWindow = node["RiseTimeWindow"].as<int>();
			params.riseTimeDiscrimination = node["RiseTimeDiscrimination"].as<int>();
			params.digitalProbeGain = node["DigitalProbeGain"].as<int>();
			params.energyNormalizationFactor = node["EnergyNormFactor"].as<int>();
			params.inputDecimation = node["InputDecimation"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<Daqromancy::PSDParameters>
	{
		static Node encode(const Daqromancy::PSDParameters& params)
		{
			Node node(NodeType::Map);

			node.force_insert<std::string, bool>("IsEnabled", params.isEnabled);
			node.force_insert<std::string, uint32_t>("PreTrigger", params.preTriggerTime);
			node.force_insert<std::string, float>("DCOffset", params.dcOffset);
			node.force_insert<std::string, int>("PulsePolarity", params.pulsePolarity);
			node.force_insert<std::string, int>("DynamicRange", params.dynamicRange);
			node.force_insert<std::string, int>("BaselineThreshold", params.baselineThreshold);
			node.force_insert<std::string, int>("TriggerHoldoff", params.triggerHoldOff);
			node.force_insert<std::string, int>("TriggerThreshold", params.triggerThreshold);
			node.force_insert<std::string, int>("SelfTrigger", params.selfTrigger);
			node.force_insert<std::string, int>("ChargeSensitivity", params.chargeSensitivity);
			node.force_insert<std::string, int>("ShortGate", params.shortGate);
			node.force_insert<std::string, int>("LongGate", params.longGate);
			node.force_insert<std::string, int>("PreGate", params.preGate);
			node.force_insert<std::string, int>("TriggerWindow", params.triggerValidationWindow);
			node.force_insert<std::string, int>("BaselineSamples", params.samplesBasline);
			node.force_insert<std::string, int>("DiscriminatorType", params.discrminatorType);
			node.force_insert<std::string, int>("CFDFraction", params.cfdFraction);
			node.force_insert<std::string, int>("CFDDelay", params.cfdDelay);
			node.force_insert<std::string, int>("TriggerConfig", params.triggerConfig);
			node.force_insert<std::string, int>("PileUpRejection", params.pileUpRejection);
			node.force_insert<std::string, int>("PURGap", params.purgap);

			return node;
		}

		static bool decode(const Node& node, Daqromancy::PSDParameters& params)
		{
			if (!node.IsMap())
				return false;

			params.isEnabled = node["IsEnabled"].as<bool>();
			params.preTriggerTime = node["PreTrigger"].as<int>();
			params.dcOffset = node["DCOffset"].as<float>();
			params.pulsePolarity = (CAEN_DGTZ_PulsePolarity_t)node["PulsePolarity"].as<int>();
			params.dynamicRange = (Daqromancy::DynamicRange)node["DynamicRange"].as<int>();
			params.baselineThreshold = node["BaselineThreshold"].as<int>();
			params.triggerHoldOff = node["TriggerHoldoff"].as<int>();
			params.triggerThreshold = node["TriggerThreshold"].as<int>();
			params.selfTrigger = node["SelfTrigger"].as<int>();
			params.chargeSensitivity = node["ChargeSensitivity"].as<int>();
			params.shortGate = node["ShortGate"].as<int>();
			params.longGate = node["LongGate"].as<int>();
			params.preGate = node["PreGate"].as<int>();
			params.triggerValidationWindow = node["TriggerWindow"].as<int>();
			params.samplesBasline = node["BaselineSamples"].as<int>();
			params.discrminatorType = node["DiscriminatorType"].as<int>();
			params.cfdFraction = node["CFDFraction"].as<int>();
			params.cfdDelay = node["CFDDelay"].as<int>();
			params.triggerConfig = (CAEN_DGTZ_DPP_TriggerConfig_t)node["TriggerConfig"].as<int>();
			params.pileUpRejection = (CAEN_DGTZ_DPP_PUR_t)node["PileUpRejection"].as<int>();
			params.purgap = node["PURGap"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<Daqromancy::PHAWaveParameters>
	{
		static Node encode(const Daqromancy::PHAWaveParameters& waves)
		{
			Node node(NodeType::Map);

			node.force_insert<std::string, int>("IsDual", waves.isDual);
			node.force_insert<std::string, int>("AnalogProbe1", waves.analogProbe1);
			node.force_insert<std::string, int>("AnalogProbe2", waves.analogProbe2);
			node.force_insert<std::string, int>("DigitalProbe1", waves.digitalProbe1);

			return node;
		}

		static bool decode(const Node& node, Daqromancy::PHAWaveParameters& waves)
		{
			if (!node.IsMap())
				return false;

			waves.isDual = (CAEN_DGTZ_DPP_VirtualProbe_t)node["IsDual"].as<int>();
			waves.analogProbe1 = (Daqromancy::PHAVirtualProbe1Options)node["AnalogProbe1"].as<int>();
			waves.analogProbe2 = (Daqromancy::PHAVirtualProbe2Options)node["AnalogProbe2"].as<int>();
			waves.digitalProbe1 = (Daqromancy::PHADigitalProbe1Options)node["DigitalProbe1"].as<int>();

			return true;
		}
	};

	template<>
	struct convert<Daqromancy::PSDWaveParameters>
	{
		static Node encode(const Daqromancy::PSDWaveParameters& waves)
		{
			Node node(NodeType::Map);

			node.force_insert<std::string, int>("IsDual", waves.isDual);
			node.force_insert<std::string, int>("AnalogProbe1", waves.analogProbe1);
			node.force_insert<std::string, int>("AnalogProbe2", waves.analogProbe2);
			node.force_insert<std::string, int>("DigitalProbe1", waves.digitalProbe1);
			node.force_insert<std::string, int>("DigitalProbe2", waves.digitalProbe2);

			return node;
		}

		static bool decode(const Node& node, Daqromancy::PSDWaveParameters& waves)
		{
			if (!node.IsMap())
				return false;

			waves.isDual = (CAEN_DGTZ_DPP_VirtualProbe_t)node["IsDual"].as<int>();
			waves.analogProbe1 = (Daqromancy::PSDVirtualProbe1Options)node["AnalogProbe1"].as<int>();
			waves.analogProbe2 = (Daqromancy::PSDVirtualProbe2Options)node["AnalogProbe2"].as<int>();
			waves.digitalProbe1 = (Daqromancy::PSDDigitalProbe1Options)node["DigitalProbe1"].as<int>();
			waves.digitalProbe2 = (Daqromancy::PSDDigitalProbe2Options)node["DigitalProbe2"].as<int>();

			return true;
		}
	};
	
	Emitter& operator<<(Emitter& stream, const Daqromancy::DigitizerArgs& args)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "ConnectionType" << YAML::Value << args.type;
		stream << YAML::Key << "LinkNumber" << YAML::Value << args.linkNumber;
		stream << YAML::Key << "CONETNode" << YAML::Value << args.conetNode;
		stream << YAML::Key << "VMEAddress" << YAML::Value << args.vmeAddress;
		stream << YAML::Key << "Handle" << YAML::Value << args.handle;
		stream << YAML::Key << "Model" << YAML::Value << args.model;
		stream << YAML::Key << "Firmware" << YAML::Value << args.firmware;
		stream << YAML::Key << "Channels" << YAML::Value << args.channels;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const Daqromancy::DigitizerParameters& params)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "RecordLength" << YAML::Value << params.recordLength;
		stream << YAML::Key << "ChannelMask" << YAML::Value << params.channelMask;
		stream << YAML::Key << "EventAggr" << YAML::Value << params.eventAggr;
		stream << YAML::Key << "AcqMode" << YAML::Value << params.acqMode;
		stream << YAML::Key << "DPPAcqMode" << YAML::Value << params.dppAcqMode;
		stream << YAML::Key << "DPPSaveMode" << YAML::Value << params.dppSaveMode;
		stream << YAML::Key << "IOLevel" << YAML::Value << params.IOlevel;
		stream << YAML::Key << "TriggerMode" << YAML::Value << params.triggerMode;
		stream << YAML::Key << "SyncMode" << YAML::Value << params.syncMode;
		stream << YAML::EndMap;
		
		return stream;
	}

	Emitter& operator<<(Emitter& stream, const Daqromancy::PHAParameters& params)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsEnabled" << YAML::Value << params.isEnabled;
		stream << YAML::Key << "PreTrigger" << YAML::Value << params.preTriggerTime;
		stream << YAML::Key << "DCOffset" << YAML::Value << params.dcOffset;
		stream << YAML::Key << "PulsePolarity" << YAML::Value << params.pulsePolarity;
		stream << YAML::Key << "DynamicRange" << YAML::Value << params.dynamicRange;
		stream << YAML::Key << "DecayTimeConst" << YAML::Value << params.decayTimeConst;
		stream << YAML::Key << "TrapFlatTop" << YAML::Value << params.trapFlatTop;
		stream << YAML::Key << "TrapRiseTime" << YAML::Value << params.trapRiseTime;
		stream << YAML::Key << "FlatTopDelay" << YAML::Value << params.flatTopDelay;
		stream << YAML::Key << "TriggerSmoothing" << YAML::Value << params.triggerFilterSmoothing;
		stream << YAML::Key << "InputRiseTime" << YAML::Value << params.inputRiseTime;
		stream << YAML::Key << "TriggerThreshold" << YAML::Value << params.triggerThreshold;
		stream << YAML::Key << "SamplesBaseline" << YAML::Value << params.samplesBaseLineMean;
		stream << YAML::Key << "SamplesPeak" << YAML::Value << params.samplesPeakMean;
		stream << YAML::Key << "PeakHoldoff" << YAML::Value << params.peakHoldOff;
		stream << YAML::Key << "BaselineHoldoff" << YAML::Value << params.baseLineHoldOff;
		stream << YAML::Key << "TriggerHoldoff" << YAML::Value << params.triggerHoldOff;
		stream << YAML::Key << "RiseTimeWindow" << YAML::Value << params.riseTimeValidationWindow;
		stream << YAML::Key << "RiseTimeDiscrimination" << YAML::Value << params.riseTimeDiscrimination;
		stream << YAML::Key << "DigitalProbeGain" << YAML::Value << params.digitalProbeGain;
		stream << YAML::Key << "EnergyNormFactor" << YAML::Value << params.energyNormalizationFactor;
		stream << YAML::Key << "InputDecimation" << YAML::Value << params.inputDecimation;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const Daqromancy::PSDParameters& params)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsEnabled" << YAML::Value << params.isEnabled;
		stream << YAML::Key << "PreTrigger" << YAML::Value << params.preTriggerTime;
		stream << YAML::Key << "DCOffset" << YAML::Value << params.dcOffset;
		stream << YAML::Key << "PulsePolarity" << YAML::Value << params.pulsePolarity;
		stream << YAML::Key << "DynamicRange" << YAML::Value << params.dynamicRange;
		stream << YAML::Key << "BaselineThreshold" << YAML::Value << params.baselineThreshold;
		stream << YAML::Key << "TriggerHoldoff" << YAML::Value << params.triggerHoldOff;
		stream << YAML::Key << "TriggerThreshold" << YAML::Value << params.triggerThreshold;
		stream << YAML::Key << "SelfTrigger" << YAML::Value << params.selfTrigger;
		stream << YAML::Key << "ChargeSensitivity" << YAML::Value << params.chargeSensitivity;
		stream << YAML::Key << "ShortGate" << YAML::Value << params.shortGate;
		stream << YAML::Key << "LongGate" << YAML::Value << params.longGate;
		stream << YAML::Key << "PreGate" << YAML::Value << params.preGate;
		stream << YAML::Key << "TriggerWindow" << YAML::Value << params.triggerValidationWindow;
		stream << YAML::Key << "BaselineSamples" << YAML::Value << params.samplesBasline;
		stream << YAML::Key << "DiscriminatorType" << YAML::Value << params.discrminatorType;
		stream << YAML::Key << "CFDFraction" << YAML::Value << params.cfdFraction;
		stream << YAML::Key << "CFDDelay" << YAML::Value << params.cfdDelay;
		stream << YAML::Key << "TriggerConfig" << YAML::Value << params.triggerConfig;
		stream << YAML::Key << "PileUpRejection" << YAML::Value << params.pileUpRejection;
		stream << YAML::Key << "PURGap" << YAML::Value << params.purgap;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const std::vector<Daqromancy::PHAParameters>& channels)
	{
		stream << YAML::BeginSeq;
		for (const auto& params : channels)
			stream << params;
		stream << YAML::EndSeq;
		return stream;
	}

	Emitter& operator<<(Emitter& stream, const std::vector<Daqromancy::PSDParameters>& channels)
	{
		stream << YAML::BeginSeq;
		for (const auto& params : channels)
			stream << params;
		stream << YAML::EndSeq;
		return stream;
	}

	Emitter& operator<<(Emitter& stream, const Daqromancy::PHAWaveParameters& waves)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsDual" << YAML::Value << waves.isDual;
		stream << YAML::Key << "AnalogProbe1" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "AnalogProbe2" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "DigitalProbe1" << YAML::Value << waves.digitalProbe1;
		stream << YAML::EndMap;

		return stream;
	}

	Emitter& operator<<(Emitter& stream, const Daqromancy::PSDWaveParameters& waves)
	{
		stream << YAML::BeginMap;
		stream << YAML::Key << "IsDual" << YAML::Value << waves.isDual;
		stream << YAML::Key << "AnalogProbe1" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "AnalogProbe2" << YAML::Value << waves.analogProbe1;
		stream << YAML::Key << "DigitalProbe1" << YAML::Value << waves.digitalProbe1;
		stream << YAML::Key << "DigitalProbe2" << YAML::Value << waves.digitalProbe2;
		stream << YAML::EndMap;

		return stream;
	}
	
}

namespace Daqromancy {


	ProjectSerializer::ProjectSerializer(const std::filesystem::path& filepath) :
		m_filepath(filepath)
	{
	}

	ProjectSerializer::~ProjectSerializer() {}

	void ProjectSerializer::SerializeData(const DYProject::Ref& project)
	{
		std::ofstream output(m_filepath);
		if (!output.is_open())
		{
			DY_ERROR("Unable to open {0} to seralize project data!", m_filepath);
			return;
		}

		YAML::Emitter yamlStream;

		yamlStream << YAML::BeginMap;

		yamlStream << YAML::Key << "ProjectPath" << YAML::Value << project->GetProjectPath().string();
		yamlStream << YAML::Key << "RunNumber" << YAML::Value << project->GetRunNumber();
		yamlStream << YAML::Key << "DPPAcqMode" << YAML::Value << project->GetDPPAcqMode();
		
		std::vector<DigitizerArgs> argList = project->GetDigitizerArgsList();

		yamlStream << YAML::Key << "Digitizers" << YAML::Value << YAML::BeginSeq;
		for (auto& args : argList)
		{
			yamlStream << YAML::BeginMap;
			yamlStream << YAML::Key << "Digitizer" << YAML::Value << args.name;
			yamlStream << YAML::Key << "DigitizerArgs" << YAML::Value << args;
			yamlStream << YAML::Key << "DigitizerParameters" << YAML::Value << project->GetDigitizerParameters(args.handle);
			switch (args.firmware)
			{
				case CAEN_DGTZ_DPPFirmware_PHA:
				{
					yamlStream << YAML::Key << "PHAChannels" << YAML::Value << project->GetPHAParameters(args.handle);
					yamlStream << YAML::Key << "PHAWaves" << YAML::Value << project->GetPHAWaveParameters(args.handle);
					break;
				}
				case CAEN_DGTZ_DPPFirmware_PSD:
				{
					yamlStream << YAML::Key << "PSDChannels" << YAML::Value << project->GetPSDParameters(args.handle);
					yamlStream << YAML::Key << "PSDWaves" << YAML::Value << project->GetPSDWaveParameters(args.handle);
					break;
				}
				case CAEN_DGTZ_DPPFirmwareNotSupported:
				{
					break;
				}
			}
			yamlStream << YAML::EndMap;
		}
		yamlStream << YAML::EndSeq << YAML::EndMap;

		output << yamlStream.c_str();

		output.close();
	}

	void ProjectSerializer::DeserializeData(const DYProject::Ref& project)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(m_filepath.string());
		}
		catch (YAML::ParserException& e)
		{
			DY_ERROR("Unable to load in project settings from file {0}", m_filepath);
			return;
		}

		project->SetProjectPath(data["ProjectPath"].as<std::string>());
		project->SetRunNumber(data["RunNumber"].as<uint32_t>());
		project->SetDPPAcqMode((DPPAcqMode)data["DPPAcqMode"].as<int>());

		YAML::Node digitizers = data["Digitizers"];

		//Init some memory  for digitizer data
		DigitizerArgs args;
		DigitizerParameters params;
		std::vector<PHAParameters> phaChannels;
		std::vector<PSDParameters> psdChannels;
		PHAWaveParameters phaWaves;
		PSDWaveParameters psdWaves;

		if (digitizers)
		{
			for (auto dgtz : digitizers)
			{
				args = dgtz["DigitizerArgs"].as<DigitizerArgs>();
				args.name = dgtz["Digitizer"].as<std::string>();
				params = dgtz["DigitizerParameters"].as<DigitizerParameters>();
				switch (args.firmware)
				{
					case CAEN_DGTZ_DPPFirmware_PHA:
					{
						phaChannels = dgtz["PHAChannels"].as<std::vector<PHAParameters>>();
						phaWaves = dgtz["PHAWaves"].as<PHAWaveParameters>();

						if (args != project->GetDigitizerArgs(args.handle))
						{
							DY_ERROR("When attempting to deserialize project settings, board mismatch detected at handle {0}! Stopping load, Daqromancy should be restarted.", args.handle);
							return;
						}

						project->SetDigitizerParameters(args.handle, params);
						project->SetPHAParameters(args.handle, phaChannels);
						project->SetPHAWaveParameters(args.handle, phaWaves);
						break;
					}
					case CAEN_DGTZ_DPPFirmware_PSD:
					{
						psdChannels = dgtz["PSDChannels"].as<std::vector<PSDParameters>>();
						psdWaves = dgtz["PSDWaves"].as<PSDWaveParameters>();

						if (args != project->GetDigitizerArgs(args.handle))
						{
							DY_ERROR("When attempting to deserialize project settings, board mismatch detected at handle {0}! Stopping load, Daqromancy should be restarted.", args.handle);
							return;
						}

						project->SetDigitizerParameters(args.handle, params);
						project->SetPSDParameters(args.handle, psdChannels);
						project->SetPSDWaveParameters(args.handle, psdWaves);
						break;
					}
					case CAEN_DGTZ_DPPFirmwareNotSupported:
					{
						DY_WARN("Invalid firmware detected, this better just be a debug test");

						if (args != project->GetDigitizerArgs(args.handle))
						{
							DY_ERROR("When attempting to deserialize project settings, board mismatch detected at handle {0}! Stopping load, Daqromancy should be restarted.", args.handle);
							return;
						}

						project->SetDigitizerParameters(args.handle, params);
					}
				}
			}
		}
	}
}