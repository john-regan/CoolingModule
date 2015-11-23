#include <hdf5.h>
#define PATH "/cosma5/data/Eagle/BG_Tables/CoolingTables/"
#define FILENAME "z_0.000.hdf5"
#define STR_LENGTH     300

#define COOL_TABLE_N_H_BINS       41
#define COOL_TABLE_TEMP_BINS      176
#define COOL_TABLE_HE_BINS          7

enum my_hdf5_types{
  my_hdf5_int,
  my_hdf5_long,
  my_hdf5_float,
  my_hdf5_double
};
enum my_hdf5_types hdf5_type;

int n_h_bins_N, temp_bins_N, he_frac_bins_N;
float *n_h_bins, *temp_bins, *he_frac_bins;



herr_t attribute_read(char *attribute_name, enum my_hdf5_types hdf5_type,
	       hid_t hdf5_file_handle, void *buf);

extern struct cool_table_struct
{
  char filename[STR_LENGTH];
  /*  int n_h_bins_N, temp_bins_N, he_frac_bins_N; */
  float  n_h_bins[COOL_TABLE_N_H_BINS];
  float temp_bins[COOL_TABLE_TEMP_BINS];
  float  he_frac_bins[COOL_TABLE_HE_BINS] ;
  float net_cooling_rate[COOL_TABLE_N_H_BINS][COOL_TABLE_TEMP_BINS][COOL_TABLE_HE_BINS];
} cool_table;


