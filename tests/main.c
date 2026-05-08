#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#include "utest.h"
#include "garment.h"

UTEST_STATE();

int main(int argc, const char *const argv[])
{
    // Setup global state
    assert(loadGarments());
    
    return utest_main(argc, argv);
}
