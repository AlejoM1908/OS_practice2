#include "lib/app/server.c"

int main(int argc, char const *argv[])
{
    setbuf(stdout, NULL);
    startServer();
    return 0;
}
