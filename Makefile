CC=gcc
CFLAGS=-O2 -DH5_USE_16_API

cooling: main.o hdf5_wrapper.o
	$(CC) $(CFLAGS) -o $@ main.o hdf5_wrapper.o  -I. -lhdf5

clean:
	rm -f cooling *.o *~