#include <iostream>
#include <thread>
#include <vector>
#include <openssl\bio.h>
#include <winsock2.h>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <cmath>
#include <math.h>


#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/evp.h>

using str = std::string;
template<typename T>
using vec = std::vector<T>;
using thr = std::thread;
using sock = SOCKET;
using std::ref;
using std::cout;
using std::endl;
using std::move;
using std::ofstream;
using std::fstream;
using std::ifstream;
using std::to_string;
namespace fs = std::filesystem;
using fs::directory_iterator;
using fs::remove;
using fs::current_path;



namespace cn {
	static int port = 32768;
	static int max_conns = 32768;
	static bool is_test = 1;
	static int max_buff = 2; // 4096
	static str pubkey_name = "net_pub.cn";
	static str privkey_name = "net_priv.cn";
	static str name = ".cn";
	static int frag_size = 2;


	class Node {
	    private:
		sock n_sock;
		thr listen_thr;
		vec<vec<char>> data;
		bool is_data = 0;
		

	    public:
	    Node(sock &new_sock) : n_sock(new_sock) {}
	    void stop();
	    int is_data() {return is_data;}
	    vec<vec<char>> get_data() {return data;}
	    void start();
	};

	class Chainsnet {
		private:
		str net_id;
		bool is_listening = 0;
		thr listen_thread;
		thr nodes_thread;
		sock listen_sock;
		vec<cn::Node> nodes;

        public:
        Chainsnet(str new_id);
        ~Chainsnet();
		void set_listening();
//		void get_file(str file);
//		void send_all(str req);

		void net_create(str &path);
		void listen_nodes();
	};

	SOCKET make_socket_binded(int &port, int &max_conn);
	void listen_node(vec<vec<char>> &data, sock &node_sock);
	void listen_income(vec<Node> &nodes, sock &listen_sock);
	void socket_init();
	void nodes_stop(vec<Node> &nodes);

    void keys_write(EC_KEY *key, str &path);
    EC_KEY *key_crate();
    void sign_dir(EC_KEY *key, str &path);
    void sign_file(EC_KEY *key, fs::path &path);
    str sign_str(EC_KEY *key, str &text);

    void data_analyze(str &data);
}

#include "cn.cpp"
#include "node.cpp"
#include "socket.cpp"
#include "listen.cpp"
#include "crypto.cpp"
#include "data.cpp"
