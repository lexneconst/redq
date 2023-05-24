
PROJECT_NAME=red queen
PROJECT_VERSION=1.10


PROJECT_DIR=/mnt/d/projects
DFLAGS=-c -Wall -fpic
CFLAGS=-DLINUX_H=1 

LIBIPROJ=libicore.so
LIBGPROJ=libgcore.so
LIBRPROJ=librmsv.so
LIBMPROJ=libmicrop.so
LIBCR0= -licore -lgcore -lmicrop -lrmsv
LIBCRT= -lrmsv -pthread

PROJECT=redq

CC=gcc

export CC
export PROJECT_DIR
export DFLAGS
export CFLAGS

all: librmsv libicore libgcore redq


clean:
	rm -f  *.o *.a *.so $(PROJECT)
	rm -f icore/*.o 
	rm -f gcore/*.o
	rm -f rmsv/*.o
	rm -f microp/*.o
	rm -f redqueen/*.o
	
OBJLIBRMSV=rmsv/dllmain.o\
	rmsv/libcr.o \
	rmsv/erro.o \
	rmsv/mm.o	
rmsv/dllmain.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) -o $@ rmsv/dllmain.c
rmsv/libcr.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) -o $@ rmsv/libcr.c
rmsv/erro.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) -o $@ rmsv/erro.c
rmsv/mm.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) -o $@ rmsv/mm.c
librmsv: $(OBJLIBRMSV)
	$(CC) -shared $(OBJLIBRMSV) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) $(CFLAGS) -o $(LIBRPROJ)
	
OBJLIBICORE=icore/dllmain.o\
	icore/icore.o \
	icore/ctime.o
icore/dllmain.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ icore/dllmain.c
icore/icore.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ icore/icore.c
icore/ctime.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ icore/ctime.c
libicore: $(OBJLIBICORE)
	$(CC) -shared $(OBJLIBICORE) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) $(CFLAGS) $(LIBCRT) -o $(LIBIPROJ)
	
OBJLIBGCORE=gcore/dllmain.o\
	gcore/gcore.o	
gcore/dllmain.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ gcore/dllmain.c
gcore/gcore.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ gcore/gcore.c
libgcore: $(OBJLIBGCORE)
	$(CC) -shared $(OBJLIBGCORE) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) $(CFLAGS) $(LIBCRT) -o $(LIBGPROJ)

OBJLIBMCORE=microp/dllmain.o\
	microp/microp.o	
microp/dllmain.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ microp/dllmain.c
microp/microp.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(DFLAGS) $(LIBCRT) -o $@ microp/microp.c
libmicrop: $(OBJLIBMCORE)
	$(CC) -shared $(OBJLIBMCORE) -I$(PROJECT_DIR)/include -L$(PROJECT_DIR) $(CFLAGS) $(LIBCRT) -o $(LIBMPROJ)
	
PROJCORE=icore/main.o
icore/main.o:
	$(CC) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) -c -o $@ redqueen/main.c
redq: $(PROJCORE)
	$(CC) $(PROJCORE) -I"$(PROJECT_DIR)/include" -L"$(PROJECT_DIR)" $(CFLAGS) $(LIBCR0) -o $(PROJECT)
	
	
exp:
	@export LD_LIBRARY_PATH=$(PROJECT_DIR):$LD_LIBRARY_PATH
	@echo $(LD_LIBRARY_PATH)
	

run:
	./$(PROJECT)