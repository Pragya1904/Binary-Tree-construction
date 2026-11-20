#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"
#include "reconstruction.h"

#define INITIAL_CAPACITY 64

static char **tokenize_line(const char *line, int *out_count) {
    *out_count = 0;
    if (!line) {
        return NULL;
    }

    int capacity = INITIAL_CAPACITY;
    char **tokens = malloc(capacity * sizeof(char *));
    if (!tokens) {
        return NULL;
    }

    char *copy = strdup(line);
    if (!copy) {
        free(tokens);
        return NULL;
    }

    char *token = strtok(copy, " \t\r\n");
    while (token) {
        if (*out_count >= capacity) {
            capacity *= 2;
            char **new_tokens = realloc(tokens, capacity * sizeof(char *));
            if (!new_tokens) {
                for (int i = 0; i < *out_count; i++) {
                    free(tokens[i]);
                }
                free(tokens);
                free(copy);
                return NULL;
            }
            tokens = new_tokens;
        }

        tokens[*out_count] = strdup(token);
        if (!tokens[*out_count]) {
            break;
        }
        (*out_count)++;
        token = strtok(NULL, " \t\r\n");
    }

    free(copy);
    return tokens;
}

static void free_tokens(char **tokens, int count) {
    if (!tokens) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

static void print_preorder(TreeNode root, bool *first) {
    if (!root) {
        printf("%s#", *first ? "" : " ");
        *first = false;
        return;
    }

    printf("%s%s", *first ? "" : " ", get_value(root));
    *first = false;
    print_preorder(get_left(root), first);
    print_preorder(get_right(root), first);
}

static void print_postorder(TreeNode root, bool *first) {
    if (!root) {
        printf("%s#", *first ? "" : " ");
        *first = false;
        return;
    }

    print_postorder(get_left(root), first);
    print_postorder(get_right(root), first);
    printf("%s%s", *first ? "" : " ", get_value(root));
    *first = false;
}

int main(void) {
    char *lines[3] = {NULL, NULL, NULL};
    size_t line_caps[3] = {0, 0, 0};
    int line_count = 0;

    while (line_count < 3) {
        ssize_t read = getline(&lines[line_count], &line_caps[line_count], stdin);
        if (read == -1) {
            break;
        }
        line_count++;
    }

    const char *pre_line = "";
    const char *post_line = "";

    if (line_count > 0 && lines[0]) {
        char *copy = strdup(lines[0]);
        char *first_tok = strtok(copy, " \t\r\n");
        if (first_tok && (strcasecmp(first_tok, "base") == 0 ||
                          strcasecmp(first_tok, "full") == 0 ||
                          strcasecmp(first_tok, "null-marked") == 0 ||
                          strcasecmp(first_tok, "default") == 0)) {
            pre_line = (line_count > 1 && lines[1]) ? lines[1] : "";
            post_line = (line_count > 2 && lines[2]) ? lines[2] : "";
        } else {
            pre_line = lines[0];
            post_line = (line_count > 1 && lines[1]) ? lines[1] : "";
        }
        free(copy);
    }

    int pre_count = 0;
    int post_count = 0;
    char **preorder = tokenize_line(pre_line, &pre_count);
    char **postorder = tokenize_line(post_line, &post_count);

    char err_msg[256] = {0};
    TreeNode root = reconstruct_tree(preorder, pre_count, postorder, post_count, err_msg, sizeof(err_msg));

    if (!root && err_msg[0] != '\0') {
        printf("STATUS: ERROR\n");
        printf("ERROR: %s\n", err_msg);
    } else {
        printf("STATUS: SUCCESS\n");
        printf("PREORDER: ");
        bool first_pre = true;
        print_preorder(root, &first_pre);
        printf("\n");

        printf("POSTORDER: ");
        bool first_post = true;
        print_postorder(root, &first_post);
        printf("\n");

        free_tree(root);
    }

    free_tokens(preorder, pre_count);
    free_tokens(postorder, post_count);
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }

    return 0;
}
