#include "test.h"
#include "../src/util/compare.h"

#define cmpi(a,b) compare_unsigned_int(&(unsigned int){a}, &(unsigned int){b})
#define cmpl(a,b) compare_unsigned_long(&(unsigned long){a}, &(unsigned long){b})

int main(void){
        test_start("compare.c");

        assert(cmpi(0,1) < 0);
        assert(cmpi(1,0) > 0);
        assert(cmpi(0,0) == 0);
        assert(cmpi(-1, 0) > 0);

        assert(cmpl(0,1) < 0);
        assert(cmpl(1,0) > 0);
        assert(cmpl(0,0) == 0);
        assert(cmpl(-1, 0) > 0);

        test_end();
}
