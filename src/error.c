#include "error.h"
#include "error_priv.h"

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

_Thread_local
static gds_return_t last_error;

__inline
gds_return_t gds_last_error(void) {
        return last_error;
}

__inline
const char* gds_last_error_msg(void) {
        return gds_get_error_msg(last_error);
}

__inline
void register_error(gds_return_t e) {
        last_error = e;
}

