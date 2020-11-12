#include <cassert>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include <iostream>
#include <thread>

#include "client.h"

static constexpr size_t c_DefaultBufferSize{512};
static constexpr int c_DefaultPortNumber{5000};
static const std::string c_InternalLoopbackIpAddress{"127.0.0.1"};

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

void Client::retrieveDataFromServer(size_t requiredElementsCount)
{
    if (requiredElementsCount > 0)
    {
        _logMessage("CLIENT :name: Connecting to server...");
        _establishConnectionToServer();
        _logMessage("CLIENT :name: Connection established");
        _logMessage("CLIENT :name: " + std::to_string(requiredElementsCount) + " elements required. Querying server for availability...");

        size_t availableElementsCount{0};
        const ssize_t c_NrOfBytesSentInFirstQuery{_requestDataFromServer(0)};

        if (c_NrOfBytesSentInFirstQuery > 0)
        {
            const ssize_t c_NrOfBytesReceivedInFirstQuery{_receiveDataFromServer()};

            if (static_cast<size_t>(c_NrOfBytesReceivedInFirstQuery) >= sizeof(size_t))
            {
                availableElementsCount = *(reinterpret_cast<size_t*>(m_Buffer));
            }
        }

        if (availableElementsCount > 0)
        {
            const size_t c_RequestedElementsCount{std::min(requiredElementsCount, availableElementsCount)}; // number of requested elements to be limited by maximum that are available at server side
            _logMessage("CLIENT :name: " + std::to_string(availableElementsCount) + " elements can be provided by server");
            _logMessage("CLIENT :name: Requesting " + std::to_string(static_cast<int>(c_RequestedElementsCount)) + " elements");
            const ssize_t c_NrOfBytesSentInSecondQuery{_requestDataFromServer(c_RequestedElementsCount)};

            if (c_NrOfBytesSentInSecondQuery > 0)
            {
                ssize_t c_NrOfBytesReceivedInSecondQuery{_receiveDataFromServer()};
                _logMessage("CLIENT :name: Received response from server (" + std::to_string(c_NrOfBytesReceivedInSecondQuery) + " bytes)");

                if (c_RequestedElementsCount * sizeof(int) <= static_cast<size_t>(c_NrOfBytesReceivedInSecondQuery)) // number of received bytes should match (or exceed) requested elements
                {
                    _storeReceivedData(c_RequestedElementsCount);
                }
                else
                {
                    _logMessage("CLIENT :name: The request could not be completed (no data or insufficient data received)", true);
                }
            }
        }
        else
        {
            _logMessage("CLIENT :name: No elements are available for download or the count info received from server is corrupt.", true);
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
        _logMessage("CLIENT :name: Error when creating socket file descriptor", true);
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
        _logMessage("CLIENT :name: IP address error", true);
        exit(-1);
    }

    if (connect(m_FileDescriptor, reinterpret_cast<sockaddr*>(&socketAddress), sizeof(socketAddress)) < 0)
    {
        _logMessage("CLIENT :name: Connection error", true);
        exit(-1);
    }
}

ssize_t Client::_requestDataFromServer(size_t nrOfRequestedElements)
{
    char* const pClientNameBufferPos{m_Buffer + sizeof(nrOfRequestedElements)};
    const size_t c_NrOfBytesToSend{sizeof(nrOfRequestedElements) + m_Name.size() + 1};

    memset(m_Buffer, '\0', m_BufferSize);
    *(reinterpret_cast<size_t*>(m_Buffer)) = nrOfRequestedElements;
    m_Name.copy(pClientNameBufferPos, m_Name.size(), 0);
    const ssize_t c_NrOfSentBytes{write(m_FileDescriptor, m_Buffer, c_NrOfBytesToSend)};

    return c_NrOfSentBytes;
}

ssize_t Client::_receiveDataFromServer()
{
    memset(m_Buffer, '\0', m_BufferSize);
    const ssize_t c_ReceivedBytesCount = read(m_FileDescriptor, m_Buffer, m_BufferSize);
    usleep(125000);

    return c_ReceivedBytesCount;
}

void Client::_storeReceivedData(size_t elementsCount)
{
    _logMessage("CLIENT :name: Storing received elements");

    for (size_t index{0}; index < elementsCount; ++index)
    {
        usleep(250000);
        m_Data.push_back(*(reinterpret_cast<int*>(m_Buffer) + index));
        _logMessage("CLIENT :name: Added element with value: " + std::to_string(*(reinterpret_cast<int*>(m_Buffer) + index)));
    }
}

void Client::_logMessage(const std::string& message, bool isErrorMessage)
{
    std::lock_guard<std::mutex> lock{m_LogMutex};
    const std::string c_ClientNamePlaceholder{":name"};
    std::string messageToLog{message};
    std::ostream& out{isErrorMessage ? std::cerr : std::clog};
    size_t clientNamePos{messageToLog.find(c_ClientNamePlaceholder, 0)};

    while (clientNamePos != std::string::npos)
    {
        messageToLog.replace(clientNamePos, c_ClientNamePlaceholder.size(), m_Name);
        clientNamePos = messageToLog.find(c_ClientNamePlaceholder, clientNamePos + m_Name.size());
    }

    out << messageToLog << "\n";
}
