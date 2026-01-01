#include <stdio.h>
#include <unistd.h>
#include <git2.h>
#include "text_formatting.h"
#include "git_functions.h"

#define PS1_SIZE 2048
#define STATUSLINE_SIZE 512
#define GIT_STATUS_SIZE 512
#define PROMPT_LINE_SIZE 256
#define DIRNAME_SIZE 1024
#define HOSTNAME_SIZE 128

int set_status_line_var(const char* hostname, char* dirname, const struct passwd* pw) {
    int err_level = 0;
    git_repository* repo = NULL;
    git_repository_open(&repo, dirname);

    const git_error_code git_repo_state = is_git_repo(repo);
    char ps1[PS1_SIZE] = "";
    char prompt_line[PROMPT_LINE_SIZE] = "";
    char git_status_line[GIT_STATUS_SIZE] = "";

    char* ptr = ps1;
    size_t remaining = PS1_SIZE;

    get_prompt_line(prompt_line, hostname, dirname, pw);
    printf("%s", prompt_line);

    // Repo exists
    if (git_repo_state == 0) {
        get_git_status_line(git_status_line, STATUSLINE_SIZE, repo);
        printf(" %s", git_status_line);
    }

    char uid_char[64];
    get_uid_char(uid_char, pw->pw_uid);

    printf(" %s", uid_char);
    git_repository_free(repo);
    return err_level;
}

void set_term_title(const struct passwd* pw, char* dirname) {
    printf("\033]0;%s:%s - %s\007", pw->pw_shell, pw->pw_name, dirname);
}

int main(void) {
    git_libgit2_init();

    int err_level = 0;

    char dirname[DIRNAME_SIZE];
    getcwd(dirname, DIRNAME_SIZE);

    char hostname[HOSTNAME_SIZE];
    gethostname(hostname, HOSTNAME_SIZE);
    __uid_t uid = getuid();
    const struct passwd* pw = getpwuid(uid);

    err_level += set_status_line_var(hostname, dirname, pw);
    // err_level += set_term_title(pw, dirname);

    return err_level;
}