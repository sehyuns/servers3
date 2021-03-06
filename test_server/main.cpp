// @file	main.cpp
//
//


#include "shared_server.h"
#include "session.h"

#include "logger.h"

int main()
{
	shared::SharedServer server;

	LOG(DEBUG, "start server!!");

	shared::Session* session = new shared::Session(1);
	std::list<shared::Session*> session_list;
	session_list.push_back(session);

	if (ERR_SUCCEEDED != server.initialize(string("0.0.0.0"), 8010, &session_list))
		exit(1);

	if (ERR_SUCCEEDED != server.start_server())
		exit(2);

	while (TRUE)
	{
	}


	//HANDLE iocp;
	//WSADATA wd;
	//WSAStartup(MAKEWORD(2, 2), &wd);

	//iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	//if (!iocp)
	//{
	//	throw "";
	//}

	//SOCKET listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	//if (listen_socket == INVALID_SOCKET)
	//{
	//	throw "";
	//}

	//BOOL opt_val = TRUE;
	//int opt_len = sizeof(BOOL);
	////setsockopt (listen_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt_val, opt_len); 

	//sockaddr_in service;
	//service.sin_family = AF_INET;
	//service.sin_addr.s_addr = htonl(INADDR_ANY);
	//service.sin_port = htons(8010);

	//if (bind(listen_socket, (SOCKADDR *)&service, sizeof(service)) != 0)
	//{
	//	throw "";
	//}

	//if (listen(listen_socket, 5) != 0)
	//{
	//	throw "";
	//}

	//int i = 1;
	//if (!CreateIoCompletionPort((HANDLE)listen_socket, iocp, (INT32)listen_socket, 1))
	//{
	//	throw "";
	//}

	//GUID acceptex_guid = WSAID_ACCEPTEX;
	//LPFN_ACCEPTEX lpfn_accept;
	//DWORD bytes;
	//if (WSAIoctl(
	//	listen_socket,
	//	SIO_GET_EXTENSION_FUNCTION_POINTER,
	//	&acceptex_guid,
	//	sizeof(acceptex_guid),
	//	&lpfn_accept,
	//	sizeof(lpfn_accept),
	//	&bytes,
	//	0, 0) == SOCKET_ERROR)
	//{
	//	throw "";
	//}

	//SOCKET accept_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	//WSAOVERLAPPED ov;
	//memset(&ov, 0, sizeof(ov));
	//char* accept_data = new char[1024];
	//bytes = 0;

	//if (lpfn_accept(listen_socket,
	//	accept_socket,
	//	accept_data,
	//	/*1024 - (2 * (sizeof(SOCKADDR_IN) + 16)),*/
	//	0,
	//	sizeof(SOCKADDR_IN) + 16,
	//	sizeof(SOCKADDR_IN) + 16,
	//	&bytes,
	//	&ov) == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
	//{
	//	throw "";
	//}

	//DWORD flag;
	//WSAOVERLAPPED op_data;
	//memset(&op_data, 0, sizeof(op_data));
	//int key = 0;

	//bytes = 0;
	//DWORD ret = GetQueuedCompletionStatus(iocp, &bytes, (ULONG_PTR *)&key, (OVERLAPPED **)&op_data, INFINITE);

	//if (setsockopt(accept_socket,
	//	SOL_SOCKET,
	//	SO_UPDATE_ACCEPT_CONTEXT,
	//	(char *)&listen_socket,
	//	sizeof(listen_socket)) == SOCKET_ERROR)
	//{
	//	DWORD ret = WSAGetLastError();
	//	throw "";
	//}

	//WSABUF recv_data;
	//recv_data.len = 100;
	//recv_data.buf = new char[recv_data.len];
	//memset(&ov, 0, sizeof(ov));
	//bytes = 0;

	////WSAGetOverlappedResult (listen_socket, &ov, &bytes, TRUE, &flag); 
	//if (WSARecv(accept_socket, &recv_data, 1, &bytes, &flag, &ov, 0) == SOCKET_ERROR)
	//{
	//	DWORD ret = WSAGetLastError();//<- 10045 

	//	switch (ret)
	//	{
	//	case WSA_IO_PENDING:
	//		break;
	//	case WSAECONNRESET:
	//	default:;
	//	}
	//}

	return 0;
}