#include <iostream>
#include "parse.h"

using namespace std;

int main(int argc, char *argv[])
{
    parse ps(argc,argv);
    ps.file_open();
    return 0;
}
