#ifndef STATUS_LINE_TEXT_FORMATTING_H
#define STATUS_LINE_TEXT_FORMATTING_H
#include "git_functions.h"
#include <pwd.h>
#include <string.h>

// Composes the '${username}@${hostname} : ${PWD}' portion of the terminal prompt
int get_prompt_line(char* buffer, const char* hostname, char* dirname, const struct passwd* pw);

// Composes the git status portion of the terminal prompt (ex. '[main +1~6?1]')
int get_git_status_line(char* buffer, size_t buffer_size, git_repository* repo);

// Composes the git branch status portion of the terminal prompt
int get_git_branch_data_line(char* buffer, size_t buffer_size, git_repository* repo);

// Composes the # or $ portion of the terminal prompt based on UID
void get_uid_char(char* buffer, __uid_t uid) ;

#endif