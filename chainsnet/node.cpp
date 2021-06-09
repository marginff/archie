void cn::Node::stop() {
	closesocket(n_sock);
	listen_thr.detach();
}

void cn::Node::start() {
	listen_thr = thr(listen_node, ref(data), ref(n_sock));
}

void cn::nodes_stop(vec<Node> &nodes) {

	for(int n_num = 0; n_num < (int)nodes.size(); n_num++) {
		if(is_test) {
			cout << "[i] Stopping node: " << n_num << endl;
		}
		nodes[n_num].stop();
	}	

	nodes.clear();
}