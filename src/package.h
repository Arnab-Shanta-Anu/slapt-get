/*
 * Copyright (C) 2003-2017 Jason Woodward <woodwardj at jaos dot org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#define SLAPT_PKG_PARSE_REGEX "(.*{1,})\\-(.*[\\-].*[\\-].*)\\.[ti]+[gblxkz]+[ ]{0,}$"
#define SLAPT_PKG_NAMEVER "(.*{1,})\\-(.*[\\-].*[\\-].*)"
#define SLAPT_PKG_VER "(.*)[\\-](.*)[\\-](.*)"
#define SLAPT_PKG_NAME_PATTERN "^PACKAGE NAME:[ ]{1,}(.*{1,})\\-(.*[\\-].*[\\-].*)(\\.[ti]+[gblzkx]+)[ ]{0,}$"
#define SLAPT_PKG_MIRROR_PATTERN "^PACKAGE MIRROR:[ ]+(.*)$"
#define SLAPT_PKG_PRIORITY_PATTERN "^PACKAGE PRIORITY:[ ]+([0-9]{1,})$"
#define SLAPT_PKG_LOCATION_PATTERN "^PACKAGE LOCATION:[ ]+(.*)$"
#define SLAPT_PKG_SIZEC_PATTERN "^PACKAGE SIZE [(]+compressed[)]{1,}:[ ]{1,}([0-9]{1,}) K$"
#define SLAPT_PKG_SIZEU_PATTERN "^PACKAGE SIZE [(]+uncompressed[)]{1,}:[ ]{1,}([0-9]{1,}) K$"
#define SLAPT_PKG_LOG_SIZEC_PATTERN "^COMPRESSED PACKAGE SIZE:[ ]{1,}([0-9\\.]{1,})[ ]{0,}([MK])$"
#define SLAPT_PKG_LOG_SIZEU_PATTERN "^UNCOMPRESSED PACKAGE SIZE:[ ]{1,}([0-9\\.]{1,})[ ]{0,}([MK])$"
#define SLAPT_PKG_LOG_DIR "/var/log/packages"
#define SLAPT_ROOT_ENV_NAME "ROOT"
#define SLAPT_ROOT_ENV_LEN 255
#define SLAPT_PKG_LOG_PATTERN "^(.*{1,})\\-(.*[\\-].*[\\-].*)"
#define SLAPT_MD5SUM_REGEX "([a-zA-Z0-9]{1,})[ ]{1,}([a-zA-Z0-9\\/._+\\-]{1,})\\/(.*{1,})\\-(.*[\\-].*[\\-].*)\\.[ti]+[gblzkx]+$"
#define SLAPT_REQUIRED_REGEX "^[ ]{0,}([^ ]{1,})[ ]{0,}([\\<\\=\\>]+){0,}[ ]{0,}([a-zA-Z0-9\\+\\.\\_\\-]+){0,}[ ]{0,}$"
#define SLAPT_MD5_STR_LEN 33
#define SLAPT_PKG_LIST "PACKAGES.TXT"
#define SLAPT_PKG_LIST_GZ "PACKAGES.TXT.gz"
#define SLAPT_PKG_LIST_L "package_data"
#define SLAPT_PATCHES_LIST "patches/PACKAGES.TXT"
#define SLAPT_PATCHES_LIST_GZ "patches/PACKAGES.TXT.gz"
#define SLAPT_CHANGELOG_FILE "ChangeLog.txt"
#define SLAPT_CHANGELOG_FILE_GZ "ChangeLog.txt.gz"
#define SLAPT_PATCHDIR "patches/"
#define SLAPT_REMOVE_CMD "/sbin/removepkg "
#define SLAPT_INSTALL_CMD "/sbin/installpkg "
#define SLAPT_UPGRADE_CMD "/sbin/upgradepkg --reinstall "
#define SLAPT_CHECKSUM_FILE "CHECKSUMS.md5"
#define SLAPT_CHECKSUM_FILE_GZ "CHECKSUMS.md5.gz"
#define SLAPT_HEAD_FILE_EXT ".head"
#define SLAPT_MAX_MMAP_SIZE 1024
#define SLAPT_MAX_ZLIB_BUFFER 1024

typedef struct {
  char md5[SLAPT_MD5_STR_LEN];
  char *name;
  char *version;
  char *mirror;
  char *location;
  char *description;
  char *required;
  char *conflicts;
  char *suggests;
  char *file_ext;
  unsigned int size_c;
  unsigned int size_u;
  unsigned int priority;
  SLAPT_BOOL_T installed;
} slapt_pkg_info_t;

typedef struct {
  slapt_pkg_info_t **pkgs;
  unsigned int pkg_count;
  SLAPT_BOOL_T free_pkgs;
  SLAPT_BOOL_T ordered;
} slapt_pkg_list_t;

typedef struct {
  slapt_pkg_info_t *installed;
  slapt_pkg_info_t *upgrade;
  SLAPT_BOOL_T reinstall;
} slapt_pkg_upgrade_t;

typedef struct {
  slapt_pkg_upgrade_t **pkgs;
  unsigned int pkg_count;
  unsigned int reinstall_count;
} slapt_pkg_upgrade_list_t;

typedef struct {
  char *pkg;
  char *error;
} slapt_pkg_err_t;

typedef struct {
  slapt_pkg_err_t **errs;
  unsigned int err_count;
} slapt_pkg_err_list_t;

/* returns an empty package structure */
slapt_pkg_info_t *slapt_init_pkg(void);
/* frees the package structure */
void slapt_free_pkg(slapt_pkg_info_t *pkg);

