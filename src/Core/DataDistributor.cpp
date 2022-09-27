#include "DataDistributor.h"

namespace Daqromancy {

	uint64_t DataDistributor::s_nextClientID = 0;
	std::unordered_map<uint64_t, ThreadSafeQueue<std::vector<DYData>>*> DataDistributor::s_clientMap;

	void DataDistributor::PushData(const std::vector<DYData>& data)
	{
		for (auto& iter : s_clientMap)
			iter.second->PushBack(data);
	}

	DistributorClient DataDistributor::Connect()
	{
		DistributorClient client;
		client.id = s_nextClientID++;

		client.dataQueue = new ThreadSafeQueue<std::vector<DYData>>();
		s_clientMap[client.id] = client.dataQueue;

		return client;
	}

	void DataDistributor::Disconnect(DistributorClient& client)
	{
		s_clientMap.erase(client.id);
		delete client.dataQueue;
		client.dataQueue = nullptr;
	}
}