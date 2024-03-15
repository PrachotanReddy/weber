#ifndef INCLUDED_HTTP_TCPSERVER
#define INCLUDED_HTTP_TCPSERVER

#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string>


namespace web{
    class server{
        public:
            server(std::string ip_address, int port);
            ~server();
            void beginlisten();
        private:
            WSADATA m_wsadata;
            SOCKET m_sock;
            SOCKET m_newsock;
            int m_socketAddress_len;
            struct sockaddr_in m_socketAddress;
            std::string m_ip;
            int m_port;
            std::string m_serverMessage;
            int startServer();
            void endServer();
            void acceptconn(SOCKET &newsocket);
            std::string buildres();
            void sendres();
    };
}
#endif