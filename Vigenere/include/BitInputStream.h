#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <iostream>

typedef unsigned char byte;

using namespace std;

class BitInputStream
{
    private:
        byte buffer;
        int nBits;
        int bitsToRead;
        istream& in;

    public:
        BitInputStream(istream is) : in(is)
        {
            this->buffer = 0;
            this->nBits = 0;
            this->bitsToRead = 0;
        };

        virtual ~BitInputStream();

    protected:


};

#endif // BITINPUTSTREAM_H
