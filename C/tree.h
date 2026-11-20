#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

typedef struct TreeNodeRep *TreeNode;

TreeNode create_node(const char *val);
void free_tree(TreeNode root);

void set_left(TreeNode node, TreeNode left);
void set_right(TreeNode node, TreeNode right);

const char *get_value(TreeNode node);
TreeNode get_left(TreeNode node);
TreeNode get_right(TreeNode node);

#endif
