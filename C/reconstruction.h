#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include <stddef.h>
#include "tree.h"

TreeNode reconstruct_tree(char **preorder, int pre_count,
                         char **postorder, int post_count,
                         char *err_msg, size_t err_size);

#endif /* RECONSTRUCTION_H */
