/*
 * Header file containing HDF5 wrapper function prototypes 
 */
#include <hdf5.h>

#define INT 0
#define LONG 1
#define FLOAT 2
#define DOUBLE 3

/*
 * Short description of what this function does (TODO)
 */

herr_t dataset_read(char *dataset_name,  int hdf5_type,  hid_t hdf5_file_handle, void *buf);
