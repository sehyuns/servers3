// @file	sample.cpp
//
//

#include "base_connector.h"

int main()
{

	WSADATA wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);

	shared::BaseConnector connector;

	if (ERR_SUCCEEDED != connector.initialize())
		exit(1);

	if (ERR_SUCCEEDED != connector.add_connection(string("127.0.0.1"), 8010, 1))
		exit(2);

	connector.start_connector();

	while(TRUE)
	{ }
	return 0;
}