#include "util/error.h"

const char* gds_get_error_msg(int error_code) {
        switch (error_code){
                case INDEX_BOUNDS_ERROR:
                        return "Index out ouf bounds";
                case ELEMENT_NOT_FOUND_ERROR:
                        return "Element not found";
                case REPEATED_ELEMENT_ERROR:
                        return "Repeated element";
                case INVALID_PARAMETER_ERROR:
                        return "Invalid parameter";
                default:
                        return "Unknown error code";
        }
}
