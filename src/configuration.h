
#define SOURCE_TOKEN "SOURCE="
#define WORKINGDIR_TOKEN "WORKINGDIR="
#define WORKINGDIR_TOKEN_LEN 256
#define EXCLUDE_TOKEN "EXCLUDE="

struct slapt_exclude_list {
  char **excludes;
  unsigned int count;
};

struct slapt_source_list {
  char **url;
  unsigned int count;
};

typedef struct {
  char working_dir[WORKINGDIR_TOKEN_LEN];
  struct slapt_source_list *sources;
  struct slapt_exclude_list *exclude_list;
  int(*progress_cb)(void *,double,double,double,double);
  SLAPT_BOOL_T download_only;
  SLAPT_BOOL_T dist_upgrade;
  SLAPT_BOOL_T simulate;
  SLAPT_BOOL_T no_prompt;
  SLAPT_BOOL_T prompt;
  SLAPT_BOOL_T re_install;
  SLAPT_BOOL_T ignore_excludes;
  SLAPT_BOOL_T no_md5_check;
  SLAPT_BOOL_T ignore_dep;
  SLAPT_BOOL_T disable_dep_check;
  SLAPT_BOOL_T print_uris;
  SLAPT_BOOL_T dl_stats;
  SLAPT_BOOL_T remove_obsolete;
  SLAPT_BOOL_T no_upgrade;
  unsigned int retry;
} slapt_rc_config;

/* initialize slapt_rc_config */
slapt_rc_config *slapt_init_config(void);
/* read the configuration from file_name.  Returns (rc_config *) or NULL */
slapt_rc_config *slapt_read_rc_config(const char *file_name);
/* free rc_config structure */
void slapt_free_rc_config(slapt_rc_config *global_config);

/* check that working_dir exists or make it if permissions allow */
void slapt_working_dir_init(const slapt_rc_config *global_config);

/*
  add an exclude expression to the exclude list.
  commonly called with global_config->exclude_list
*/
struct slapt_exclude_list *slapt_init_exclude_list(void);
void slapt_add_exclude(struct slapt_exclude_list *list,const char *e);
void slapt_remove_exclude(struct slapt_exclude_list *list,const char *e);
void slapt_free_exclude_list(struct slapt_exclude_list *list);

/*
  add or remove a package source url to the source list.
  commonly called with global_config->source_list
*/
struct slapt_source_list *slapt_init_source_list(void);
void slapt_add_source(struct slapt_source_list *list,const char *s);
void slapt_remove_source (struct slapt_source_list *list, const char *s);
void slapt_free_source_list(struct slapt_source_list *list);

