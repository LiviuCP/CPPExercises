#ifndef CLIENT_H
#define CLIENT_H

#include <cstdlib>
#include <string>
#include <vector>

class Client
{
public:
    explicit Client();
    explicit Client(size_t bufferSize, int portNumber, std::string m_IpAddress, const std::string &name = "");
    ~Client();

    void retrieveDataFromServer(size_t requestedNrOfElements);

    void setIpAddress(const std::string& ipAddress);
    std::string getIpAddress() const;

    std::vector<int> getData() const;
    std::string getName() const;

private:
    void _init();
    void _setFileDescriptor();
    void _establishConnectionToServer();

    size_t m_BufferSize; // should match the server buffer size
    int m_PortNumber; // port should match the server one and should have four numeric digits to avoid binding errors
    std::string m_IpAddress;
    std::string m_Name;
    char* m_Buffer;
    int m_FileDescriptor;
    std::vector<int> m_Data;
    };

#endif // CLIENT_H
