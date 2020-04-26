#pragma once

#include <vector>
#include <fstream>

#include <DateOperation.grpc.pb.h>

class ZodiacService final : public DateService::Service
{
public:
	ZodiacService();
	::grpc::Status Zodiac(::grpc::ServerContext* context, const ::DateRequest* request, ::DateResponse* response) override;

private:
	std::vector<std::vector<std::string>> intervals;
};

