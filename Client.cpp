// client.cpp
#include <boost/asio.hpp>
#include <array>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <chrono>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

// Fast decimal parser: reads integer and fractional part, advances ptr
static double parse_double(const char*& ptr) {
    double value = 0.0;
    while (*ptr >= '0' && *ptr <= '9') {
        value = value * 10 + (*ptr - '0');
        ++ptr;
    }
    if (*ptr == '.') {
        ++ptr;
        double place = 1.0;
        while (*ptr >= '0' && *ptr <= '9') {
            value += (*ptr - '0') * (place *= 0.1);
            ++ptr;
        }
    }
    return value;
}

int main(int argc, char* argv[]) {
    const char* trader_name = (argc > 1 ? argv[1] : "Trader1");
    boost::asio::io_context io;

    // JOIN UDP MULTICAST
    udp::socket udp_sock(io);
    udp_sock.open(udp::v4());
    udp_sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    udp_sock.bind(udp::endpoint(udp::v4(), 3001));
    udp_sock.set_option(
            boost::asio::ip::multicast::join_group(
                    boost::asio::ip::make_address("239.255.0.1")
            )
    );

    // PERSISTENT TCP CONNECTION
    tcp::socket tcp_sock(io);
    tcp::endpoint server_ep(boost::asio::ip::make_address("127.0.0.1"), 4000);
    tcp_sock.connect(server_ep);

    std::array<char, 2000> buf;
    struct { char t[8]; double bid, ask; } entries[10];
    char target[8];
    int challenge_id = 0;

    for (;;) {
        // 1) Receive UDP packet
        udp::endpoint sender;
        size_t len = udp_sock.receive_from(boost::asio::buffer(buf), sender);
        buf[len] = '\0';

        // 2) Mark tick arrival (only processing time measured)
        auto t_tick = std::chrono::steady_clock::now();

        // 3) Parse market data into entries array
        int ec = 0;
        char* p = buf.data();
        while (*p && ec < 10) {
            if (std::memcmp(p, "SEC|", 4) == 0) {
                p += 4;
                std::memcpy(entries[ec].t, p, 7);
                entries[ec].t[7] = '\0';
                p += 7 + 5; // skip "TICKER|BID|"
                entries[ec].bid = parse_double((const char*&)p);
                p += 1 + 4; // skip "|ASK|"
                entries[ec].ask = parse_double((const char*&)p);
                ++ec;
            }
            while (*p && *p++ != '\n');
        }

        // 4) Extract CHALLENGE_ID and TARGET
        if (auto id_pos = std::strstr(buf.data(), "CHALLENGE_ID:"))
            challenge_id = std::atoi(id_pos + 13);
        if (auto tgt_pos = std::strstr(buf.data(), "TARGET:")) {
            std::memcpy(target, tgt_pos + 7, 7);
            target[7] = '\0';
        }

        // 5) Lookup bid/ask for TARGET
        double bid = 0, ask = 0;
        for (int i = 0; i < ec; ++i) {
            if (std::memcmp(entries[i].t, target, 8) == 0) {
                bid = entries[i].bid;
                ask = entries[i].ask;
                break;
            }
        }

        // 6) Send TCP response
        char out[128];
        int n = std::snprintf(
                out, sizeof(out),
                "CHALLENGE_RESPONSE %d %s %.6f %.6f %s\n",
                challenge_id, target, bid, ask, trader_name
        );
        boost::asio::write(tcp_sock, boost::asio::buffer(out, n));

        // 7) Log processing latency only
        auto t_send = std::chrono::steady_clock::now();
        auto proc_us = std::chrono::duration_cast<std::chrono::microseconds>(t_send - t_tick).count();
        std::cout << "[LOG] Challenge " << challenge_id
                  << ", Target=" << target
                  << ", Bid=" << std::fixed << std::setprecision(6) << bid
                  << ", Ask=" << ask
                  << ", ProcTime=" << proc_us << " us"
                  << std::endl;
    }

    return 0;
}
