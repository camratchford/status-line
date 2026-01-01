#include "text_formatting.h"
#include "ansi.h"
#include <unistd.h>
#include <stdio.h>
#include <libgen.h>

#define NAME_SIZE 256


int get_prompt_line(char* buffer, const char* hostname, char* dirname, const struct passwd* pw) {
    char* base_name;
    if (strcmp(dirname, pw->pw_dir) == 0) {
        base_name = "~";
    } else {
        base_name = basename(dirname);
    }

    sprintf(buffer, TEAL "%s" BEIGE "@" GREEN "%s" RED " : " PURPLE "%s", pw->pw_name, hostname, base_name);
    return 0;
}

int get_git_status_line(char* buffer, const size_t buffer_size, git_repository* repo) {
    GitStatusNumbers status = {};

    char current_branch_name[MAX_BRANCH_NAME_LENGTH] = "";
    char default_branch_name[MAX_BRANCH_NAME_LENGTH] = "";
    char remote_name[MAX_REMOTE_NAME_SIZE] = "";

    get_current_branch_name(repo, current_branch_name, MAX_BRANCH_NAME_LENGTH);

    get_default_branch_name(repo, default_branch_name, MAX_BRANCH_NAME_LENGTH);
    get_remote_name(repo, remote_name, MAX_REMOTE_NAME_SIZE);

    get_git_status(repo, &status);
    get_git_current_branch_status(repo, current_branch_name, remote_name, &status);
    get_git_default_branch_status(repo, &status);


    if (status.has_changed == 0) {
        snprintf(buffer, buffer_size, GREEN CHECK_MARK RESET);
        return 0;
    }

    char* ptr = buffer;
    size_t remaining = buffer_size;
    int written;
    if (status.untracked > 0) {
        written = snprintf(ptr, remaining, WHITE "[" BEIGE "%s ", current_branch_name);
        ptr += written;
        remaining -= written;
    }
    if (status.branch_out_of_sync) {
        if (status.current_ahead > 0 && remaining > 0) {
            written = snprintf(ptr, remaining, PURPLE UP_DOUBLE_ARROW "%u", status.current_ahead);
            ptr += written;
            remaining -= written;
        }
        if (status.current_behind > 0 && remaining > 0) {
            written = snprintf(ptr, remaining, PURPLE DOWN_DOUBLE_ARROW "%u", status.current_behind);
            ptr += written;
            remaining -= written;
        }
        if (status.default_ahead > 0 && remaining > 0) {
            written = snprintf(ptr, remaining, TEAL UP_ARROW "%u", status.default_ahead);
            ptr += written;
            remaining -= written;
        }
        if (status.default_behind > 0 && remaining > 0) {
            written = snprintf(ptr, remaining, TEAL DOWN_ARROW "%u", status.default_behind);
            ptr += written;
            remaining -= written;
        }
    }

    if (status.untracked > 0 && remaining > 0) {
        written = snprintf(ptr, remaining, LIGHT_RED "?%u", status.untracked);
        ptr += written;
        remaining -= written;
    }
    if (status.added > 0 && remaining > 0) {
        written = snprintf(ptr, remaining, LIGHT_RED "+%u", status.added);
        ptr += written;
        remaining -= written;
    }
    if (status.deleted > 0 && remaining > 0) {
        written = snprintf(ptr, remaining, LIGHT_RED "-%u", status.deleted);
        ptr += written;
        remaining -= written;
    }
    if (status.modified > 0 && remaining > 0) {
        written = snprintf(ptr, remaining, LIGHT_RED "~%u", status.modified);
        ptr += written;
        remaining -= written;
    }
    if (remaining > 0) {
        written = snprintf(ptr, remaining, WHITE "]");
        ptr += written;
        remaining -= written;
    }

    if (remaining == 0) {
        snprintf(buffer, buffer_size, "MemError");
        return 1;
    }

    return 0;
}

int get_git_branch_data_line(char* buffer, const size_t buffer_size, git_repository* repo) {

    return 0;
}

void get_uid_char(char* buffer, const __uid_t uid) {
    if (uid == 0) {
        sprintf(buffer, "%s# %s", RED, RESET);
    }
    sprintf(buffer, "%s$ %s", GREEN, RESET);
}
