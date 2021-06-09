void cn::listen_income(vec<Node> &nodes, sock &listen_sock) {

    try {
	 	listen_sock = make_socket_binded(port, max_conns);
	} catch(int) {
	 	return;
	}


	for(int cons = 0; cons < max_conns; cons++) {
		sockaddr_in income_addr;
		int income_addr_size = sizeof(income_addr);

		sock new_sock; 
		int err = (new_sock = accept(listen_sock, 
								(sockaddr*) &(income_addr),
								&income_addr_size));
		if(err < 0) return;

		nodes.push_back(Node(new_sock));
		nodes.back().start();

		if(is_test) {
			cout << "[i] New sock: " << cons << endl;
		}

	}

	closesocket(listen_sock);
}



void cn::listen_node(vec<vec<char>> &data, sock &node_sock) {

    while(1) {
		char *buff = new char[max_buff];
		int count = 0;
	    
	    int len = recv(node_sock, buff, max_buff, 0);
	    if(len < 0) return;

        vec<char> t_date; data.push_back(t_date);

        for(int sym = 0; sym < len; sym++) {
	         data[count].push_back(buff[sym]);
	    }

	    while(len == max_buff) {
            vec<char> t_date; data.push_back(t_date);
	        count++; 

	     	len = recv(node_sock, buff, max_buff, 0);
	     	if(len < 0) return;

	     	for(int sym = 0; sym < len; sym++) {
	          	data[count].push_back(buff[sym]);
	      	}
	    }

	    is_data = 1;

        if(is_test) {
        	cout << "[i] New data: ";
        	for(vec<char> buf : data) {
        		for(char cha : buf)
        			cout << cha;
        	}
        	cout << endl;
        }

	    while (is_data) {}
	}
}