
void cn::Chainsnet::listen_nodes() {

	while(1) {
        if(net_id.empty()) return;

		for(Node n : nodes) {
			if(n.is_data())
				data_analyze(n.get_data(), net_id);
		}
	}
}

cn::Chainsnet::Chainsnet(str new_id) : net_id(new_id) {
	nodes_thread = thr(listen_nodes, this);
	set_listening();
}

cn::Chainsnet::~Chainsnet() {
	net_id.clear();
	closesocket(listen_sock);
	cn::nodes_stop(nodes);
	listen_thread.detach();
	nodes_thread.detach();
}

void cn::Chainsnet::set_listening() {

	is_listening = 1;

	listen_thread = thr(listen_income, 
		ref(nodes), ref(listen_sock));

	cout << "[i] Listening enabled" << endl;
}

void cn::Chainsnet::net_create(str &path) {
	try {
		EC_KEY *key = key_crate();
		keys_write(key, path);
		sign_dir(key, path);

	} catch(int) {
		if(is_test) {
			cout << "[!] Failed: net_create()" << endl;
		}
		throw 0;
	}

	
}
