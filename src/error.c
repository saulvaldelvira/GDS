#include "error.h"

const char* gds_get_error_msg(int error_code) {
        switch (error_code){
                case GDS_INDEX_BOUNDS_ERROR:
                        return "Index out ouf bounds";
                case GDS_ELEMENT_NOT_FOUND_ERROR:
                        return "Element not found";
                case GDS_REPEATED_ELEMENT_ERROR:
                        return "Repeated element";
                case GDS_INVALID_PARAMETER_ERROR:
                        return "Invalid parameter";
                default:
                        return "Unknown error code";
        }
}
