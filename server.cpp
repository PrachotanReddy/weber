#include "http_tcpserver.h"

int main(){
    using namespace web;
    server serve=server("0.0.0.0",8080);
    serve.beginlisten();
    return 0;
}