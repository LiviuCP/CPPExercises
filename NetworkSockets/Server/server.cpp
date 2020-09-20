#include "server.h"
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

static constexpr size_t c_DefaultBufferSize{512};
static constexpr int c_DefaultPortNumber{5000};
static constexpr size_t c_MaxConnections{4};
static constexpr size_t c_NrOfClientRequests{2}; // number of client requests per connection (one for knowing maximum number of available elements and one for the actual elements)
static const std::vector<int> c_DefaultServerData{1, 5, 2, -1, 4, -10, 6, 8, 9};

Server::Server()
    : m_BufferSize{c_DefaultBufferSize + 1}
    , m_PortNumber{c_DefaultPortNumber}
    , m_Data{c_DefaultServerData}
    , m_Name{}
    , m_Buffer{nullptr}
    , m_ServerFileDescriptor{-1}
{
    _init();
}

Server::Server(size_t bufferSize, int portNumber, const std::vector<int>& serverData, const std::string& name)
    : m_BufferSize{bufferSize + 1}
    , m_PortNumber{portNumber}
    , m_Data{serverData}
    , m_Name{name}
    , m_Buffer{nullptr}
    , m_ServerFileDescriptor{-1}
{
    assert(m_BufferSize > 0 && "No buffer allocated");
    assert(m_PortNumber > 0 && "Invalid port number");

    _init();
}

Server::~Server()
{
    delete []m_Buffer;
    m_Buffer = nullptr;

    if (m_ServerFileDescriptor >= 0)
    {
        close(m_ServerFileDescriptor);
        m_ServerFileDescriptor = -1;
    }
}

void Server::listenForConnections()
{
    printf("SERVER %s: listening on port %d for clients...\n\n", m_Name.c_str(), m_PortNumber);

    for (;;)
    {
        struct sockaddr_in clientAddress;
        unsigned int len = sizeof(clientAddress);

        int clientFileDescriptor{accept(m_ServerFileDescriptor, reinterpret_cast<sockaddr*>(&clientAddress), &len)};
        if (clientFileDescriptor < 0)
        {
            fprintf(stderr, "SERVER %s: Connection accept error\n", m_Name.c_str());
            perror("");
            continue;
        }

        size_t* const reqElemCountAddress{reinterpret_cast<size_t*>(m_Buffer)};

        for (size_t requestNr = 0; requestNr < c_NrOfClientRequests; ++requestNr)
        {
            memset(m_Buffer, '\0', m_BufferSize);

            printf("SERVER %s: Client request received. Reading client request data...\n", m_Name.c_str());
            ssize_t count{read(clientFileDescriptor, m_Buffer, m_BufferSize)};

            if (count >= static_cast<ssize_t>(sizeof(size_t)))
            {
                if (*reqElemCountAddress > 0)
                {
                    const size_t nrOfElementsToSend{std::min(*reqElemCountAddress, m_Data.size())};
                    printf("SERVER %s: Client will get first %d elements from list\n", m_Name.c_str(), static_cast<int>(nrOfElementsToSend));

                    for (size_t index = 0; index < nrOfElementsToSend; ++index)
                    {
                        printf("SERVER %s: Writing element %d into buffer\n", m_Name.c_str(), static_cast<int>(m_Data[index]));
                        *((reinterpret_cast<int*>(m_Buffer)) + index) = m_Data[index];
                    }
                }
                else
                {
                    printf("SERVER %s: Client requested to know how many elements are available. Providing number to client: %d\n", m_Name.c_str(), static_cast<int>(m_Data.size()));
                    *reqElemCountAddress = m_Data.size();
                }

                printf("SERVER %s: Sending requested data to client...\n", m_Name.c_str());
                write(clientFileDescriptor, m_Buffer, m_BufferSize);
                printf("SERVER %s: Done\n\n", m_Name.c_str());
            }
        }

        close(clientFileDescriptor);
    }
}

void Server::_init()
{
    if (!m_Buffer)
    {
        m_Buffer = new char[m_BufferSize];
    }

    if (m_ServerFileDescriptor < 0)
    {
        m_ServerFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

        if (m_ServerFileDescriptor < 0)
        {
            fprintf(stderr, "SERVER %s:Error when creating socket file descriptor\n", m_Name.c_str());
            perror("");
            exit(-1);
        }
    }

    _setServerSocketConnectionParams();
}

void Server::_setServerSocketConnectionParams()
{
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(static_cast<uint16_t>(m_PortNumber));

    if (bind(m_ServerFileDescriptor, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0)
    {
        fprintf(stderr, "SERVER %s: Error when binding socket address\n", m_Name.c_str());
        perror("");
        exit(-1);
    }

    if (listen(m_ServerFileDescriptor, c_MaxConnections) < 0)
    {
        fprintf(stderr, "SERVER %s: Connection listening error\n", m_Name.c_str());
        perror("");
        exit(-1);
    }
}
