#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

struct TreeNodeRep {
    char *val;
    struct TreeNodeRep *left;
    struct TreeNodeRep *right;
};

TreeNode create_node(const char *val) {
    if (!val) {
        return NULL;
    }

    TreeNode node = malloc(sizeof(struct TreeNodeRep));
    if (!node) {
        return NULL;
    }

    node->val = strdup(val);
    if (!node->val) {
        free(node);
        return NULL;
    }

    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(TreeNode root) {
    if (!root) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root->val);
    free(root);
}

void set_left(TreeNode node, TreeNode left) {
    if (node) {
        node->left = left;
    }
}

void set_right(TreeNode node, TreeNode right) {
    if (node) {
        node->right = right;
    }
}

const char *get_value(TreeNode node) {
    return node ? node->val : NULL;
}

TreeNode get_left(TreeNode node) {
    return node ? node->left : NULL;
}

TreeNode get_right(TreeNode node) {
    return node ? node->right : NULL;
}
