#include <cassert>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <iostream>

#include "server.h"

static constexpr size_t c_DefaultBufferSize{512};
static constexpr int c_DefaultPortNumber{5000};
static constexpr size_t c_MaxConnections{4};
static constexpr size_t c_NrOfClientRequests{2}; // number of client requests per connection (one for knowing maximum number of available elements and one for the actual elements)
static const std::vector<int> c_DefaultServerData{1, 5, 2, -1, 4, -10, 6, 8, 9};

Server::Server()
    : m_BufferSize{c_DefaultBufferSize + 1}
    , m_PortNumber{c_DefaultPortNumber}
    , m_Concurrent{false}
    , m_Data{c_DefaultServerData}
    , m_Name{}
    , m_Buffer{nullptr}
    , m_ServerFileDescriptor{-1}
{
    _init();
}

Server::Server(size_t bufferSize, int portNumber, bool concurrent, const std::vector<int>& serverData, const std::string& name)
    : m_BufferSize{bufferSize + 1}
    , m_PortNumber{portNumber}
    , m_Concurrent{concurrent}
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
    std::clog << "SERVER " << m_Name << ": listening on port " << m_PortNumber << " for clients..." << std::endl << std::endl;

    for (;;)
    {
        struct sockaddr_in clientAddress;
        unsigned int len = sizeof(clientAddress);

        int clientFileDescriptor{accept(m_ServerFileDescriptor, reinterpret_cast<sockaddr*>(&clientAddress), &len)};
        if (clientFileDescriptor < 0)
        {
            std::cerr << "SERVER " << m_Name << ": Connection accept error" << std::endl;
            perror("");
            continue;
        }

        if (m_Concurrent)
        {
            int processId{fork()};

            if (processId == 0)
            {
                close(m_ServerFileDescriptor);
                _processClientRequest(clientFileDescriptor);
                exit(0);
            }
            else
            {
                close(clientFileDescriptor);
            }
        }
        else
        {
            _processClientRequest(clientFileDescriptor);
            close(clientFileDescriptor);
        }
    }
}

std::string Server::getName() const
{
    return m_Name;
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
            std::cerr << "SERVER " << m_Name << ": Error when creating socket file descriptor" << std::endl;
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
        std::cerr << "SERVER " << m_Name << ": Error when binding socket address" << std::endl;
        perror("");
        exit(-1);
    }

    if (listen(m_ServerFileDescriptor, c_MaxConnections) < 0)
    {
        std::cerr << "SERVER " << m_Name << ": Connection listening error" << std::endl;
        perror("");
        exit(-1);
    }
}

void Server::_processClientRequest(int clientFileDescriptor)
{
    size_t* const reqElemCountAddress{reinterpret_cast<size_t*>(m_Buffer)};

    for (size_t requestNr = 0; requestNr < c_NrOfClientRequests; ++requestNr)
    {
        memset(m_Buffer, '\0', m_BufferSize);

        std::clog << "SERVER " << m_Name << ": Client request received. Reading client request data..." << std::endl;
        ssize_t count{read(clientFileDescriptor, m_Buffer, m_BufferSize)};

        if (count >= static_cast<ssize_t>(sizeof(size_t)))
        {
            if (*reqElemCountAddress > 0)
            {
                const size_t nrOfElementsToSend{std::min(*reqElemCountAddress, m_Data.size())};
                std::clog << "SERVER " << m_Name << ": Client will get first " << static_cast<int>(nrOfElementsToSend) << " elements from list" << std::endl;

                for (size_t index = 0; index < nrOfElementsToSend; ++index)
                {
                    std::clog << "SERVER " << m_Name << ": Writing element " << static_cast<int>(m_Data[index]) << " into buffer" << std::endl;
                    *((reinterpret_cast<int*>(m_Buffer)) + index) = m_Data[index];
                }
            }
            else
            {
                std::clog << "SERVER " << m_Name << ": Client requested to know how many elements are available. Providing number to client: " << static_cast<int>(m_Data.size()) << std::endl;
                *reqElemCountAddress = m_Data.size();
            }

            std::clog << "SERVER " << m_Name << ": Sending requested data to client..." << std::endl;
            sleep(2); // for simulating a longer operation
            write(clientFileDescriptor, m_Buffer, m_BufferSize);
            std::clog << "SERVER " << m_Name << ": Done" << std::endl << std::endl;
        }
    }
}
