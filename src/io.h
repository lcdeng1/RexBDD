#ifndef REXBDD_IO_H
#define REXBDD_IO_H

#include "defines.h"

#ifndef _REXBDD_WITHOUT_CSTDIO_
#include <cstdio>
#endif

#ifndef _REXBDD_WITHOUT_IOSTREAM_
#include <iostream>
#endif

namespace REXBDD {
    class Input;
    class FileInput;
    class IstreamInput;
    class Output;
    class FileOutput;
    class OstreamOutput;
}

//
// We are using our own I/O classes because we want to support both
// C++ streams and C FILE*s, and this seems to be the cleanest way
// to do that.  Also it allows users to (easily?) define their own
// I/O mechanisms.
//
//

// ******************************************************************
// *                                                                *
// *                          Input  class                          *
// *                                                                *
// ******************************************************************
/**
    Input class.
    Abstract base class for file input.
*/
class REXBDD::Input {
    public:
        Input();
        virtual ~Input();

        /**
            Return true if and only if the input stream has hit EOF.
        */
        virtual bool eof() const = 0;

        /**
            Read exactly one character from the input stream.
                @return   The character consumed, or EOF.
                @throws   an appropriate error
        */
        virtual int get_char() = 0;

        /**
            Put the last consumed character back on the stream.
            Does nothing if no character was consumed.
                @param  x   Character to put back.
                            x will be the last consumed character.
                @throws     an appropriate error
        */
        virtual void unget(char x) = 0;

        /**
            Read an integer in signed decimal notation.
            TBD - integer is terminated by whitespace, or any character?
                @return   The integer consumed
                @throws   an appropriate error
        */
        virtual long get_integer() = 0;

        /**
            Read a floating point value in signed decimal notation.
            TBD - value is terminated by whitespace, or any character?
                @return   The value consumed
                @throws   an appropriate error
        */
        virtual double get_real() = 0;

        /**
            Read raw bytes into a memory location.
                @param  bytes   Number of bytes requested
                @param  buffer  Pointer to store the bytes

                @return Number of bytes actually read
        */
        virtual size_t read(size_t bytes, unsigned char* buffer) = 0;


        /*
            Handy input stream manipulation
        */

        /**
            Consume whitespace (if any) from the input stream,
            including comments of the form #...\n
        */
        void stripWS();

        /**
            Consume a keyword from the input stream.
            If the keyword does not match the input stream,
            we throw an INVALID_FILE error.
        */
        void consumeKeyword(const char* keyword);

};  // end of Input class

// ******************************************************************
// *                                                                *
// *                          output class                          *
// *                                                                *
// ******************************************************************

/** Output class.
    Abstract base class.
*/
class REXBDD::Output {
    public:
        Output();
        virtual ~Output();

        /**
            Write exactly one character to the output stream.
                @param  x   Character to write
                @throws     An appropriate error
        */
        virtual void put(char x) = 0;

        /**
            Write a string to the output stream.
                @param  x   String to write
                @param  w   Width for formatting
                @throws     An appropriate error
        */
        virtual void put(std::string s, int w=0) = 0;

        /**
            Write a signed, decimal integer to the output stream.
                @param  x   Integer to write
                @param  w   Width for formatting
                @throws     An appropriate error
        */
        virtual void put(long x, int w=0) = 0;

        inline void put(int x, int w=0) {
            put(long(x), w);
        }

        /**
            Write an unsigned, decimal integer to the output stream.
                @param  x   Integer to write
                @param  w   Width for formatting
                @throws     An appropriate error
        */
        virtual void put(unsigned long x, int w=0) = 0;

        inline void put(unsigned x, int w=0) {
            put((unsigned long) x, w);
        }

        /**
            Write hex digits to the output stream.
                @param  x   Value to write
                @param  w   Width for formatting
                @throws     An appropriate error
        */
        virtual void put_hex(unsigned long x, int w=0) = 0;

        /**
            Write a signed, decimal, floating-point value
            to the output stream.
                @param  x   Value to write
                @param  w   Width
                @param  p   Precision
                @param  f   Format, either 'e', 'f', or 'g'
                            (for the style of printf)
                @throws     An appropriate error
        */
        virtual void put(double x, int w=0, int p=6, char f='g') = 0;

        /**
            Write raw bytes from a memory location.
                @param  bytes   Number of bytes in the buffer
                @param  buffer  Pointer to memory location

                @return Number of bytes actually written
        */
        virtual size_t write(size_t bytes, const unsigned char* buffer) = 0;

        /**
            Flush the output stream.
        */
        virtual void flush() = 0;

        /*
            Handy output stream manipulation
        */

        /**
            Write number of bytes, with units.
                @param  m       Number of bytes
                @param  human   If false, units will be "bytes".
                                If true, scale units so that output
                                is betwen one and 1000.
        */
        void put_mem(size_t m, bool human);

};  // end of output class

#endif