void cn::data_analyze(vec<vec<char>> &data_vec, str &net_id) {
    
    str data;
    for(vec<char> buf : data_vec) {
        for(char cha : buf)
        	data_str += cha;
    }

    str net_id_data = data.substr(0, data.find(" "));
    data.erase(0, data.find(" ") + 1);

    str format = data.substr(0, data.find(" "));
    data.erase(0, data.find(" ") + 1);

    if(net_id_data != net_id)
    	return;


    if(format == "a") {
    	cout <<"a";
    }
//	data >> foramt;


}