#include <iostream>
#include "parse.h"

using namespace std;

int main(int argc, char *argv[])
{
    parse ps(argc,argv);
    ps.reading_parsing_file();
    return 0;
}