/* create an empty package list */
slapt_pkg_list_t *slapt_init_pkg_list(void);
/* add a package to a package list */
void slapt_add_pkg_to_pkg_list(slapt_pkg_list_t *list,
                               slapt_pkg_info_t *pkg);
/* free package list */
void slapt_free_pkg_list(slapt_pkg_list_t *);

/*
  update the local package cache. Must be chdir'd to working_dir.
*/
int slapt_update_pkg_cache(const slapt_rc_config *global_config);
/* write pkg data to disk */
void slapt_write_pkg_data(const char *source_url,FILE *d_file,
                          slapt_pkg_list_t *pkgs);
/* parse the PACKAGES.TXT file */
slapt_pkg_list_t *slapt_parse_packages_txt(FILE *);
/*
  return a list of available packages.  Must be chdir'd to
  rc_config->working_dir.  Otherwise, open a filehandle to the package data
  and pass it to slapt_parse_packages_txt();
*/
slapt_pkg_list_t *slapt_get_available_pkgs(void);
/* retrieve list of installed pkgs */
slapt_pkg_list_t *slapt_get_installed_pkgs(void);

/* retrieve the newest package from package list */
slapt_pkg_info_t *slapt_get_newest_pkg(slapt_pkg_list_t *,const char *);
/* get the exact package */
slapt_pkg_info_t *slapt_get_exact_pkg(slapt_pkg_list_t *list,
                                      const char *name,
                                      const char *version);
/* lookup package by details */
slapt_pkg_info_t *slapt_get_pkg_by_details(slapt_pkg_list_t *list,
                                           const char *name,
                                           const char *version,
                                           const char *location);
/* search package list with pattern */
slapt_pkg_list_t *slapt_search_pkg_list(slapt_pkg_list_t *list,
                                             const char *pattern);


/*
  install package by calling installpkg
  returns 0 on success, -1 on error
*/
int slapt_install_pkg(const slapt_rc_config *,slapt_pkg_info_t *);
/*
  upgrade package by calling upgradepkg
  returns 0 on success, -1 on error
*/
int slapt_upgrade_pkg(const slapt_rc_config *global_config,
                      slapt_pkg_info_t *pkg);
/*
  remove package by calling removepkg
  returns 0 on success, -1 on error
*/
int slapt_remove_pkg(const slapt_rc_config *,slapt_pkg_info_t *);

/* get a list of obsolete packages */
slapt_pkg_list_t *
  slapt_get_obsolete_pkgs ( const slapt_rc_config *global_config,
                            slapt_pkg_list_t *avail_pkgs,
                            slapt_pkg_list_t *installed_pkgs);


/*
  generate a short description, returns (char *) on success or NULL on error
  caller responsible for freeing the returned data
*/
char *slapt_gen_short_pkg_description(slapt_pkg_info_t *);
/*
  generate the filename from the url
  caller responsible for freeing the returned data
*/
char *slapt_gen_filename_from_url(const char *url,const char *file);
/*
  generate the package file name
  caller responsible for freeing the returned data
*/
char *slapt_gen_pkg_file_name(const slapt_rc_config *global_config,
                              slapt_pkg_info_t *pkg);
/*
  generate the head cache filename
  caller responsible for freeing the returned data
*/
char *slapt_gen_head_cache_filename(const char *filename_from_url);
/*
  generate the download url for a package
  caller responsible for freeing the returned data
*/
char *slapt_gen_pkg_url(slapt_pkg_info_t *pkg);
/*
  exclude pkg based on pkg name
  returns 1 if package is present in the exclude list, 0 if not present
*/
int slapt_is_excluded(const slapt_rc_config *,slapt_pkg_info_t *);
/*
  package is already downloaded and cached, md5sum if applicable is ok
  returns slapt_code_t.
*/
slapt_code_t slapt_verify_downloaded_pkg(const slapt_rc_config *global_config,
                                slapt_pkg_info_t *pkg);
