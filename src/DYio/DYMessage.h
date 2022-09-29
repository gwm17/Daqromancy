#ifndef BS_MESSAGE_H
#define BS_MESSAGE_H

#include "DAQ/DigitizerDefs.h"
#include "DYListData.h" 

namespace Daqromancy {

	struct DYMessage
	{
		DYMessage() = default;
		DYMessage(const std::vector<DYData>& data)
		{
			size = data.size() * Data::dataSize;
			body.resize(size);
			for (const auto& datum : data)
				LoadDYDataToBuffer(body, datum);
		}

		uint64_t size; //in bytes
		std::vector<char> body;
	};

}

#endif
