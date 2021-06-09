EC_KEY *cn::key_crate() {

	EC_KEY *new_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
 	EC_KEY_generate_key(new_key);
 	EC_KEY_check_key(new_key);

    return new_key;
}

void cn::keys_write(EC_KEY *key, str &path) {

    if(!EC_KEY_check_key(key)) {
        if(is_test){
        	cout << "[!] Failed keys_write()" << endl;
        }
		throw 0;   	
    }

 	char *pubkey_hex = EC_POINT_point2hex(EC_KEY_get0_group(key), 
 		EC_KEY_get0_public_key(key), 
 		POINT_CONVERSION_COMPRESSED, BN_CTX_new()); 	
 	
    
    str path_pub = path + pubkey_name;
    str path_priv = path + privkey_name;

 	ofstream f(path_pub, std::ios::trunc); 
 	if(!f.is_open()){ 
 		if(is_test) {
 			cout << "[!] Failed: keys_write() " << path << endl;
 		}
 		throw 1;
 	}
 	f << pubkey_hex;
 	f.close(); 	

 	f.open(path_priv, std::ios::trunc);
 	f << BN_bn2hex(EC_KEY_get0_private_key(key));
 	f.close();

 	if(is_test) {
 		cout << "[i] Created new keys: " << 
 		path_pub << ", " << path_priv << endl;
 	}

}

void cn::sign_dir(EC_KEY *key, str &path) {
    if(!EC_KEY_check_key(key)) {
        if(is_test){
        	cout << "[!] Failed sign_dir(), 0" << endl;
        }
		throw 0;   	
    }

	try {
		for(auto &file : directory_iterator(path)) 
			sign_file(key, const_cast<fs::path&>(file.path()));

	} catch(fs::filesystem_error&) {
        if(is_test){
        	cout << "[!] Failed sign_dir(), 1" << endl;
        }
		throw 1;
	}

	
}

void cn::sign_file(EC_KEY *key, fs::path &path) {
	ifstream f(path, std::ios::ate);
	if(!f.is_open()) {
        if(is_test){
        	cout << "[!] Failed sign_file()" << endl;
        }
		throw 0;
	}
	if(!EC_KEY_check_key(key)) {
        if(is_test){
        	cout << "[!] Failed sign_file()" << endl;
        }
		throw 1;   	
    }
    if(path.extension() == name) {
    	// if(is_test){
     //    	cout << "[i] sign_file() of .cn" << endl;
     //    }
		return;
    }
	str name_new = path.string() + name;
	ofstream f_new(name_new, std::ios::trunc);

	int farg_all = ceil((float)f.tellg() / frag_size);
	str f_name = path.filename().string();
	char *buff = new char[frag_size]; 

	f.seekg(0);
	for(int frag = 1; frag <= farg_all; frag++) {
		f.get(buff, frag_size + 1);
		str buff_str(buff);

		str info = f_name + " " + to_string(frag) + " " + 
		to_string(farg_all) + " " + buff_str;

		f_new << sign_str(key, info) << info << endl;
	}
}



str cn::sign_str(EC_KEY *key, str &text) {

    if(!EC_KEY_check_key(key)) {
        if(is_test){
        	cout << "[!] Failed sign_str()" << endl;
        }
		throw 0;   	
    }
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_PKEY_assign_EC_KEY(pkey, key);
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new(pkey,NULL);
    EVP_PKEY_sign_init(pkey_ctx);
    EVP_PKEY_CTX_set_signature_md(pkey_ctx, EVP_sha256());

    size_t sig_len=0;
    EVP_PKEY_sign(pkey_ctx, NULL,&sig_len, 
    	(const unsigned char*)text.c_str(), text.size());
    
    str res; res.assign(sig_len,0);
    EVP_PKEY_sign(pkey_ctx, (unsigned char*)res.c_str(), &sig_len, 
    	(const unsigned char*)text.c_str(), text.size());

    
    EVP_PKEY_free(pkey);

    str ret;
    for(int ch = 0; ch < res.size(); ch++){
        	ret += to_string((int)res[ch]);
        	ret.append(" ");
        }

    return ret;
}
