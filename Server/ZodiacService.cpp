#include "ZodiacService.h"

ZodiacService::ZodiacService()
{
	std::ifstream f("interval.txt");
	
	for (int i = 0; i < 12; i++)
	{
		std::vector<std::string> vect;
		for (int j = 0; j < 3; j++)
		{
			std::string x;
			f >> x;
			vect.push_back(x);
		}
		intervals.push_back(vect);
	}
}

::grpc::Status ZodiacService::Zodiac(::grpc::ServerContext* context, const::DateRequest* request, ::DateResponse* response)
{
	std::string delimiter = "/";
	size_t pos = 0;
	std::string month;

	bool ok = false;

	for (int i = 0; i < 12; i++)
	{
		auto startingDate = intervals[i][0];

		while ((pos = startingDate.find(delimiter)) != std::string::npos)
		{
			month = startingDate.substr(0, pos);

			if (request->month().compare(month) == 0)
			{
				auto day = startingDate.erase(0, pos + delimiter.length());
				if (request->day().compare(day) > 0)
				{
					auto zodie = intervals[i][2];
					response->set_result(zodie);
				}
				else if (i == 0)
				{
					auto zodie = intervals[intervals.size() - 1][2];
					response->set_result(zodie);
				}
				else
				{
					auto zodie = intervals[i - 1][2];
					response->set_result(zodie);
				}
				ok = true;
				break;
			}

			startingDate.erase(0, pos + delimiter.length());
		}
		if (ok)
		{
			break;
		}
	}
	return ::grpc::Status::OK;
}
