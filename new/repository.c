#include "repository.h"
#include "helper.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

REPOSITORY * repository_init(const char * root_pathname) {
    REPOSITORY * repo = (REPOSITORY *)malloc(sizeof(REPOSITORY));
    
    strncpy(repo->root_pathname, root_pathname, FULL_PATH_SZ);
    
    return repo;
}

void repository_dispose(REPOSITORY * repo) {
    free(repo);
}

REPO_ITEM * repository_new_item(REPOSITORY * repo) {
    REPO_ITEM * item = (REPO_ITEM *)malloc(sizeof(REPO_ITEM));
    
    item->repo = repo;
    
    return item;
}

void repository_close_item(REPO_ITEM * item) {
    
}

#ifdef __cplusplus
}
#endif
