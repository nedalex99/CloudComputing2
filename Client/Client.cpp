#include <iostream>
#include <locale>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>
#include <ctime>

#include <DateOperation.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;

bool IsLeap(int year)
{
	return (((year % 4 == 0) &&
		(year % 100 != 0)) ||
		(year % 400 == 0));
}

bool IsValidDate(int m, int d, int y)
{
	if (m < 1 || m > 12)
	{
		return false;
	}

	if (d < 1 || d > 31)
	{
		return false;
	}

	if (m == 2)
	{
		if (IsLeap(y))
		{
			return (d <= 29);
		}
		else
		{
			return (d <= 28);
		}
	}

	if (m == 4 || m == 6 || m == 9 || m == 11)
	{
		return (d <= 30);
	}

	return true;
}

int main()
{
	grpc_init();
	ClientContext context;

	auto print_stub = DateService::NewStub(grpc::CreateChannel("localhost:8888",
		grpc::InsecureChannelCredentials()));
	DateRequest dateRequest;

	std::string delimiter = "/";
	std::string token;
	std::string date;
	size_t pos = 0;

	while (true)
	{
		std::cout << "Birth date: ";
		std::cin >> date;
		try
		{
			if (std::count(date.begin(), date.end(), '/') != 2)
			{
				throw "The date must have only /!\n";
			}
			
			std::vector<std::string> dates;

			while ((pos = date.find(delimiter)) != std::string::npos)
			{
				token = date.substr(0, pos);
				dates.push_back(token);
				date.erase(0, pos + delimiter.length());
			}
			dates.push_back(date);

			if (!IsValidDate(std::stoi(dates[0]), std::stoi(dates[1]), std::stoi(dates[2])))
			{
				throw "Invalid date!\n";
			}

			dateRequest.set_month(dates[0]);
			dateRequest.set_day(dates[1]);
			dateRequest.set_yeaar(dates[2]);

			break;
		}
		catch (const char* message)
		{
			std::cout << message;
		}
	}

	DateResponse response;
	auto status = print_stub->Zodiac(&context, dateRequest, &response);
	if (status.ok())
	{
		std::cout << "Zodie: " << response.result() << "\n";
	}

	return 0;
}