/*
  fill in the md5sum of the package
*/
void slapt_get_md5sums(slapt_pkg_list_t *pkgs, FILE *checksum_file);
/*
  find out the pkg file size (post download)
*/
size_t slapt_get_pkg_file_size(const slapt_rc_config *global_config,
                               slapt_pkg_info_t *pkg);

/*
  compare package versions
  returns just like strcmp,
    > 0 if a is greater than b
    < 0 if a is less than b
    0 if a and b are equal
*/
int slapt_cmp_pkg_versions(const char *a, const char *b);
int slapt_cmp_pkgs(slapt_pkg_info_t *a, slapt_pkg_info_t *b);

/*
  resolve dependencies
  returns 0 on success, -1 on error setting conflict_err and missing_err
  (usually called with transaction->conflict_err and transaction->missing_err)
*/
int slapt_get_pkg_dependencies(const slapt_rc_config *global_config,
                         slapt_pkg_list_t *avail_pkgs,
                         slapt_pkg_list_t *installed_pkgs,
                         slapt_pkg_info_t *pkg,
                         slapt_pkg_list_t *deps,
                         slapt_pkg_err_list_t *conflict_err,
                         slapt_pkg_err_list_t *missing_err);
/*
  return list of package conflicts
*/
slapt_pkg_list_t *slapt_get_pkg_conflicts(slapt_pkg_list_t *avail_pkgs,
                                               slapt_pkg_list_t *installed_pkgs,
                                               slapt_pkg_info_t *pkg);
/*
  return list of packages required by
*/
slapt_pkg_list_t *slapt_is_required_by(const slapt_rc_config *global_config,
                                            slapt_pkg_list_t *avail,
                                            slapt_pkg_list_t *installed_pkgs,
                                            slapt_pkg_list_t *pkgs_to_install,
                                            slapt_pkg_list_t *pkgs_to_remove,
                                            slapt_pkg_info_t *pkg);

/*
  empty packages from cache dir
*/
void slapt_clean_pkg_dir(const char *dir_name);
/*
  clean out old outdated packages in the cache that are no longer available
  in the current source lists (ie are not downloadable)
*/
void slapt_purge_old_cached_pkgs(const slapt_rc_config *global_config,
                                 const char *dir_name,
                                 slapt_pkg_list_t *avail_pkgs);

/*
  make a copy of a package (needs to be freed with free_pkg)
*/
slapt_pkg_info_t *slapt_copy_pkg(slapt_pkg_info_t *dst,slapt_pkg_info_t *src);

/*
  package error handling api to handle errors within core functions
*/
slapt_pkg_err_list_t *slapt_init_pkg_err_list(void);
void slapt_add_pkg_err_to_list(slapt_pkg_err_list_t *l,
                               const char *pkg,const char *err);
int slapt_search_pkg_err_list(slapt_pkg_err_list_t *l,
                              const char *pkg, const char *err);
void slapt_free_pkg_err_list(slapt_pkg_err_list_t *l);

/*
  download the PACKAGES.TXT and CHECKSUMS.md5 files
  compressed is set to 1 if the compressed version was downloaded.
*/
slapt_pkg_list_t *slapt_get_pkg_source_packages (const slapt_rc_config *global_config,
                                                      const char *url, unsigned int *compressed);
slapt_pkg_list_t *slapt_get_pkg_source_patches (const slapt_rc_config *global_config,
                                                     const char *url, unsigned int *compressed);
FILE *slapt_get_pkg_source_checksums (const slapt_rc_config *global_config,
                                      const char *url, unsigned int *compressed);
int slapt_get_pkg_source_changelog (const slapt_rc_config *global_config,
                                      const char *url, unsigned int *compressed);

/* clean package name from package description */
void slapt_clean_description (char *description, const char *name);

/*
  retrieve the packages changelog entry, if any.  Returns NULL otherwise
  Must be chdir'd to working_dir.
*/
char *slapt_get_pkg_changelog(const slapt_pkg_info_t *pkg);

/* returns a string representation of the package */
char *slapt_stringify_pkg(const slapt_pkg_info_t *pkg);

/*
  get the package filelist, returns (char *) on success or NULL on error
  caller responsible for freeing the returned data
*/
char *slapt_get_pkg_filelist(const slapt_pkg_info_t *pkg);

/*
  generate the directory name for the package log directory, 
  considering the ROOT environment variable if set
  caller responsible for freeing the returned data
 */
char *slapt_gen_package_log_dir_name(void);

