#include "http_tcpserver.h"

int main(){
    using namespace web;
    server serve=server("127.0.0.1",8080);
    serve.beginlisten();
    return 0;
}