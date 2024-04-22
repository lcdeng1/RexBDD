#include "defines.h"
#include "error.h"

const char* REXBDD::error::getName() const
{
    switch (errCode) {
        case  ErrCode::UNINITIALIZED:         return "Uninitialized";
        case  ErrCode::ALREADY_INITIALIZED:   return "Already initialized";
        case  ErrCode::NOT_IMPLEMENTED:       return "Not implemented";
        case  ErrCode::INSUFFICIENT_MEMORY:   return "Insufficient memory";
        case  ErrCode::INVALID_OPERATION:     return "Invalid operation";
        case  ErrCode::INVALID_VARIABLE:      return "Invalid variable";
        case  ErrCode::INVALID_LEVEL:         return "Invalid level";
        case  ErrCode::INVALID_BOUND:         return "Invalid bound";
        case  ErrCode::DOMAIN_NOT_EMPTY:      return "Domain not empty";
        case  ErrCode::UNKNOWN_OPERATION:     return "Unknown operation";
        case  ErrCode::DOMAIN_MISMATCH:       return "Domain mismatch";
        case  ErrCode::FOREST_MISMATCH:       return "Forest mismatch";
        case  ErrCode::TYPE_MISMATCH:         return "Type mismatch";
        case  ErrCode::WRONG_NUMBER:          return "Wrong number";
        case  ErrCode::VALUE_OVERFLOW:        return "Overflow";
        case  ErrCode::DIVIDE_BY_ZERO:        return "Divide by zero";
        case  ErrCode::INVALID_POLICY:        return "Invalid policy";
        case  ErrCode::INVALID_ASSIGNMENT:    return "Invalid assignment";
        case  ErrCode::INVALID_ARGUMENT:      return "Invalid argument";
        case  ErrCode::INVALID_FILE:          return "Invalid file";
        case  ErrCode::COULDNT_WRITE:         return "Couldn't write to file";
        case  ErrCode::COULDNT_READ:          return "Couldn't read from file";
        case  ErrCode::MISCELLANEOUS:         return "Miscellaneous";
        default:                              return "Unknown error";
    }
}