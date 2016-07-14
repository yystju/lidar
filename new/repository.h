#ifndef _REPOSITORY_H_
#define _REPOSITORY_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FULL_PATH_SZ 1024
#define UUID_STR_SZ 64

typedef struct {
  char root_pathname[FULL_PATH_SZ];
} REPOSITORY;

typedef struct {
  REPOSITORY * repo;
  char uuid[UUID_STR_SZ];
  char item_pathname[FULL_PATH_SZ];
} REPO_ITEM;

REPOSITORY * repository_init(const char * root_pathname);
void repository_dispose(REPOSITORY * repo);

REPO_ITEM * repository_new_item(REPOSITORY * repo);
void repository_close_item(REPO_ITEM * item);

FILE * repository_open_write_file(REPO_ITEM * item, const char * name);
FILE * repository_open_read_file(REPO_ITEM * item, const char * name);

#ifdef __cplusplus
}
#endif

#endif