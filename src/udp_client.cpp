#include <array>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
using boost::asio::ip::udp;

class Client
{
public:
    Client(boost::asio::io_context& io)
    :   socket_(io, udp::v4()),
        server_(boost::asio::ip::make_address("127.0.0.1"), 9000)
    {

    }

    void start()
    {
        message_ = std::make_shared<std::string>("bitch boy");
        do_send();
    }

private:
    void do_send()
    {
        const auto func = [this](boost::system::error_code ec, std::size_t)
        {
            if (!ec)
            {
                do_receive();
            }
        };
        socket_.async_send_to(boost::asio::buffer(*message_), server_, func);
    }

    void do_receive()
    {
        const auto func = [this](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "Reply: " << std::string(buffer_.data(), length) << "\n";
            }
        };
        socket_.async_receive_from(boost::asio::buffer(buffer_), sender_, func);
    }

    udp::socket socket_;
    udp::endpoint server_;
    udp::endpoint sender_;
    std::array<char, 1024> buffer_;
    std::shared_ptr<std::string> message_;
};


int main()
{
    boost::asio::io_context io;
    Client c(io);
    c.start();
    io.run();
}