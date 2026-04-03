#include <array>
#include <iostream>

#include <boost/asio.hpp>
using boost::asio::ip::udp;


class Receiver
{
public:
    Receiver(boost::asio::io_context& io)
    :   socket_(io, udp::endpoint(udp::v4(), 9000))
    {
        do_receive();
    }

private:
    void do_receive()
    {
        const auto func = [this](boost::system::error_code ec, std::size_t length)
        {
            if (!ec && length > 0)
            {
                std::cout << "Received: "
                          << std::string(data_.data(), length)
                          << " from " << sender_.address().to_string()
                          << ":" << sender_.port() << "\n";

                std::string msg(data_.data(), length);
                if (msg == "bitch boy")
                {
                    char res[] = "FUCK YOU REE\n";
                    strcpy(data_.data(), res);
                    do_send(sizeof(res));
                }
            }

            do_receive(); // keep listening
        };
        socket_.async_receive_from(boost::asio::buffer(data_), sender_, func);
    }

    void do_send(size_t length)
    {
        const auto func = [this](boost::system::error_code ec, std::size_t)
        {
            if (!ec)
            {
                std::cout << "Send reply\n";
            }
        };
        socket_.async_send_to(boost::asio::buffer(data_, length), sender_, func);
    }

    udp::socket socket_;
    udp::endpoint sender_;
    std::array<char, 1024> data_;
};


int main()
{
    boost::asio::io_context io;
    Receiver r(io);
    io.run();
}

