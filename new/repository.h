#ifndef _REPOSITORY_H_
#define _REPOSITORY_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FULL_PATH_SZ 1024
#define UUID_SZ 16

typedef struct {
  char root_pathname[FULL_PATH_SZ];
} REPOSITORY;

typedef struct {
  REPOSITORY * repo;
  char uuid[UUID_SZ];
  char item_pathname[FULL_PATH_SZ];
} REPO_ITEM;

REPOSITORY * repository_init(const char * root_pathname);
void repository_dispose(REPOSITORY * repo);

REPO_ITEM * repository_new_item(REPOSITORY * repo);
void repository_close_item(REPO_ITEM * item);

#ifdef __cplusplus
}
#endif

#endif