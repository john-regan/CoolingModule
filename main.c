#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "allvars.h"

int main(){
  
  /*First we read in data from the hdf5 files*/

  hid_t file_id; /*these will be used for reading the hdf5 files*/
  herr_t status; /*tells us if we have an error with reading hdf5 files*/
  char fname[500], set_name[500];

  sprintf(fname, "%s%s",PATH, FILENAME);
  printf("Cooling table file is: %s\n",fname); /*just a check*/
  file_id = H5Fopen(fname,H5F_ACC_RDONLY,H5P_DEFAULT); /*this is giving us a file_identifier*/

  /* check whether the tables have the expected size */
  //enum hdf5_type=my_hdf5_int;
  status=dataset_read("/Header/Number_of_temperature_bins", 0, file_id, &temp_bins_N);
  printf("Number of temperature bins is: %d\n", temp_bins_N);
  status=dataset_read("/Header/Number_of_density_bins", 0, file_id, &n_h_bins_N);
  printf("Number of hydrogen density bins is: %d\n", n_h_bins_N);
  status=dataset_read("/Header/Number_of_helium_fractions", 0, file_id, &he_frac_bins_N);
  printf("Number of helium mass fraction bins is: %d\n", he_frac_bins_N);
  //printf("Number of temperature bins is 3");
  exit(0);

  /* status=attribute_read('Attribute',hdf5_type, file_id,&n_h_bins_N); */
  /* status=attribute_read('Attribute',hdf5_type, file_id,&temp_bins_N); */
  /* status=attribute_read('Attribute',hdf5_type, 	file_id,&he_frac_bins_N); */
  
  /*now we need to get the header file to know the size of our arrays*/

  //dataset_id = H5Dopen(file_id, "/Header/Number_of_density_bins");
  //status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &n_h_bins_N);
  //status = H5Dclose(dataset_id);

  //dataset_id = H5Dopen(file_id, "/Header/Number_of_temperature_bins");
  //status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &temp_bins_N);
  //status = H5Dclose(dataset_id);

  //dataset_id = H5Dopen(file_id, "/Header/Number_of_helium_fractions");
  //status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &he_frac_bins_N);
  //status = H5Dclose(dataset_id);
  
  //printf("Number of hydrogen density bins is: %d\n", n_h_bins_N);
  //printf("Number of temperature bins is: %d\n", temp_bins_N);
  //printf("Number of helium fraction bins is: %d\n", he_frac_bins_N);

  /*Now we make our arrays of hydrogen_density, temperature, and helium fractions*/
#ifdef DONTCARE
  n_h_bins=(float *) malloc(n_h_bins_N * sizeof(float));
  temp_bins=(float *) malloc(temp_bins_N * sizeof(float));
  he_frac_bins=(float*) malloc(he_frac_bins_N * sizeof(float));
  
  enum hdf5_type = my_hdf5_float;
  //dataset_read is a function I have yet to write
  status=dataset_read("/Metal_free/Hydrogen_density_bins",hdf5_type, 
			file_id,&n_h_bins);
  status=dataset_read("/Metal_free/temperature_bins",hdf5_type,
			file_id,&temp_bins);
  status=dataset_read("/Metal_free/Helium_fraction_bins",hdf5_type,
			file_id,&he_frac_bins);
  
  //net_cooling_rate is a 3-D array
  net_cooling_rate= (float ***)malloc(he_frac_bins_N*sizeof(float **)); //might need to switch around the 'bins_N'
  net_cooling_rate[0]= (float **)malloc(temp_bins_N*sizeof(float *));
  net_cooling_rate[0][0]= (float *)malloc(n_h_bins_N*sizeof(float));

  /*sprintf(set_name,"/Metal_free/Hydrogen_density_bins"); 
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name);
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, n_h_bins); 
  status = H5Dclose(dataset_id);
  
  sprintf(set_name,"/Metal_free/Temperature_bins");
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name);
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, temp_bins); 
  status = H5Dclose(dataset_id);

  sprintf(set_name,"/Metal_free/Helium_mass_fraction_bins");
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name);
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, he_frac_bins); 
  status = H5Dclose(dataset_id);

  sprintf(set_name,"/Metal_free/Net_Cooling"); /*we are considering metal-free cooling*/
  printf("READING: %s\n", set_name);
  dataset_id = H5Dopen(file_id, set_name); /*gives us an identifier for the dataset within the file*/
  status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, net_cooling_rate); 
  status = H5Dclose(dataset_id);

  status = H5Fclose(file_id); //close the file

  //now we have some arrays we can perform operations on them
  
  printf("TEST:\n N_H IS: %.5e\n TEMPERATURE IS: %.5e\n HE_FRAC IS: %.5e\n LAMBDA IS %.5e\n",n_h_bins[20],temp_bins[100],he_frac_bins[5], net_cooling_rate[5][100][20]);
  
  /*Now we create a temperature array with interpolated values*/
  int array_size;
  array_size=10*(temp_bins_N-1); //cheating a bit here, 'cutting off' the highest temperatures
  float temp_array[array_size];
  int i,j;
  float factor;
  for (i=0;i<temp_bins_N-1;i++){
    factor=0.0;
    for (j=0;j<10;j++){
      temp_array[i*10+j]=temp_bins[i]+(factor/10.0)*(temp_bins[i+1]-temp_bins[i]);
      //printf("%d: %.5e\n",i*10+j,temp_array[i*10+j]);
      factor=factor+1.0;
    }
  }
  /*Write values of temperature bins to a text file*/

  FILE *f;
  f=fopen("./txt_files/temp_bins.txt","w");
  for (i=0;i<temp_bins_N-1;i++){
    fprintf(f,"%.7e\n",temp_bins[i]);
      };
  fclose(f);
  
  printf("Written temperature bins to file\n");

  f=fopen("./txt_files/temp_array.txt","w");
  for (i=0;i<array_size;i++){
    fprintf(f,"%.7e\n",temp_array[i]);
      };
  fclose(f);
  
  printf("Written temperature array to file\n");
   
  /*now we get corresponding arrays for 'lambda' for particular values of n_h and he_frac which are in the table*/
  int n_h_index, he_frac_index; //hard coding values
  n_h_index=40;
  he_frac_index=3;

  float n_h, he_frac;
  n_h=n_h_bins[n_h_index];
  he_frac=he_frac_bins[he_frac_index]; //selecting a particular value for n_h and he_frac
  printf("Chosen value of N_H: %.5e\n", n_h);
  printf("Chosen value of HE_FRAC: %.5e\n", he_frac);

  float lambda_bins[temp_bins_N-1]; // normalized_lambda_bins[temp_bins_N-1];
  for (i=0;i<temp_bins_N-1;i++){
    lambda_bins[i]=net_cooling_rate[he_frac_index][i][n_h_index];
    //normalized_lambda_bins[i]=lambda_bins[i]/(n_h*n_h);
  }

  /*Write these 'lambda bins' to a text file*/

  char lambda_bins_file[500], lambda_array_file[500];
  sprintf(lambda_bins_file,"./txt_files/lambda_bins_n_h_%.5e_he_frac_%.2e.txt", n_h,he_frac);
  sprintf(lambda_array_file,"./txt_files/lambda_array_n_h_%.5e_he_frac_%.2e.txt", n_h,he_frac);
  f=fopen(lambda_bins_file,"w");
  for (i=0;i<temp_bins_N-1;i++){
    fprintf(f,"%.7e\n",lambda_bins[i]);
      };
  fclose(f);

  printf("Written lambda bins to a file\n");

  /*Now we want to interpolate between the values in the lambda bins to get the full array*/
  
  float lambda_array[array_size];
  for (i=0;i<temp_bins_N-1;i++){
    for (j=0;j<10;j++){
      lambda_array[i*10+j]=lambda_bins[i]+((temp_array[i*10+j]-temp_bins[i])/(temp_bins[i+1]-temp_bins[i]))*(lambda_bins[i+1]-lambda_bins[i]); //this is the actual interpolation
    }
  }

  f=fopen(lambda_array_file,"w");
  for (i=0;i<array_size;i++){
    fprintf(f,"%d: %.7e\n",i,lambda_array[i]);
      };
  fclose(f);

  printf("Written lambda array to a file\n");

  printf("All done! Now use python to plot\n");

  
#endif  
return(0);
    }
