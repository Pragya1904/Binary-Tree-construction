# C Implementation — Tree Construction from Preorder and Postorder

This directory contains the C implementation of the binary tree reconstruction algorithm, structured using a modular Abstract Data Type (ADT) design.

## File Organisation

* `tree.h`: Public Tree ADT interface with opaque pointer encapsulation (`typedef struct TreeNodeRep *TreeNode;`).
* `tree.c`: Private node representation, allocation (`create_node`), accessor functions (`get_value`, `get_left`, `get_right`), and recursive memory deallocation (`free_tree`).
* `reconstruction.h`: Interface for the tree reconstruction algorithm.
* `reconstruction.c`: Input validation and recursive subtree partitioning using linear search.
* `solution.c`: CLI entry point, dynamic input tokenization, and canonical traversal output.

## Compilation

Compile all modules using a C99/C11 compatible compiler:

```bash
clang -O2 -Wall -Wextra tree.c reconstruction.c solution.c -o solution
```

or with GCC:

```bash
gcc -O2 -Wall -Wextra tree.c reconstruction.c solution.c -o solution
```

## Running

```bash
./solution << 'EOF'
A B D E C F G
D E B F G C A
EOF
```
