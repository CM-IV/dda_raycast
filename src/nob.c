#define NOB_IMPLEMENTATION
#include "../include/nob.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd,
        "cc",
        "-Wall", "-Wextra", "-Werror", "-Wpedantic",
        "-o", "main", "src/main.c",
        "-I../include",
        "-L./lib",
        "-lraylib",
        "-lGL", "-lm", "-lpthread", "-ldl"
    );
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
