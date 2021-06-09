void socket_init() {
	static WSADATA ws;
	WSAStartup (MAKEWORD( 2, 2 ), &ws);
}

SOCKET cn::make_socket_binded(int &port, int &max_conn) {
	
	SOCKET new_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(port);
	
	int err = bind(new_sock, (sockaddr*) &sock_addr, sizeof(sock_addr));
	if (err < 0) { // If failed to bind the socket
		if(is_test) 
			{cout << "[!] Failed make_socket_binded(): " << port << endl;}
		throw 0;
	} 
	
	listen(new_sock, max_conn);
	
	return new_sock;
}