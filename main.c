#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "cooling_params.h"
#include "hdf5_wrapper.h"

int main(){
  
  /*First read the redshifts from the .dat file. First we will check the number of redshifts*/
  
  FILE *infile;
  
  int i = 0;

  char buffer[500];
  
  infile = fopen(REDSHIFTS_FILENAME, "r");
  if(infile == NULL)
    puts("Can't open the redshifts file");

  if(fscanf(infile, "%s", buffer) != EOF){
    n_redshifts = atoi(buffer);
    if (n_redshifts != NUMBER_OF_REDSHIFTS){
      printf("Incorrect number of redshifts\n");
      exit(-1);
    }
    printf("Number of redshifts is %d\n",n_redshifts);
    for (i=0;i<NUMBER_OF_REDSHIFTS;i++){
      fscanf(infile, "%s", buffer);
      redshifts[i]=atof(buffer);
    }
  }
  fclose(infile);

  /*Now we want to find two redshifts 'either side' of our specified z*/

  float z = 2.6; //hard-coded value we will use for now
  float z_1 = redshifts[0];
  float z_2 = redshifts[1];
  float dz;

  i=0;

  if(z<redshifts[0]){
    printf("Error: Specified redshift is too small,smallest allowed value is %1.3f\n", redshifts[0]);
    exit(-1);
  }
  
  while (z>redshifts[i+1] && i<NUMBER_OF_REDSHIFTS-2){
    i += 1;
    z_1=redshifts[i];
    z_2=redshifts[i+1];
    dz=z-z_1;
  }

  if (z>redshifts[NUMBER_OF_REDSHIFTS-1]){
    printf("Error: Specified redshift is too large, largest allowed value is %1.3f\n", redshifts[NUMBER_OF_REDSHIFTS-1]);
    exit(-1);
  }

  printf("Chosen redshift: %1.3f\n", z);
  printf("Lower redshift table at: %1.3f\n", z_1);
  printf("Upper redshift table at: %1.3f\n", z_2);
  printf("d_z is: %1.3f\n", dz);
  
    

  /*Read in data from the hdf5 files*/

  hid_t file_id; /*these will be used for reading the hdf5 files*/
  herr_t status; /*tells us if we have an error with reading hdf5 files*/

  /*here I am two structs which for the two cooling tables, for some reason
    I couldn't do this in the header file*/

  
  struct cooling_table_struct *ct_1 = malloc(sizeof(struct cooling_table_struct));
  struct cooling_table_struct *ct_2 = malloc(sizeof(struct cooling_table_struct));

  sprintf(ct_1->filename, "%sz_%1.3f.hdf5",BASE_PATH, z_1);
  sprintf(ct_2->filename, "%sz_%1.3f.hdf5",BASE_PATH, z_2);

  printf("Lower redshift cooling table file is: %s\n",ct_1->filename);
  printf("Upper redshift cooling table file is: %s\n",ct_2->filename);

  /*

    READING LOWER REDSHIFT COOLING TABLE*/


  file_id = H5Fopen(ct_1->filename,H5F_ACC_RDONLY,H5P_DEFAULT);

  /* check whether the tables have the expected size */
 
  status=dataset_read("/Header/Number_of_temperature_bins", INT, file_id, &ct_1->temp_bins_N);
  if (ct_1->temp_bins_N != COOL_TABLE_TEMP_BINS){
    printf("Incorrect number of temperature bins\n");
    exit(-1);
  }
  printf("Number of temperature bins is: %d\n", ct_1->temp_bins_N);

  status=dataset_read("/Header/Number_of_density_bins", INT, file_id, &ct_1->n_h_bins_N);
  if (ct_1->n_h_bins_N != COOL_TABLE_N_H_BINS){
    printf("Incorrect number of density bins\n");
    exit(-1);
  }
  printf("Number of Hydrogen density bins is: %d\n", ct_1->n_h_bins_N);

  status=dataset_read("/Header/Number_of_helium_fractions", INT, file_id, &ct_1->he_frac_bins_N);
  if (ct_1->he_frac_bins_N != COOL_TABLE_HE_BINS){
    printf("Incorrect number of Helium fraction bins\n");
    exit(-1);
  }
  printf("Number of helium mass fraction bins is: %d\n", ct_1->he_frac_bins_N);

  /*Now we make our arrays of hydrogen_density, temperature, and helium fractions*/

  /*Read in the bins*/
  status=dataset_read("/Metal_free/Hydrogen_density_bins",FLOAT, 
			file_id,&ct_1->n_h_bins);
  status=dataset_read("/Metal_free/Temperature_bins",FLOAT,
			file_id,&ct_1->temp_bins);
  status=dataset_read("/Metal_free/Helium_mass_fraction_bins",FLOAT,
 			file_id,&ct_1->he_frac_bins);
  /*Read in the cooling table*/
  status=dataset_read("/Metal_free/Net_Cooling",FLOAT,
			file_id,&ct_1->net_cooling_rate);

  status = H5Fclose(file_id); //close the file

  /*
    READ UPPER REDSHIFT COOLING TABLE
  */

  file_id = H5Fopen(ct_2->filename,H5F_ACC_RDONLY,H5P_DEFAULT);

  /* check whether the tables have the expected size */
 
  status=dataset_read("/Header/Number_of_temperature_bins", INT, file_id, &ct_2->temp_bins_N);
  if (ct_2->temp_bins_N != COOL_TABLE_TEMP_BINS){
    printf("Incorrect number of temperature bins\n");
    exit(-1);
  }
  printf("Number of temperature bins is: %d\n", ct_2->temp_bins_N);

  status=dataset_read("/Header/Number_of_density_bins", INT, file_id, &ct_2->n_h_bins_N);
  if (ct_2->n_h_bins_N != COOL_TABLE_N_H_BINS){
    printf("Incorrect number of density bins\n");
    exit(-1);
  }
  printf("Number of Hydrogen density bins is: %d\n", ct_2->n_h_bins_N);

  status=dataset_read("/Header/Number_of_helium_fractions", INT, file_id, &ct_2->he_frac_bins_N);
  if (ct_2->he_frac_bins_N != COOL_TABLE_HE_BINS){
    printf("Incorrect number of Helium fraction bins\n");
    exit(-1);
  }
  printf("Number of helium mass fraction bins is: %d\n", ct_2->he_frac_bins_N);

  /*Now we make our arrays of hydrogen_density, temperature, and helium fractions*/

  /*Read in the bins*/
  status=dataset_read("/Metal_free/Hydrogen_density_bins",FLOAT, 
			file_id,&ct_2->n_h_bins);
  status=dataset_read("/Metal_free/Temperature_bins",FLOAT,
			file_id,&ct_2->temp_bins);
  status=dataset_read("/Metal_free/Helium_mass_fraction_bins",FLOAT,
 			file_id,&ct_2->he_frac_bins);
  /*Read in the cooling table*/
  status=dataset_read("/Metal_free/Net_Cooling",FLOAT,
			file_id,&ct_2->net_cooling_rate);

  status = H5Fclose(file_id); //close the file
  
  



  //now we have some arrays we can perform operations on them
  
  printf("TEST:\n N_H IS: %.5e\n TEMPERATURE IS: %.5e\n HE_FRAC IS: %.5e\n LAMBDA IS %.5e\n",ct_1->n_h_bins[20],ct_1->temp_bins[100],ct_1->he_frac_bins[5], ct_1->net_cooling_rate[5][100][20]);
  printf("TEST:\n N_H IS: %.5e\n TEMPERATURE IS: %.5e\n HE_FRAC IS: %.5e\n LAMBDA IS %.5e\n",ct_2->n_h_bins[20],ct_2->temp_bins[100],ct_2->he_frac_bins[5], ct_2->net_cooling_rate[5][100][20]);
  exit(0);


#ifdef LEAVE
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
    fprintf(f," %.7e\n",lambda_array[i]);
      };
  fclose(f);

  printf("Written lambda array to a file\n");

  printf("All done! Now use python to plot\n");
#endif
return(0);
    }
