#include "options.h"
#include "shell.h"

int main(int argc, char *argv[])
{
    struct sh_data data;
    data.options = parse_options(argc, argv);

    setup_shell(&data);

    int rc = run_shell(&data);

    end_shell(&data);

    return rc;
}
