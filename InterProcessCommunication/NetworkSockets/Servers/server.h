#pragma once

#include <cstdlib>
#include <string>
#include <vector>

class Server
{
public:
    explicit Server();
    explicit Server(size_t bufferSize, int portNumber, bool concurrent, const std::vector<int>& serverData,
                    const std::string& name = "");
    ~Server();

    [[noreturn]] void listenForConnections();

    std::string getName() const;

private:
    void _init();
    void _setServerSocketConnectionParams();
    void _processClientRequest(int clientFileDescriptor);

    size_t m_BufferSize; // should match the client buffer size
    int m_PortNumber;    // port should match the client one and should have four numeric digits to avoid binding errors
    bool m_Concurrent;   // handle multiple client connections simultaneously (fork process)
    std::vector<int> m_Data;
    std::string m_Name;
    char* m_Buffer;
    int m_ServerFileDescriptor;
};
