#include <arpa/inet.h>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "server.h"

static constexpr size_t c_DefaultBufferSize{512};
static constexpr int c_DefaultPortNumber{5000};
static constexpr size_t c_MaxConnections{4};
static constexpr size_t c_NrOfClientRequests{2}; // number of client requests per connection (one for knowing maximum
                                                 // number of available elements and one for the actual elements)
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

Server::Server(size_t bufferSize, int portNumber, bool concurrent, const std::vector<int>& serverData,
               const std::string& name)
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
    delete[] m_Buffer;
    m_Buffer = nullptr;

    if (m_ServerFileDescriptor >= 0)
    {
        close(m_ServerFileDescriptor);
        m_ServerFileDescriptor = -1;
    }
}

void Server::listenForConnections()
{
    std::clog << "SERVER " << m_Name << ": listening on port " << m_PortNumber << " for clients..." << std::endl;

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

        int socketOption{1};

        // prevent "address-already-in-use" error
        if (setsockopt(m_ServerFileDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption,
                       sizeof(socketOption)))
        {
            std::cerr << "SERVER " << m_Name << ": Error when setting socket option for reusing address and port"
                      << std::endl;
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
    size_t* const pRequestedElementsCount{
        reinterpret_cast<size_t*>(m_Buffer)}; // number of elements requested by client (if 0 then the server should
                                              // send back the maximum available number of elements)
    char* const pClientName{m_Buffer +
                            sizeof(size_t)}; // optional information sent by client that helps identifying the client
                                             // for a concurrent client - server model (actually the client name)

    for (size_t requestNr = 0; requestNr < c_NrOfClientRequests; ++requestNr)
    {
        memset(m_Buffer, '\0', m_BufferSize);

        ssize_t count{read(clientFileDescriptor, m_Buffer, m_BufferSize)};
        std::string clientName{pClientName};
        std::clog << "SERVER " << m_Name << ": Client request received. Client name: " << clientName << std::endl;

        size_t bytesToSend{0};

        if (count >= static_cast<ssize_t>(
                         sizeof(size_t))) // there should be at least size_t bytes (number of requested elements)
        {
            size_t requestedElementsCount{*pRequestedElementsCount};
            memset(m_Buffer, '\0', m_BufferSize);

            if (requestedElementsCount > 0)
            {
                const size_t nrOfElementsToSend{std::min(requestedElementsCount, m_Data.size())};
                std::clog << "SERVER " << m_Name << ": Client " << clientName << " requested "
                          << static_cast<int>(nrOfElementsToSend) << " elements" << std::endl;

                for (size_t index = 0; index < nrOfElementsToSend; ++index)
                {
                    std::clog << "SERVER " << m_Name << ": Adding element " << static_cast<int>(m_Data[index])
                              << " to buffer for sending to client " << clientName << std::endl;
                    usleep(1000);
                    *((reinterpret_cast<int*>(m_Buffer)) + index) = m_Data[index];
                    bytesToSend += sizeof(int);
                }
            }
            else
            {
                std::clog << "SERVER " << m_Name << ": Client " << clientName
                          << " requested to know how many elements are available. The number is: "
                          << static_cast<int>(m_Data.size()) << std::endl;
                *pRequestedElementsCount = m_Data.size();
                bytesToSend += sizeof(size_t);
            }

            // add a NULL terminating character (\0) to the sent byte stream
            ++bytesToSend;

            std::clog << "SERVER " << m_Name << ": Sending requested data to client " << clientName << " ..."
                      << std::endl;
            sleep(1); // for simulating a longer operation
            write(clientFileDescriptor, m_Buffer, bytesToSend);
            std::clog << "SERVER " << m_Name << ": " << bytesToSend << " bytes sent to client " << clientName
                      << std::endl;
        }
    }
}
