#include "defines.h"
#include "error.h"

REXBDD::error::error(REXBDD::error::code c, const char* fn, unsigned ln)
{
    errCode = c;
    fileName = fn;
    lineNo = ln;
}

const char* REXBDD::error::getName() const
{
    switch (errCode) {
        case  error::UNINITIALIZED:         return "Uninitialized";
        case  error::ALREADY_INITIALIZED:   return "Already initialized";
        case  error::NOT_IMPLEMENTED:       return "Not implemented";
        case  error::INSUFFICIENT_MEMORY:   return "Insufficient memory";
        case  error::INVALID_OPERATION:     return "Invalid operation";
        case  error::INVALID_VARIABLE:      return "Invalid variable";
        case  error::INVALID_LEVEL:         return "Invalid level";
        case  error::INVALID_BOUND:         return "Invalid bound";
        case  error::DOMAIN_NOT_EMPTY:      return "Domain not empty";
        case  error::UNKNOWN_OPERATION:     return "Unknown operation";
        case  error::DOMAIN_MISMATCH:       return "Domain mismatch";
        case  error::FOREST_MISMATCH:       return "Forest mismatch";
        case  error::TYPE_MISMATCH:         return "Type mismatch";
        case  error::WRONG_NUMBER:          return "Wrong number";
        case  error::VALUE_OVERFLOW:        return "Overflow";
        case  error::DIVIDE_BY_ZERO:        return "Divide by zero";
        case  error::INVALID_POLICY:        return "Invalid policy";
        case  error::INVALID_ASSIGNMENT:    return "Invalid assignment";
        case  error::INVALID_ARGUMENT:      return "Invalid argument";
        case  error::INVALID_FILE:          return "Invalid file";
        case  error::COULDNT_WRITE:         return "Couldn't write to file";
        case  error::COULDNT_READ:          return "Couldn't read from file";
        case  error::MISCELLANEOUS:         return "Miscellaneous";
        default:                            return "Unknown error";
    }
}