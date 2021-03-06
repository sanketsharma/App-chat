#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <deque>
#include <boost/asio.hpp>
#include <protocol/protocol.h>

class Client {
public:
    Client(boost::asio::io_service &io, const boost::asio::ip::tcp::resolver::results_type& eps)
        : io_service(io), sock(io)
    {
        do_connect(eps);
    }

    void write(const std::string& message);

    void close() {
        boost::asio::post(io_service, [this]() {
            sock.close();
        });
    }
    void set_login_id(int id)   { client_id = id;}
    const char* get_login() const { return login; }

private:
    boost::asio::io_service &io_service;
    boost::asio::ip::tcp::socket sock;

    Message receiving_message;
    std::deque<Message> sending_message;

    std::deque<packet_ptr> packets_to_server;

    char login[Block::LoginName];
    char password[Block::Password];
    int32_t client_id;
    int32_t room_id;

private:
    input_req_ptr logon();
    void do_connect(const boost::asio::ip::tcp::resolver::results_type& eps);

    void send_login_packet(packet_ptr packet);
    void read_response_header();
    void read_response_data(response_ptr);
    void read_response_text_data(text_response_ptr packet);

    void start_sending();
    void send_data();
};

#endif // CLIENT_H
