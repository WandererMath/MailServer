#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include<asio.hpp>
#include<asio/ts/buffer.hpp>
#include<asio/ts/internet.hpp>
#include<thread>

#define PORT 2334
#define IP "157.119.101.36"

using namespace std;
using asio::ip::tcp;


void send_mail(tcp::socket* socket, string key) {
	string buffer;
	asio::write(*socket, asio::buffer("S" + key + "##"));
	cout << "Message:  ";
	//cin >> buffer;
	cin.ignore();//  !! Trick
	getline(cin, buffer);
	asio::write(*socket, asio::buffer(buffer+"#"));
}

void recv_mail(tcp::socket* socket, string key) {
	string c = "R"+key + "##";
	asio::error_code err;
	asio::write(*socket, asio::buffer(c), err);
	asio::streambuf buffer;
	asio::read_until(*socket, buffer, "####");
	cout << &buffer << endl;
}

int main() {
	string key;
	int cmd;
	cout << "Keyword:\t";
	cin >> key;

	asio::io_context context;
	tcp::endpoint endpoint(asio::ip::address::from_string(IP), PORT);
	asio::error_code err;

	while (1) {
		cout << ">> ";
		cin >> cmd;
		if (cmd == 3)
			break;
		if (cmd == 1) {
			tcp::socket socket(context);
			socket.connect(endpoint, err);
			send_mail(&socket,  key);
			socket.close();
		}
		if (cmd == 2) {
			tcp::socket socket(context);
			socket.connect(endpoint, err);
			recv_mail(&socket, key);
			socket.close();
		}
	}

}