#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>


#include <git2.h>
#include <git2/errors.h>


#include "git_functions.h"

#define TEST_CASE_GET_GIT_REPO_PASS "/home/cam/dotfiles"
#define TEST_CASE_GET_GIT_REPO_FAIL "/home/cam/"

#define TEST_CASE_IS_GIT_REPO_PASS "/home/cam/dotfiles"
#define TEST_CASE_IS_GIT_REPO_FAIL "/home/cam/"

#ifndef VERBOSE_TEST_OUTPUT
    #define VERBOSE_TEST_OUTPUT 1
#endif

void test_get_git_repo() {
    git_repository* repo_pass = NULL;
    const int rc_pass = get_git_repo(&repo_pass, TEST_CASE_GET_GIT_REPO_PASS);
    assert(rc_pass == GIT_OK);
    git_repository_free(repo_pass);

    git_repository* repo_fail = NULL;
    const int rc_fail = get_git_repo(&repo_fail, TEST_CASE_GET_GIT_REPO_FAIL);
    assert(rc_fail == GIT_ENOTFOUND);
    git_repository_free(repo_fail);
}

void test_is_git_repo() {
    git_repository* repo_pass = NULL;
    chdir(TEST_CASE_IS_GIT_REPO_PASS);
    const int err_pass = is_git_repo(repo_pass);
    assert(err_pass == 0);
    // This function does free the repo

    git_repository* repo_fail = NULL;
    chdir(TEST_CASE_IS_GIT_REPO_FAIL);
    const int err_fail = is_git_repo(repo_fail);
    assert(err_fail == 1);
}

void test_initialize_git_status_numbers() {
    GitStatusNumbers status = {};
    initialize_git_status_numbers(&status);
}

void test_get_git_status() {
    // chdir(TEST_CASE_IS_GIT_REPO_PASS);
    GitStatusNumbers status = {};
    initialize_git_status_numbers(&status);
    git_repository* repo = NULL;
    get_git_repo(&repo, TEST_CASE_IS_GIT_REPO_PASS);
    get_git_status(repo, &status);
}

void print_git_errors() {
    if (! VERBOSE_TEST_OUTPUT) {
        return;
    }
    const git_error* err = git_error_last();
    if (err != NULL) {
        printf("%s\n", err->message);
    }
}


int main(const int argc, char** argv) {


    git_libgit2_init();
    print_git_errors();

    test_get_git_repo();
    print_git_errors();

    test_is_git_repo();
    print_git_errors();

    test_initialize_git_status_numbers();

    test_get_git_status();
    print_git_errors();

}