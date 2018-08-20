exe_ntfs : parse.o main.o
	g++ -o exe_ntfs parse.o main.o

main.o : parse.o main.cpp
	g++ -c main.cpp
parse.o : ntfs_header.h parse.h parse.cpp
	g++ -c -o parse.o parse.cpp

clean : 
	rm -rf *.o
	rm -rf exe_ntfs	
