#include "repository.h"
#include "helper.h"
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

REPOSITORY * repository_init(const char * root_pathname) {
    debug("[repository_init] root_pathname : %s\n", root_pathname);
    REPOSITORY * repo = (REPOSITORY *)malloc(sizeof(REPOSITORY));
    
    memset(repo, 0, sizeof(REPOSITORY));
    
    strncpy(repo->root_pathname, root_pathname, FULL_PATH_SZ);
    
    debug("repo->root_pathname : %s\n", repo->root_pathname);
    
    if(!file_exits(repo->root_pathname)) {
         mkdir(repo->root_pathname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //Mode 755
    }
    
    return repo;
}

void repository_dispose(REPOSITORY * repo) {
    debug("[repository_dispose] repo : %p\n", repo);
    free(repo);
}

REPO_ITEM * repository_new_item(REPOSITORY * repo) {
    debug("[repository_new_item] repo : %p\n", repo);
    
    REPO_ITEM * item = (REPO_ITEM *)malloc(sizeof(REPO_ITEM));
    
    memset(item, 0, sizeof(REPO_ITEM));
    
    item->repo = repo;
    
    char * uuid = get_uuid();
    
    strncpy(item->uuid, uuid, UUID_STR_SZ);
    
    debug("item->uuid : %s\n", item->uuid);
    
    free(uuid);
    
    snprintf(item->item_pathname, FULL_PATH_SZ, "%s%s", repo->root_pathname, item->uuid);
    
    debug("item->item_pathname : %s\n", item->item_pathname);
    
    if(!file_exits(item->item_pathname)) {
         mkdir(item->item_pathname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //Mode 755
    }
    
    return item;
}

void repository_close_item(REPO_ITEM * item) {
    debug("[repository_close_item] item : %p\n", item);
    free(item);
}

FILE * repository_open_write_file(REPO_ITEM * item, const char * name) {
    debug("[repository_open_write_file] item : %p, name : %s\n", item, name);
    
    FILE * fp = NULL;
    
    char * p = (char *)malloc(sizeof(char) * FULL_PATH_SZ);
    
    snprintf(p, FULL_PATH_SZ, "%s/%s", item->item_pathname, name);
    
    fp = fopen(p, "w");
    
    free(p);
    
    return fp;
}

FILE * repository_open_read_file(REPO_ITEM * item, const char * name) {
    debug("[repository_open_read_file] item : %p, name : %s\n", item, name);
    FILE * fp = NULL;
    
    char * p = (char *)malloc(sizeof(char) * FULL_PATH_SZ);
    
    snprintf(p, FULL_PATH_SZ, "%s/%s", item->item_pathname, name);
    
    fp = fopen(p, "r");
    
    free(p);
    
    return fp;
}

#ifdef __cplusplus
}
#endif
