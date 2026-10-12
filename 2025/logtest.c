#include <rlog.h>

int main() {
    lforder("./test/");
    lopen("mylog", "w");

    llwrite(lctx, "", "Это будет только в лог-файле");
    llinfo(lctx, "Лог info: %d", 42);
    llerror(lctx, "Лог error");

    llclose();
    return 0;
}