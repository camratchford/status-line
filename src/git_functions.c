#include "git_functions.h"
#include <git2.h>
#include <git2/errors.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>




int get_git_repo(git_repository** repo, const char* dir) {
    char dirname[DIRNAME_SIZE];

    if (dir == NULL) {
        getcwd(dirname, DIRNAME_SIZE);
    } else {
        strcpy(dirname, dir);
    }

    const int error = git_repository_open_ext(repo, dirname, GIT_REPOSITORY_OPEN_CROSS_FS, NULL);

    return error;
}

int is_git_repo(git_repository* repo) {
    char dirname[DIRNAME_SIZE];
    getcwd(dirname, DIRNAME_SIZE);

    const int error = git_repository_open_ext(&repo, dirname, GIT_REPOSITORY_OPEN_CROSS_FS, NULL);
    git_repository_free(repo);

    if (error == GIT_OK) {
        return 0;
    }

    return 1;
}

void initialize_git_status_numbers(GitStatusNumbers* statusNumbers) {
    statusNumbers->added = 0;
    statusNumbers->deleted = 0;
    statusNumbers->modified = 0;
    statusNumbers->untracked = 0;
    statusNumbers->current_ahead = 0;
    statusNumbers->current_behind = 0;
    statusNumbers->default_ahead = 0;
    statusNumbers->default_behind = 0;
}


int update_git_status_list(const char *path, const unsigned int status_flags, void *payload) {
    GitStatusNumbers* status_list = payload;
    if (status_flags & GIT_STATUS_WT_NEW) {
        status_list->untracked++;
        status_list->has_changed = 1;
    } else if (status_flags & GIT_STATUS_INDEX_NEW) {
        status_list->added++;
        status_list->has_changed = 1;
    } else if (status_flags & (GIT_STATUS_INDEX_DELETED | GIT_STATUS_WT_DELETED)) {
        status_list->deleted++;
        status_list->has_changed = 1;
    } else if (status_flags & (GIT_STATUS_INDEX_MODIFIED | GIT_STATUS_WT_MODIFIED)) {
        status_list->modified++;
        status_list->has_changed = 1;
    }
    return 0;
}

int get_git_status(git_repository* repo, GitStatusNumbers* statusNumbers) {
    const int error = git_status_foreach(repo, &update_git_status_list, statusNumbers);
    return error;
}

int get_remote_name(git_repository* repo, char* buffer, const size_t buffer_size) {
    int rc = 1;

    git_strarray remotes = {};
    git_remote_list(&remotes, repo);
    if (buffer_size > sizeof(remotes.strings[0])) {
        strcpy(buffer, "");
        git_strarray_dispose(&remotes);
        return 1;
    }

    if (remotes.count > 0) {
        strcpy(buffer, remotes.strings[0]);
        rc = 0;
    }

    git_strarray_dispose(&remotes);
    return rc;
}

int get_default_branch_name(git_repository* repo, char* buffer, const size_t buffer_size) {
    int rc = 1;
    git_reference *head_ref;
    char remote_path[HEAD_PATH_SIZE*sizeof(" ")];
    char head_path[HEAD_PATH_SIZE*sizeof(" ")];
    char remote_name[REMOTE_NAME_SIZE*sizeof(" ")];
    get_remote_name(repo, remote_name, REMOTE_NAME_SIZE);
    sprintf(remote_path, "refs/remotes/%s", remote_name);
    sprintf(head_path, "%s/HEAD", remote_path);

    git_reference_lookup(&head_ref, repo, head_path);
    if (head_ref) {
        char* parent_branch_name_ptr = (char*)git_reference_symbolic_target(head_ref);

        const size_t remote_path_len = strlen(remote_path);
        while ((parent_branch_name_ptr = strstr(parent_branch_name_ptr, remote_path)) != NULL) {
            memmove(parent_branch_name_ptr, parent_branch_name_ptr + remote_path_len, strlen(parent_branch_name_ptr + remote_path_len) + 1);
        }

        if (sizeof(parent_branch_name_ptr) <= buffer_size) {
            strcpy(buffer, parent_branch_name_ptr);
            rc = 0;
        }
    }

    git_reference_free(head_ref);
    return rc;
}


int get_current_branch_name(git_repository* repo, char* buffer, const size_t buffer_size) {
    git_reference *head;
    git_repository_head(&head, repo);

    const char* branch_name = git_reference_shorthand(head);

    git_reference_free(head);
    if (sizeof(branch_name) > buffer_size) {
        return 1;
    }
    strcpy(buffer, branch_name);
    return 0;
}

int get_git_current_branch_status(git_repository* repo, const char* branch_name, const char* remote_name, GitStatusNumbers* statusNumbers) {
    size_t ahead, behind;
    git_oid local_oid, remote_oid;

    // Get OIDs for both branches
    git_reference *local_ref, *remote_ref;

    char local_branch_path[MAX_BRANCH_PATH_LENGTH];
    sprintf(local_branch_path, "refs/heads/%s", branch_name);
    char remote_branch_path[MAX_BRANCH_NAME_LENGTH];
    sprintf(remote_branch_path, "refs/heads/%s/%s", remote_name, branch_name);

    git_reference_lookup(&local_ref, repo, local_branch_path);
    git_reference_lookup(&remote_ref, repo, remote_branch_path);

    git_reference_name_to_id(&local_oid, repo, git_reference_name(local_ref));
    git_reference_name_to_id(&remote_oid, repo, git_reference_name(remote_ref));

    git_graph_ahead_behind(&ahead, &behind, repo, &local_oid, &remote_oid);

    statusNumbers->current_ahead = ahead;
    statusNumbers->current_behind = behind;
    if (ahead != 0 || behind != 0) {
        statusNumbers->branch_out_of_sync = 1;
    }
    git_reference_free(local_ref);
    git_reference_free(remote_ref);
    return 0;
}

int get_git_default_branch_status(git_repository* repo, GitStatusNumbers* statusNumbers) {
    size_t ahead, behind;
    git_oid head_oid, upstream_oid;

    git_reference* head;
    git_repository_head(&head, repo);

    git_reference* upstream;
    git_branch_upstream(&upstream, head);

    git_reference_name_to_id(&head_oid, repo, git_reference_name(head));
    git_reference_name_to_id(&upstream_oid, repo, git_reference_name(upstream));

    git_graph_ahead_behind(&ahead, &behind, repo, &head_oid, &upstream_oid);

    statusNumbers->default_ahead = ahead;
    statusNumbers->default_behind = behind;
    if (ahead != 0 || behind != 0) {
        statusNumbers->branch_out_of_sync = 1;
    }
    git_reference_free(head);
    git_reference_free(upstream);
    return 0;
}



