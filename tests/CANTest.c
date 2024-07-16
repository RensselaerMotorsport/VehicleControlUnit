#include <stdio.h>
#include "../includes/DBCParser.h"

int main() {
    DBC dbc;
    parse_dbc_file("files/test.dbc", &dbc);
    print_dbc(&dbc);

    return 0;
}
