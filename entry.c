

/*
  Created by figurantpp on 25/01/2021.
*/


#include <stdlib.h>
#include "tests.h"


#include "restack/restack.h"

#include <cutest/CuTest.h>
#include <stdio.h>

int main(void)
{
    CuSuite *spec;
    CuSuite *suite;
    CuString *output;

    spec = get_restack_suite();

    output = CuStringNew();
    suite = CuSuiteNew();

    CuSuiteAddSuite(suite, spec);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    puts(output->buffer);

    return 0;
}
