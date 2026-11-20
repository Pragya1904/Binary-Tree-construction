/**
 * DevDictates - Binary Tree Construction from Preorder and Postorder Traversals.
 * Language: C (C99 / C11)
 * Approach: Base Recursive Subtree Partitioning (Linear Search, O(n^2) worst case, O(h) auxiliary space)
 * Author: Pragya
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TOKEN_LEN 64
#define MAX_TOKENS 1024
#define BUFFER_SIZE 65536

typedef struct Node {
    char val[MAX_TOKEN_LEN];
    struct Node *left;
    struct Node *right;
} Node;

Node *create_node(const char *val) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    strncpy(n->val, val, MAX_TOKEN_LEN - 1);
    n->val[MAX_TOKEN_LEN - 1] = '\0';
    n->left = NULL;
    n->right = NULL;
    return n;
}

void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void serialize_preorder_with_nulls(Node *root, char tokens[][MAX_TOKEN_LEN], int *count) {
    if (!root) {
        strncpy(tokens[*count], "#", MAX_TOKEN_LEN - 1);
        tokens[*count][MAX_TOKEN_LEN - 1] = '\0';
        (*count)++;
        return;
    }
    strncpy(tokens[*count], root->val, MAX_TOKEN_LEN - 1);
    tokens[*count][MAX_TOKEN_LEN - 1] = '\0';
    (*count)++;
    serialize_preorder_with_nulls(root->left, tokens, count);
    serialize_preorder_with_nulls(root->right, tokens, count);
}

void serialize_postorder_with_nulls(Node *root, char tokens[][MAX_TOKEN_LEN], int *count) {
    if (!root) {
        strncpy(tokens[*count], "#", MAX_TOKEN_LEN - 1);
        tokens[*count][MAX_TOKEN_LEN - 1] = '\0';
        (*count)++;
        return;
    }
    serialize_postorder_with_nulls(root->left, tokens, count);
    serialize_postorder_with_nulls(root->right, tokens, count);
    strncpy(tokens[*count], root->val, MAX_TOKEN_LEN - 1);
    tokens[*count][MAX_TOKEN_LEN - 1] = '\0';
    (*count)++;
}

Node *build_tree_recursive(char preorder[][MAX_TOKEN_LEN], int pre_start, int pre_end,
                           char postorder[][MAX_TOKEN_LEN], int post_start, int post_end,
                           char *err_msg, size_t err_size) {
    if (pre_start > pre_end) {
        return NULL;
    }

    const char *root_val = preorder[pre_start];
    if (strcmp(postorder[post_end], root_val) != 0) {
        snprintf(err_msg, err_size, "Root mismatch: preorder root '%s' != postorder root '%s'",
                 root_val, postorder[post_end]);
        return NULL;
    }

    Node *root = create_node(root_val);
    if (pre_start == pre_end) {
        return root;
    }

    const char *left_root_val = preorder[pre_start + 1];

    // Linear search in postorder[post_start .. post_end - 1]
    int idx = -1;
    for (int i = post_start; i < post_end; i++) {
        if (strcmp(postorder[i], left_root_val) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        snprintf(err_msg, err_size, "Left child '%s' not found in postorder segment", left_root_val);
        free_tree(root);
        return NULL;
    }

    int left_size = idx - post_start + 1;

    root->left = build_tree_recursive(preorder, pre_start + 1, pre_start + left_size,
                                      postorder, post_start, idx,
                                      err_msg, err_size);
    if (!root->left && (pre_start + 1 <= pre_start + left_size)) {
        free_tree(root);
        return NULL;
    }

    root->right = build_tree_recursive(preorder, pre_start + left_size + 1, pre_end,
                                       postorder, idx + 1, post_end - 1,
                                       err_msg, err_size);
    if (!root->right && (pre_start + left_size + 1 <= pre_end)) {
        free_tree(root);
        return NULL;
    }

    return root;
}

int tokenize_line(char *line, char tokens[][MAX_TOKEN_LEN], int max_tokens) {
    int count = 0;
    char *token = strtok(line, " \t\r\n");
    while (token != NULL && count < max_tokens) {
        strncpy(tokens[count], token, MAX_TOKEN_LEN - 1);
        tokens[count][MAX_TOKEN_LEN - 1] = '\0';
        count++;
        token = strtok(NULL, " \t\r\n");
    }
    return count;
}

bool check_distinct(char tokens[][MAX_TOKEN_LEN], int count) {
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(tokens[i], tokens[j]) == 0) {
                return false;
            }
        }
    }
    return true;
}

bool check_sets_match(char pre[][MAX_TOKEN_LEN], char post[][MAX_TOKEN_LEN], int count) {
    for (int i = 0; i < count; i++) {
        bool found = false;
        for (int j = 0; j < count; j++) {
            if (strcmp(pre[i], post[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

int main(void) {
    char line1[BUFFER_SIZE] = {0};
    char line2[BUFFER_SIZE] = {0};
    char line3[BUFFER_SIZE] = {0};

    char *lines[3] = {NULL, NULL, NULL};
    char raw_buf[BUFFER_SIZE];
    int line_count = 0;

    while (fgets(raw_buf, sizeof(raw_buf), stdin) && line_count < 3) {
        // Strip trailing newline/spaces
        size_t len = strlen(raw_buf);
        while (len > 0 && (raw_buf[len - 1] == '\n' || raw_buf[len - 1] == '\r')) {
            raw_buf[--len] = '\0';
        }
        if (line_count == 0) {
            strncpy(line1, raw_buf, BUFFER_SIZE - 1);
            lines[0] = line1;
        } else if (line_count == 1) {
            strncpy(line2, raw_buf, BUFFER_SIZE - 1);
            lines[1] = line2;
        } else if (line_count == 2) {
            strncpy(line3, raw_buf, BUFFER_SIZE - 1);
            lines[2] = line3;
        }
        line_count++;
    }

    char *pre_line = "";
    char *post_line = "";

    if (line_count > 0 && lines[0]) {
        // Check if line 1 is a mode keyword
        char temp[BUFFER_SIZE];
        strncpy(temp, lines[0], BUFFER_SIZE - 1);
        char *first_tok = strtok(temp, " \t\r\n");
        if (first_tok && (strcasecmp(first_tok, "base") == 0 ||
                          strcasecmp(first_tok, "full") == 0 ||
                          strcasecmp(first_tok, "null-marked") == 0)) {
            pre_line = (line_count > 1 && lines[1]) ? lines[1] : "";
            post_line = (line_count > 2 && lines[2]) ? lines[2] : "";
        } else {
            pre_line = lines[0];
            post_line = (line_count > 1 && lines[1]) ? lines[1] : "";
        }
    }

    static char pre_tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    static char post_tokens[MAX_TOKENS][MAX_TOKEN_LEN];

    int pre_count = tokenize_line(pre_line, pre_tokens, MAX_TOKENS);
    int post_count = tokenize_line(post_line, post_tokens, MAX_TOKENS);

    if (pre_count != post_count) {
        printf("STATUS: ERROR\n");
        printf("ERROR: Length mismatch: preorder (%d) != postorder (%d)\n", pre_count, post_count);
        return 0;
    }

    if (pre_count == 0) {
        printf("STATUS: SUCCESS\n");
        printf("PREORDER: #\n");
        printf("POSTORDER: #\n");
        return 0;
    }

    if (!check_distinct(pre_tokens, pre_count)) {
        printf("STATUS: ERROR\n");
        printf("ERROR: Duplicate node values detected in preorder traversal\n");
        return 0;
    }

    if (!check_distinct(post_tokens, post_count)) {
        printf("STATUS: ERROR\n");
        printf("ERROR: Duplicate node values detected in postorder traversal\n");
        return 0;
    }

    if (!check_sets_match(pre_tokens, post_tokens, pre_count)) {
        printf("STATUS: ERROR\n");
        printf("ERROR: Elements in preorder and postorder traversals do not match\n");
        return 0;
    }

    char err_msg[256] = {0};
    Node *root = build_tree_recursive(pre_tokens, 0, pre_count - 1,
                                      post_tokens, 0, post_count - 1,
                                      err_msg, sizeof(err_msg));

    if (!root) {
        printf("STATUS: ERROR\n");
        printf("ERROR: %s\n", err_msg[0] ? err_msg : "Reconstruction failed");
        return 0;
    }

    static char out_pre[MAX_TOKENS * 3][MAX_TOKEN_LEN];
    static char out_post[MAX_TOKENS * 3][MAX_TOKEN_LEN];
    int out_pre_count = 0;
    int out_post_count = 0;

    serialize_preorder_with_nulls(root, out_pre, &out_pre_count);
    serialize_postorder_with_nulls(root, out_post, &out_post_count);

    printf("STATUS: SUCCESS\n");
    printf("PREORDER: ");
    for (int i = 0; i < out_pre_count; i++) {
        printf("%s%s", out_pre[i], (i == out_pre_count - 1) ? "" : " ");
    }
    printf("\n");

    printf("POSTORDER: ");
    for (int i = 0; i < out_post_count; i++) {
        printf("%s%s", out_post[i], (i == out_post_count - 1) ? "" : " ");
    }
    printf("\n");

    free_tree(root);
    return 0;
}
