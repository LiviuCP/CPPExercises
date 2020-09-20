#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <string>
#include <vector>

class Server
{
public:
    explicit Server();
    explicit Server(size_t bufferSize, int portNumber, const std::vector<int> &serverData, const std::string &name = "");
    ~Server();

    [[noreturn]] void listenForConnections();

    std::string getName() const;

private:
    void _init();
    void _setServerSocketConnectionParams();

    size_t m_BufferSize; // should match the client buffer size
    int m_PortNumber; // port should match the client one and should have four numeric digits to avoid binding errors
    std::vector<int> m_Data;
    std::string m_Name;
    char* m_Buffer;
    int m_ServerFileDescriptor;
};

#endif // SERVER_H
