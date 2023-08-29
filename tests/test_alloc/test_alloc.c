
#include "test_alloc.h"

static const Efl_Test_Case etc[] = {
  { "Alloc", mem_malloc_test },
  { "Calloc", mem_calloc_test },
  { NULL, NULL }
};

SUITE_INIT(elm) {
}

SUITE_SHUTDOWN(elm) {
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Memory allocation", etc, SUITE_INIT_FN(elm), SUITE_SHUTDOWN_FN(elm));

   return (failed_count == 0) ? 0 : 255;
}
