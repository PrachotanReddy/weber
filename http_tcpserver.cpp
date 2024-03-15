#include "http_tcpserver.h"
#include <iostream>
#include <unistd.h>
#include <sstream>

namespace{
    void log(const std::string &msg){
        std::cout<<msg<<std::endl;
    }
    void errorexit(const std::string &errmsg){
        std::cout<<WSAGetLastError()<<std::endl;
        log("Error: "+errmsg);
        exit(1);
    }
}
namespace web{
    server::server(std::string ip, int port):m_ip(ip),m_port(port),m_sock(),m_newsock(),m_socketAddress(),m_socketAddress_len(sizeof(m_socketAddress)),m_serverMessage(buildres()),m_wsadata()
    {
        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip.c_str());
        startServer();
    }
    server::~server()
    {
        endServer();
    }
    int server::startServer(){
        if(WSAStartup(MAKEWORD(2,0),&m_wsadata)!=0)
            errorexit("Startup failed");
        m_sock=socket(AF_INET,SOCK_STREAM,0);
        if(m_sock<0)
            errorexit("Cannot create socket");
        if(bind(m_sock,(sockaddr*)&m_socketAddress,m_socketAddress_len)<0)
            errorexit("Cannot bind socket with address");
        return 0;
    }
    void server::endServer(){
        closesocket(m_sock);
        closesocket(m_newsock);
        WSACleanup();
        exit(0);
    }
    void server::beginlisten(){
        if(listen(m_sock,20)<0)
            errorexit("Listening on socket failed");
        std::ostringstream ss;
        ss << "\n^^^^ Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " at PORT: " << ntohs(m_socketAddress.sin_port) << " ^^^^\n";
        log(ss.str());
        while(true){
            log("^^^^ Waiting for next connection ^^^^\n\n");
            acceptconn(m_newsock);
            const int BUFFER_SIZE = 30720;
            char buffer[BUFFER_SIZE] = {0};
            int bytesReceived = recv(m_newsock, buffer, BUFFER_SIZE, 0);
            if (bytesReceived < 0)
                errorexit("Did not receive bytes from client socket connection");
            std::ostringstream ss;
            ss << "^^^^ Request Received from client ^^^^\n";
            log(ss.str());
            sendres();
            closesocket(m_newsock);
        }

    }
    void server::acceptconn(SOCKET &newsocket){
        newsocket=accept(m_sock,(sockaddr*)&m_socketAddress,&m_socketAddress_len);
        if(newsocket<0){
            std::ostringstream ss;
            ss << 
            "Server did not accept incoming connection from ADDRESS: " 
            << inet_ntoa(m_socketAddress.sin_addr) << "; at PORT: " 
            << ntohs(m_socketAddress.sin_port);
            errorexit(ss.str());
        }
    }
    std::string server::buildres(){
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void server::sendres(){
        int bytesSent;
        long totalBytesSent = 0;
        while (totalBytesSent < m_serverMessage.size())
        {
            bytesSent = send(m_newsock, m_serverMessage.c_str(), m_serverMessage.size(), 0);
            if (bytesSent < 0)
                break;
            totalBytesSent += bytesSent;
        }
        if (totalBytesSent == m_serverMessage.size())
            log("^^^^ Server Response sent to client ^^^^\n");
        else
            log("Error sending response to client.");
    }

}