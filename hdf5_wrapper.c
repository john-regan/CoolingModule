#include "cooling_params.h"
#include "hdf5_wrapper.h"
#include <stdlib.h>
#include <hdf5.h>
/*! \file hdf5_wrapper.c
 *  \brief hdf5 wrapper of commonly used hdf5 read and write routines
 */
/*! read an attribute "attribute_name" of hdf5 type  "hdf5_type" from file handle 
 *  
 */

#ifdef LEAVE
herr_t attribute_read(char *attribute_name,  enum my_hdf5_types hdf5_type,
		      hid_t hdf5_file_handle, void *buf)
{
  hid_t hdf5_attribute_id, hdf5_dataspace, hdf5_datatype;
  herr_t hdf5_status = 0;

  //here we get an identifier for a datatype
  switch (hdf5_type)
    {
    case my_hdf5_int:
      hdf5_datatype = H5Tcopy(H5T_NATIVE_INT);
      break;
    case my_hdf5_long:
      hdf5_datatype = H5Tcopy(H5T_NATIVE_LONG);
      break;
    case my_hdf5_float:
      hdf5_datatype = H5Tcopy(H5T_NATIVE_FLOAT);
      break;
    case my_hdf5_double:
      hdf5_datatype = H5Tcopy(H5T_NATIVE_DOUBLE);
      break;
    default:
		exit(-1);
      break;
    }

  hdf5_dataspace = H5Screate(H5S_SCALAR);
  hdf5_attribute_id =
    H5Acreate(hdf5_file_handle, attribute_name, hdf5_datatype, hdf5_dataspace, H5P_DEFAULT); //here we 'create' an attribute
  switch (hdf5_type)
    {
    case my_hdf5_int:
      hdf5_status = H5Aread(hdf5_attribute_id, hdf5_datatype, (int *) buf);
      break;
    case my_hdf5_long:
      hdf5_status = H5Aread(hdf5_attribute_id, hdf5_datatype, (long *) buf);
      break;
    case my_hdf5_float:
      hdf5_status = H5Aread(hdf5_attribute_id, hdf5_datatype, (float *) buf);
      break;
    case my_hdf5_double:
      hdf5_status = H5Aread(hdf5_attribute_id, hdf5_datatype, (double *) buf);
      break;
    default:
		exit(-1);
      break;
    }

  H5Tclose(hdf5_datatype);
  H5Aclose(hdf5_attribute_id);
  H5Sclose(hdf5_dataspace);

  return hdf5_status;
}
#endif

herr_t dataset_read(char *dataset_name,  int hdf5_type,  hid_t hdf5_file_handle, void *buf)
{
  hid_t hdf5_dataset_id, hdf5_dataspace, hdf5_datatype;
  herr_t hdf5_status = 0;

  /* datatype = H5Tcopy(H5T_C_S1); */
  /* status = H5Tset_size(datatype, dataset_size); */
  hdf5_dataset_id = H5Dopen(hdf5_file_handle, dataset_name);
  if(hdf5_status){
  printf("Error with opening dataset\n");
  exit(-1);
  }


  switch (hdf5_type)
    {
    case INT:
		hdf5_status = H5Dread(hdf5_dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, (int *) buf);
      break;
    case LONG:
		hdf5_status = H5Dread(hdf5_dataset_id, H5T_NATIVE_LONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, (long *) buf);
      break;
    case FLOAT:
		hdf5_status = H5Dread(hdf5_dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, (float *) buf);
      break;
    case DOUBLE:
		hdf5_status = H5Dread(hdf5_dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, (double *) buf);
      break;
    default:
		printf("Not a valid data type in eagle_H5Aread()!\n");
      exit(-1);
      break;
    }
  if(hdf5_status){
    printf("Error with reading dataset");
    exit(-1);}

  hdf5_status = H5Dclose(hdf5_dataset_id);
  if(hdf5_status)
	 exit(-1);

  return hdf5_status;
}
