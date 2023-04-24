#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <thread>
#include <fstream>
#include <map>

#define PORT 2334
#define MAX 1000
using namespace std;
using asio::ip::tcp;

tcp::socket* sockets[MAX];

map<string, int> key_pid;



//////////////////////////////////////////////
string strbuf2str(asio::streambuf* buffer) {
	ostringstream sline;
	sline << buffer; // pass pointer
	std::string line = sline.str();
	return line;
	//usage: cout<<strbuf2str(&buffer)<<endl;
}

void chat(int pid) {
	cout << "Accepted... \n";
	string key;
	string buffer;
	asio::error_code err;

	asio::read_until(*sockets[pid], asio::dynamic_buffer(key), "##");
	key.pop_back();
	key.pop_back();
	if (key[0]=='R') {
		
		fstream f(key.substr(1), ios::in);

		if (!f.good()) {
			asio::write(*sockets[pid], asio::buffer("NOT EXIST\n####"), err);
			sockets[pid]->close();
			return;
		}
		//asio::write(*sockets[pid], asio::buffer("Hello"), err);
		while (!f.eof()) {
			getline(f, buffer);
			cout << buffer<<"\n";

			asio::write(*sockets[pid], asio::buffer(buffer+ "\n"), err);
		}
		(*sockets[pid]).write_some(asio::buffer("####", 4), err);
	}
	else {
		cout << "write " << key.substr(1) << endl;
		fstream f(key.substr(1), ios::app);
		asio::read_until(*sockets[pid], asio::dynamic_buffer(buffer), "#");
		buffer.pop_back();
		f << buffer<<"\n";
		sockets[pid]->close();
	}

	
}

int main() {
	asio::io_context context;
	tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), PORT));
	int i = 0;
	while (true) {
		sockets[i]=new tcp::socket(context);
		acceptor.accept(*sockets[i]);
		thread(&chat, i).detach();
		i =(i+1)%MAX;
	}

}