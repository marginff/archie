#include "chainsnet/cn.h"

static str test_id = "333";

void test_fail(str func, int state = 0) {

	cout << "[T] Failed: " << func << ", " << state << endl << endl;
	std::exit(0);
}

void test_success(str func) {

	cout << "[T] Successed: " << func << endl << endl;
}

void test_running(str func) {

	cout << "[T] Running: " << func << endl;
}


void test_make_socket_binded() {
	str name = "make_socket_binded()";
	test_running(name);
	sock test_sock;


	try {
		test_sock = cn::make_socket_binded(cn::port, cn::max_conns);
	}
	catch(int) {
		test_fail(name);
		return;
	}
	closesocket(test_sock);
	test_success(name);
}

void test_listen_income() {

	str name = "test_listen_income()";
	test_running(name);

	vec<cn::Node> test_nodes;
	sock listen_sock;
	thr test_listen_thread = thr(cn::listen_income, 
		ref(test_nodes), ref(listen_sock));

	SOCKET new_sock;
	sockaddr_in new_addr;
	
	str test_ip = "127.0.0.1";
	new_sock = socket(AF_INET, SOCK_STREAM, 0);
	new_addr.sin_family = AF_INET;
	new_addr.sin_addr.S_un.S_addr = inet_addr(test_ip.c_str());
	new_addr.sin_port = htons(cn::port);

	int e = connect(new_sock, 
	 				(sockaddr*) &new_addr, sizeof(new_addr));


	 if (e < 0) { // If failed to connect
	 	closesocket(listen_sock);
	 	cn::nodes_stop(test_nodes);
	 	test_listen_thread.detach();
	 	test_fail(name, 1);
	 	return;
	 }

	 

	  Sleep(2000);

	if(test_nodes.empty()) { // If not added node
	 	closesocket(listen_sock);
	 	cn::nodes_stop(test_nodes);		 	
        test_listen_thread.detach();
        test_fail(name, 2);
		return;
	}

	
	
	closesocket(listen_sock);
	cn::nodes_stop(test_nodes);
	test_success(name);
	test_listen_thread.detach();

//	while (1) {}
}


void test_recv_data() {
	str name = "recv_data";
	test_running(name);

	vec<cn::Node> test_nodes;
	sock listen_sock;
	thr test_listen_thread = thr(cn::listen_income, 
		ref(test_nodes), ref(listen_sock));

	SOCKET new_sock;
	sockaddr_in new_addr;
	
	str test_ip = "127.0.0.1";
	new_sock = socket(AF_INET, SOCK_STREAM, 0);
	new_addr.sin_family = AF_INET;
	new_addr.sin_addr.S_un.S_addr = inet_addr(test_ip.c_str());
	new_addr.sin_port = htons(cn::port);

	connect(new_sock, (sockaddr*) &new_addr, sizeof(new_addr));


    str data = "Testa";
    cout << "[T] Send: " << data << endl;

    const char *buff = data.c_str();
    send(new_sock, buff, data.size(), 0);

    while(!test_nodes.size()) { Sleep(1000); }
    while(!test_nodes[0].is_data()) { Sleep(1000); }
    
	closesocket(listen_sock);
	cn::nodes_stop(test_nodes);
	test_success(name);
	test_listen_thread.detach();
}

void test_keys_create() {
	str name = "keys_create()";
	test_running(name);

	str path = "";
	try{
		cn::keys_write(cn::key_crate(), path);
	}catch(int){
		test_fail(name);
		return;
	}

	str p = path + cn::pubkey_name;
	remove(p.c_str());	

	p = path + cn::privkey_name;
	remove(p.c_str());
	
	test_success(name);
}

void test_net_create() {
	str name = "net_create()";
	test_running(name);

	str path = current_path();
	path.append("/cn_test/");
	try {
	    cn::Chainsnet test(test_id);
		test.net_create(path);
	} catch(int e){
		test_fail(name, e);
	}

//	while (1) {}

	test_success(name);
}

int main() {

	socket_init();

//	test_make_socket_binded();
//	test_listen_income();

  // test_recv_data();

    
  //  test_keys_create();

  //   test_net_create();
	str a = "a b";
	cout << a.substr(0, a.find(" "));
	


	
    //	while (1) {}


    return 0;
}