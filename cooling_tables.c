#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <hdf5.h>
#include "allvars.h"

void read_headers(int *n_h_bins_N, int *temp_bins_N, int *he_frac_bins_N){

  hid_t file_id, dataset_id; /*these will be used for reading the hdf5 files*/
  herr_t status; /*tells us if we have an error with reading hdf5 files*/
  char fname[500], set_name[500];

  sprintf(fname, "%s%s",PATH, FILE);
  printf("Cooling table file is: %s\n",fname); /*just a check*/
  file_id = H5Fopen(fname,H5F_ACC_RDONLY,H5P_DEFAULT); /*this is giving us a file_identifier*/

  /*now we need to get the header file to know the size of our arrays*/

  dataset_id = H5Dopen(file_id, "/Header/Number_of_density_bins");
  status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &n_h_bins_N);
  status = H5Dclose(dataset_id);

  dataset_id = H5Dopen(file_id, "/Header/Number_of_temperature_bins");
  status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &temp_bins_N);
  status = H5Dclose(dataset_id);

  dataset_id = H5Dopen(file_id, "/Header/Number_of_helium_fractions");
  status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &he_frac_bins_N);
  status = H5Dclose(dataset_id);
  
  printf("Number of hydrogen density bins is: %d\n", n_h_bins_N);
  printf("Number of temperature bins is: %d\n", temp_bins_N);
  printf("Number of helium fraction bins is: %d\n", he_frac_bins_N);
  
}

void read_bins(float *n_h_bins,float *temp_bins,float *he_frac_bins){

  hid_t file_id, dataset_id; /*these will be used for reading the hdf5 files*/
  herr_t status; /*tells us if we have an error with reading hdf5 files*/
  char fname[500], set_name[500];
 
  /*Now we make our arrays of hydrogen_density, temperature, and helium fractions*/
  
  float n_h_bins[n_h_bins_N], temp_bins[temp_bins_N], he_frac_bins[he_frac_bins_N];
  
  sprintf(set_name,"/Metal_free/Hydrogen_density_bins"); 
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name);
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &n_h_bins); 
  status = H5Dclose(dataset_id);
  
  sprintf(set_name,"/Metal_free/Temperature_bins");
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name);
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &temp_bins); 
  status = H5Dclose(dataset_id);

  sprintf(set_name,"/Metal_free/Helium_mass_fraction_bins");
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name);
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &he_frac_bins); 
  status = H5Dclose(dataset_id);

}

void read_cooling_table(float ***net_cooling_rate){

  float net_cooling_rate[n_h_bins_N][temp_bins_N][he_frac_bins_N]; /*this is the 3-d array we will
								  read data into*/
  sprintf(set_name,"/Metal_free/Net_Cooling"); /*we are considering metal-free cooling*/
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name); /*gives us an identifier for the dataset within the file*/
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &net_cooling_rate); /*here we read the d
													ata into a 3d array*/
  status = H5Dclose(dataset_id);
  status = H5Fclose(file_id);
  
  printf("TEST: N_H IS: %.5e\n TEMPERATURE IS: %.5e\n HE_FRAC IS: %.5e\n LAMBDA IS %.5e\n",n_h_bins[20],temp_bins[100],he_frac_bins[5], net_cooling_rate[20][100][5]);
  }


  
