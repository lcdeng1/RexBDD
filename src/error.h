#ifndef REXBDD_ERROR_H
#define REXBDD_ERROR_H
#include <iostream>
#include <stdexcept>

namespace REXBDD {
    /// Error codes.
    typedef enum {
        /// The library was not initialized.
        UNINITIALIZED,
        /// The library was already initialized.
        ALREADY_INITIALIZED,
        /// The requested operation is not yet implemented!
        NOT_IMPLEMENTED,
        /// An operation failed due to lack of memory.
        INSUFFICIENT_MEMORY,
        /// An operation is not supported for the given forest.
        INVALID_OPERATION,
        /// A provided variable is erroneous.
        INVALID_VARIABLE,
        /// A provided level number is erroneous.
        INVALID_LEVEL,
        /// A provided variable bound is out of range.
        INVALID_BOUND,
        /// We expected an empty domain, but it wasn't
        DOMAIN_NOT_EMPTY,
        /// Unknown operation (bad operation handle).
        UNKNOWN_OPERATION,
        /// Requested operation requires same domains, they weren't.
        DOMAIN_MISMATCH,
        /// Requested operation requires same forest, it wasn't.
        FOREST_MISMATCH,
        /// Requested operation unsupported for operand or result type.
        TYPE_MISMATCH,
        /// Requested operation requires different number of operands.
        WRONG_NUMBER,
        /// A result won't fit in an integer / float.
        VALUE_OVERFLOW,
        /// Integer division by 0 is invalid.
        DIVIDE_BY_ZERO,
        /// Invalid policy setting.
        INVALID_POLICY,
        /// Bad value for something.
        INVALID_ASSIGNMENT,
        /// Invalid argument (for specialized operations)
        INVALID_ARGUMENT,
        /// File format error.
        INVALID_FILE,
        /// File input error.
        COULDNT_READ,
        /// File output error.
        COULDNT_WRITE,
        /// Miscellaneous error
        MISCELLANEOUS
    }ErrCode;
    
    class error;
};

/**
    Class for errors thrown by REXBDD.

 */
class REXBDD::error {
    public:
        error(ErrCode c, const char* fn=nullptr, int ln=0)
            :fileName(fn), lineNo(ln) {
                errCode = c;
            };

        // error(const std::string& message, const char* file, int line)
        //     : errMessage(message), fileName(file), lineNo(line) {}
        /// Return a human-readable error message
        const char* getName() const;

        // Override what() method to return error message
        const char* what() const{
            formattedMessage = "[RexBDD] ERROR!\t";
            formattedMessage += getName();
            formattedMessage += ": \n\tFile: \t";
            formattedMessage += fileName;
            formattedMessage += ": \n\tLine: \t";
            formattedMessage += std::to_string(lineNo);
            formattedMessage += "\n";

            return formattedMessage.c_str();
        }

        inline operator ErrCode() const        { return errCode; }
        inline ErrCode getCode() const         { return errCode; }
        inline const char* getFile() const  { return fileName; }
        inline unsigned getLine() const     { return lineNo; }

    private:
        ErrCode errCode;
        std::string errMessage;
        const char* fileName;
        int lineNo;
        mutable std::string formattedMessage;
};

#endif