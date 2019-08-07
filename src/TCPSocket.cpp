#include "TCPSocket.hpp"
#include <iostream>

namespace MiniSocket {

using std::char_traits;
using std::basic_streambuf;
using std::iostream;

template <class CharT, class Traits = char_traits<CharT> >
class SocketStreamBuffer : public basic_streambuf<CharT, Traits> {
public:
    typedef typename Traits::int_type                 int_type;

    SocketStreamBuffer(TCPSocket *sock): sock_(sock) 
    {
        this->setg(inBuffer_, inBuffer_ + sizeof(inBuffer_),
                inBuffer_ + sizeof(inBuffer_));
        this->setp(outBuffer_, outBuffer_ + sizeof(outBuffer_));
    }

protected:
    int_type overflow(int_type c = Traits::eof()) 
    {
        this->sync();

        if (c != Traits::eof()) {
            this->sputc(Traits::to_char_type(c));
        }

        return 0;
    }

    int sync() 
    {
        sock_->sendAll(outBuffer_, (this->pptr() - outBuffer_) * sizeof(CharT));
        this->setp(outBuffer_, outBuffer_ + sizeof(outBuffer_));
        return 0;
    }

    int_type underflow() 
    {
        size_t len = sock_->recv(inBuffer_, sizeof(inBuffer_) * sizeof(CharT));

        if (len == 0) {
            return Traits::eof();
        }

        this->setg(inBuffer_, inBuffer_, inBuffer_ + len / sizeof(CharT));
        return this->sgetc();
    }

private:
    TCPSocket *sock_;

    CharT inBuffer_[1024];
    CharT outBuffer_[1024];
};

TCPSocket::TCPSocket(SOCKET sockDesc)
{
    sockDesc_ = sockDesc;
}

TCPSocket::TCPSocket(const SocketAddress &foreignAddress)
{
    int domain = foreignAddress.getSockaddr()->sa_family;
    createSocket(domain, SOCK_STREAM, 0);
    connect(foreignAddress);
}

void TCPSocket::sendAll(const char *buffer, int bufferLen)
{
	auto ptr = (const char *) buffer;
	auto nleft = bufferLen;
    int nwritten = 0;
	while (nleft > 0) {
		nwritten = send(ptr, nleft);
		nleft -= nwritten;
		ptr   += nwritten;
	}
}

iostream &TCPSocket::getStream()
{
    if (myStream_ == NULL) {
        myStreambuf_ = new SocketStreamBuffer<char>(this);
        myStream_ = new iostream(myStreambuf_);
    }
    return *myStream_;
}

}   // namesapce MiniSocket
