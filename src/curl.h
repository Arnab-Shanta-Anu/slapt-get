
/*
  retrieves the head data for the url, returns (char *) or NULL on error
*/
char *slapt_head_request(const char *url);

/*
  this fills FILE with data from url, used for PACKAGES.TXT and CHECKSUMS
  returns 0 on success, on error -1 or CURLE_HTTP_RANGE_ERROR
*/
int slapt_get_mirror_data_from_source(FILE *fh,
                                      const slapt_rc_config *global_config,
                                      const char *base_url,
                                      const char *filename);

/*
  download pkg, calls download_data
  returns 0 on success, on error -1 or CURLE_HTTP_RANGE_ERROR
*/
int slapt_download_pkg(const slapt_rc_config *global_config,
                       slapt_pkg_info_t *pkg);

/*
  this is the default progress callback if global_config->progress_cb == NULL
*/
int slapt_progress_callback(void *clientp, double dltotal, double dlnow,
                      double ultotal, double ulnow);

/*
  do a head request on the mirror data to find out if it's new
  returns (char *) or NULL
*/
char *slapt_head_mirror_data(const char *wurl,const char *file);
/*
  clear head cache storage file
*/
void slapt_clear_head_cache(const char *cache_filename);
/*
  cache the head request
*/
void slapt_write_head_cache(const char *cache, const char *cache_filename);
/*
  read the cached head request
  returns (char *) or NULL
*/
char *slapt_read_head_cache(const char *cache_filename);

