#include <iostream>
#include <sys/socket.h>
#include <fstream>
#include <netinet/in.h>
#include <sys/wait.h>
#include "unistd.h"
#include <cerrno>
#include <arpa/inet.h>

std::ifstream fin;

int main(int argc, char **argv) {
    //read args for the port to listen on
    int port = 8080;
    if (argc == 1) {
        std::clog << "Defaulting to port 8080\n";
    } else if (argc != 2) {
        std::cerr << "Usage: ./server <port to bind on>\n";
        return 1;
    } else {
        port = std::stoi(argv[1]);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int bindfd = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (bindfd == -1) {
        std::cerr << "Error binding socket\n";
        return 1;
    }
    listen(sockfd, 20);
    std::clog << "Listening for connections\n";
    fork();
    fork();
    fork();
    fork();
    while (true) {
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int newfd = accept(sockfd, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (newfd == -1) {
            std::cerr << "Error accepting connection\n";
            std::cerr << errno << std::endl;
            continue;
        }
        char buf[1024] = {'\0'};
        int recvfd = recv(newfd, buf, 1024, 0);
        if (recvfd == -1) {
            std::cerr << "Error receiving data\n";
        }
        std::string res = buf;
        if (res.empty() || res.find(' ') == std::string::npos) {
            std::cerr << "Error: invalid request\n";
            close(newfd);
            continue;
        }
        std::string reqMethod = res.substr(0, res.find(' ')), reqPath = res.substr(res.find(' ') + 1, res.find(' ',
                                                                                                               res.find(
                                                                                                                       ' ') +
                                                                                                               1) -
                                                                                                      res.find(
                                                                                                              ' ') -
                                                                                                      1);
        std::clog << reqMethod << " at " << reqPath << " at " << time(nullptr) << " from "
                  << inet_ntoa(clientAddress.sin_addr)
                  << '\n';
        //std::clog << res << std::endl;
        if (reqPath.find("..") != std::string::npos) {
            std::cerr
                    << "Error: path contains .. (This is a security protection against directory traversal attacks)\nShutting down connection\n";
            shutdown(newfd, SHUT_RDWR);
            continue;
        }
        if (reqPath == "/") reqPath = "/index.html";
        fin.open(reqPath.substr(1));
        std::string data;
        if (!fin.is_open() || reqMethod != "GET") {
            data = "HTTP/1.0 404 Not Found\n"
                   "Server: http-server\n"
                   "Content-Type: text/html\n"
                   "Connection: close\r\n\r\n";
            //try to find 404.html
            fin.open("404.html");
            if (fin.is_open()) {
                std::string line;
                while (getline(fin, line)) {
                    data += line + "\n";
                }
            } else {
                data += "<html>\n"
                        "<head><title>404 Not Found</title></head>\n"
                        "<body>\n"
                        "<center><h1>404 Not Found</h1></center>\n"
                        "<hr><center>http-server</center>\n"
                        "</body>\n"
                        "</html>\n";
            }
        } else {
            data = "HTTP/1.0 200 OK\n"
                   "Server: http-server\n"
                   "Connection: close\n";
            if (reqPath.substr(reqPath.find_last_of('.') + 1) == "html") data += "Content-Type: text/html; charset=UTF-8\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "css") data += "Content-Type: text/css\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "js") data += "Content-Type: text/javascript\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "png") data += "Content-Type: image/png\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "jpg") data += "Content-Type: image/jpeg\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "gif") data += "Content-Type: image/gif\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "ico") data += "Content-Type: image/x-icon\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "txt") data += "Content-Type: text/plain\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "pdf") data += "Content-Type: application/pdf\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "mp3") data += "Content-Type: audio/mpeg\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "mp4") data += "Content-Type: video/mp4\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "zip") data += "Content-Type: application/zip\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "gz") data += "Content-Type: application/gzip\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "tar") data += "Content-Type: application/x-tar\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "xml") data += "Content-Type: application/xml\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "json") data += "Content-Type: application/json\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "svg") data += "Content-Type: image/svg+xml\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "woff") data += "Content-Type: font/woff\n";
            else if (reqPath.substr(reqPath.find_last_of('.') + 1) == "woff2") data += "Content-Type: font/woff2\n";
            else data += "\n";
            data += "\n";
            std::string line;
            while (getline(fin, line)) {
                data += line + "\n";
            }
        }
        int sentDat = send(newfd, data.c_str(), data.length(), 0);
        if (sentDat < data.length()) std::cerr << "Warning: problem sending data: data not complete\n";
        fin.close();
        close(newfd);
    }
}