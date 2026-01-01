#ifndef STATUS_LINE_GIT_FUNCTIONS_H
#define STATUS_LINE_GIT_FUNCTIONS_H
#include "git2.h"

// https://stackoverflow.com/questions/24014361/max-length-of-git-branch-name
// As per a stackoverflow post regarding GitHub's limits, see link above.
#define MAX_BRANCH_NAME_LENGTH 244
#define MAX_REMOTE_NAME_SIZE 244

#define DIRNAME_SIZE 256
#define HEAD_PATH_SIZE (MAX_REMOTE_NAME_SIZE+64)
#define MAX_BRANCH_PATH_LENGTH (MAX_BRANCH_NAME_LENGTH+64)
#define REMOTE_NAME_SIZE MAX_REMOTE_NAME_SIZE

typedef struct GitStatusNumbers {
    unsigned int has_changed;
    unsigned int branch_out_of_sync;
    unsigned int added;
    unsigned int deleted;
    unsigned int modified;
    unsigned int untracked;
    unsigned int current_ahead;
    unsigned int current_behind;
    unsigned int default_ahead;
    unsigned int default_behind;
} GitStatusNumbers;

int get_git_repo(git_repository** repo, const char* dir) ;
int is_git_repo(git_repository* repo);
void initialize_git_status_numbers(GitStatusNumbers* statusNumbers);
int update_git_status_list(const char *path, unsigned int status_flags, void *payload);
int get_git_status(git_repository* repo, GitStatusNumbers* statusNumbers);
int get_remote_name(git_repository* repo, char* buffer, size_t buffer_size);
int get_default_branch_name(git_repository* repo, char* buffer, size_t buffer_size);
int get_current_branch_name(git_repository* repo, char* buffer, size_t buffer_size);
int get_git_current_branch_status(git_repository* repo, const char* branch_name, const char* remote_name, GitStatusNumbers* statusNumbers);
int get_git_default_branch_status(git_repository* repo, GitStatusNumbers* statusNumbers);

#endif