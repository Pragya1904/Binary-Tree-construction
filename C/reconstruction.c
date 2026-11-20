#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "reconstruction.h"

static bool has_duplicates(char **tokens, int count) {
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(tokens[i], tokens[j]) == 0) {
                return true;
            }
        }
    }
    return false;
}

static bool have_same_elements(char **preorder, char **postorder, int count) {
    for (int i = 0; i < count; i++) {
        bool found = false;
        for (int j = 0; j < count; j++) {
            if (strcmp(preorder[i], postorder[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

static TreeNode build_tree(char **preorder, int pre_start, int pre_end,
                           char **postorder, int post_start, int post_end,
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

    TreeNode root = create_node(root_val);
    if (!root) {
        snprintf(err_msg, err_size, "Memory allocation error");
        return NULL;
    }

    if (pre_start == pre_end) {
        return root;
    }

    const char *left_root = preorder[pre_start + 1];

    /* Find the left subtree root in postorder using linear search. */
    int idx = -1;
    for (int i = post_start; i < post_end; i++) {
        if (strcmp(postorder[i], left_root) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        snprintf(err_msg, err_size, "Left child '%s' not found in postorder segment", left_root);
        free_tree(root);
        return NULL;
    }

    int left_size = idx - post_start + 1;

    TreeNode left_child = build_tree(preorder, pre_start + 1, pre_start + left_size,
                                     postorder, post_start, idx,
                                     err_msg, err_size);
    if (!left_child && (pre_start + 1 <= pre_start + left_size)) {
        free_tree(root);
        return NULL;
    }
    set_left(root, left_child);

    TreeNode right_child = build_tree(preorder, pre_start + left_size + 1, pre_end,
                                      postorder, idx + 1, post_end - 1,
                                      err_msg, err_size);
    if (!right_child && (pre_start + left_size + 1 <= pre_end)) {
        free_tree(root);
        return NULL;
    }
    set_right(root, right_child);

    return root;
}

TreeNode reconstruct_tree(char **preorder, int pre_count,
                         char **postorder, int post_count,
                         char *err_msg, size_t err_size) {
    err_msg[0] = '\0';

    if (pre_count != post_count) {
        snprintf(err_msg, err_size, "Length mismatch: preorder (%d) != postorder (%d)",
                 pre_count, post_count);
        return NULL;
    }

    if (pre_count == 0) {
        return NULL;
    }

    if (has_duplicates(preorder, pre_count)) {
        snprintf(err_msg, err_size, "Duplicate node values detected in preorder traversal");
        return NULL;
    }

    if (has_duplicates(postorder, post_count)) {
        snprintf(err_msg, err_size, "Duplicate node values detected in postorder traversal");
        return NULL;
    }

    if (!have_same_elements(preorder, postorder, pre_count)) {
        snprintf(err_msg, err_size, "Elements in preorder and postorder traversals do not match");
        return NULL;
    }

    return build_tree(preorder, 0, pre_count - 1,
                      postorder, 0, post_count - 1,
                      err_msg, err_size);
}
