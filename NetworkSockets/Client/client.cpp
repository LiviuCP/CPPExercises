#include <cassert>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "client.h"

static constexpr size_t c_DefaultBufferSize{512};
static constexpr int c_DefaultPortNumber{5000};
static const std::string c_InternalLoopbackIpAddress{"127.0.0.1"};
static const size_t c_AvailabilityRequestCode = 0; // used for querying server about the maximum number of retrievable elements

Client::Client()
    : m_BufferSize{c_DefaultBufferSize + 1}
    , m_PortNumber{c_DefaultPortNumber}
    , m_IpAddress{c_InternalLoopbackIpAddress}
    , m_Name{}
    , m_Buffer{nullptr}
    , m_FileDescriptor{-1}
    , m_Data{}
    {
    _init();
}

Client::Client(size_t bufferSize, int portNumber, std::string ipAddress, const std::string& name)
    : m_BufferSize{bufferSize + 1}
    , m_PortNumber{portNumber}
    , m_IpAddress{ipAddress}
    , m_Name{name}
    , m_Buffer{nullptr}
    , m_FileDescriptor{-1}
    , m_Data{}
{
    assert(m_BufferSize > 0 && "No buffer allocated");
    assert(m_PortNumber > 0 && "Invalid port number");
    assert(m_IpAddress.size() > 0 && "Empty IP address parameter");

    _init();
}

Client::~Client()
{
    delete []m_Buffer;
    m_Buffer = nullptr;

    if (m_FileDescriptor >= 0)
    {
        close(m_FileDescriptor);
    }
}

void Client::retrieveDataFromServer(size_t requestedNrOfElements)
{
    if (requestedNrOfElements > 0)
    {
        printf("\nCLIENT %s: Connecting to server...\n", m_Name.c_str());
        _establishConnectionToServer();
        printf("CLIENT %s: Connection established\n\n", m_Name.c_str());

        printf("CLIENT %s: Checking data availability\n", m_Name.c_str());

        size_t availableCount{0}, actuallyRequestedCount{0};
        sleep(1);

        if (write(m_FileDescriptor, &c_AvailabilityRequestCode, sizeof(size_t)))
        {
            memset(m_Buffer, '\0', m_BufferSize);

            size_t* startAddress = reinterpret_cast<size_t*>(m_Buffer);

            if (read(m_FileDescriptor, m_Buffer, m_BufferSize))
            {
                availableCount = *startAddress;
            }
        }

        printf("CLIENT %s: Done\n\n", m_Name.c_str());

        if (availableCount == 0)
        {
            printf("CLIENT %s: No data is available.\n", m_Name.c_str());
        }
        else
        {
            if (requestedNrOfElements > availableCount)
            {
                printf("CLIENT %s: More data requested than available. Number of elements will be limited to %d.\n\n", m_Name.c_str(), static_cast<int>(availableCount));
            }

            actuallyRequestedCount = std::min(requestedNrOfElements, availableCount);

            printf("CLIENT %s: Requesting elements from server\n\n", m_Name.c_str());
            if (write(m_FileDescriptor, &actuallyRequestedCount, sizeof(size_t)))
            {
                memset(m_Buffer, '\0', m_BufferSize);

                int* startAddress{reinterpret_cast<int*>(m_Buffer)};

                if (read(m_FileDescriptor, m_Buffer, m_BufferSize))
                {
                    sleep(1);
                    printf("CLIENT %s: Response received from server\n\n", m_Name.c_str());
                    sleep(1);
                    printf("CLIENT %s: Storing received elements\n", m_Name.c_str());
                    sleep(1);
                    for (size_t index{0}; index < actuallyRequestedCount; ++index)
                    {
                        m_Data.push_back(*(startAddress + index));
                        printf("CLIENT %s: Added element with value: %d\n", m_Name.c_str(), *(startAddress + index));
                    }
                }
                else
                {
                    fprintf(stderr, "CLIENT %s: The request could not be completed", m_Name.c_str());
                }
            }
            else
            {
                fprintf(stderr, "CLIENT %s: The request could not be completed\n", m_Name.c_str());
            }
        }

        close(m_FileDescriptor);
        m_FileDescriptor = -1; // signal to destructor that the file descriptor has already been closed
    }
}

void Client::setIpAddress(const std::string& ipAddress)
{
    m_IpAddress = ipAddress;
}

std::string Client::getIpAddress() const
{
    return m_IpAddress;
}

std::vector<int> Client::getData() const
{
    return m_Data;
}

std::string Client::getName() const
{
    return m_Name;
}

void Client::_init()
{
    if (!m_Buffer)
    {
        m_Buffer = new char[m_BufferSize];
    }

    if (m_FileDescriptor < 0)
    {
        _setFileDescriptor();
    }
}

void Client::_setFileDescriptor()
{
    m_FileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (m_FileDescriptor < 0)
    {
        fprintf(stderr, "Client %s: Error when creating socket file descriptor\n", m_Name.c_str());
        perror("");
        exit(-1);
    }
}

void Client::_establishConnectionToServer()
{
    sleep(1);
    if (m_FileDescriptor < 0)
    {
        _setFileDescriptor();
    }

    sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(static_cast<uint16_t>(m_PortNumber));

    if(inet_pton(AF_INET, m_IpAddress.c_str(), &socketAddress.sin_addr)<=0)
    {
        fprintf(stderr, "CLIENT %s: IP address error\n", m_Name.c_str());
        exit(-1);
    }

    if (connect(m_FileDescriptor, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0)
    {
        fprintf(stderr, "CLIENT %s: Connection error\n", m_Name.c_str());
        exit(-1);
    }
}
