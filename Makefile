
all: slicloader_installer.exe

slicloader_installer.o: slicloader_installer.c slicloader_installer.h
	$(CC) -c slicloader_installer.c

slicloader_installer.exe: slicloader_installer.o
	$(CC) -static -static-libgcc slicloader_installer.o -o slicloader_installer.exe

clean:
	rm slicloader_installer.o
	rm slicloader_installer.exe

