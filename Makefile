flags=-O2 -Wall -std=c23
# ldflags=-lbu
ldflags=

.PHONY: all clean

all: clean dp_virtual_machin

/mnt/c/Users/Dhruv\ Patel/dp_project/DpVirtualMachine/dputils/dputils.o:  /mnt/c/Users/Dhruv\ Patel/dp_project/DpVirtualMachine/dputils/dputils.c /mnt/c/Users/Dhruv\ Patel/dp_project/DpVirtualMachine/dputils/dputils.h
	$(MAKE) -C /mnt/c/Users/Dhruv\ Patel/dp_project/DpVirtualMachine/dputils

dp_virtual_machin: dp_virtual_machin.o /mnt/c/Users/Dhruv\ Patel/dp_project/DpVirtualMachine/dputils/dputils.o
	cc $(flags) $< -o $@ $(ldflags) /mnt/c/Users/Dhruv\ Patel/dp_project/DpVirtualMachine/dputils/dputils.o

dp_virtual_machin.o: dp_virtual_machin.c dp_virtual_machin.h
	cc $(flags) -c $<

clean:
	rm -f *.o dp_virtual_machin