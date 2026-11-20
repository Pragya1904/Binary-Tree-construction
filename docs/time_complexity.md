# Time and Space Complexity Analysis

This document provides the definitive mathematical analysis of the time and space complexity for reconstructing a binary tree from its traversal sequences, as presented in the [DevDictates](https://youtube.com) educational series.

---

## 1. Parameters and Notation

Throughout this analysis:
* $n$: The number of actual nodes in the binary tree.
* $h$: The height of the binary tree ($h \in [\lfloor \log_2 n \rfloor, n]$).
* $k$: The number of nodes in a given subtree during recursion.
* $s$: The length of the postorder array segment searched at a given recursive step.

We distinguish between two distinct reconstruction approaches:
1. **Approach 1:** Base recursive subtree partitioning with linear search (under the Full Binary Tree assumption).
2. **Approach 2:** Reconstruction using explicit null markers.

We also strictly separate **auxiliary working memory** (call stack and temporary structures) from **input storage** and **output tree storage**.

---

## 2. Approach 1: Base Recursive Partitioning with Linear Search

### 2.1 Per-Call Operations

At each recursive call processing a subtree of size $k$:

| Operation | Time Complexity | Details |
| :--- | :---: | :--- |
| Extract and validate root | O(1) | Compare `preorder[pre_start]` with `postorder[post_end]` |
| Identify left root candidate | O(1) | Read `preorder[pre_start + 1]` |
| Search for left root in postorder | O(s) | Linear scan over range of size $s = k - 1$ |
| Compute partition boundary | O(1) | `left_size = idx - post_start + 1` |
| Recursive calls | Subproblems | Recurse on left and right subtrees |

Every node is allocated and initialized exactly once, contributing $O(n)$ total node-creation work. The dominant operational cost at each level is the linear search.

At any call processing a subtree of size $k$:

$$
T_{\text{search}}(k) = O(k)
$$

### 2.2 Worst-Case Time Complexity: $O(n^2)$

Under the Full Binary Tree assumption, every internal node must have exactly two children. A degenerate single-child chain is **not** a valid full binary tree.

Instead, the worst-case occurs in a **highly unbalanced Full Binary Tree** (such as a comb-shaped or caterpillar full tree). In this structure, every internal node has:
* One leaf child (1 node).
* One larger subtree containing the remaining $n - 2$ nodes.

At each recursive level, the linear search inspects the postorder segment of the larger subtree before finding the boundary. The recurrence relation is:

$$
T(n) = T(n - 2) + O(n)
$$

Expanding this recurrence:

$$
T(n) = O(n) + O(n - 2) + O(n - 4) + \cdots + O(1)
$$

This forms an arithmetic progression with approximately $n / 2$ terms:

$$
T(n) = O\left(\sum_{i=0}^{(n-1)/2} (n - 2i)\right) = O(n^2)
$$

Thus, the worst-case time complexity is $O(n^2)$.

### 2.3 Balanced-Tree Time Complexity: $O(n \log n)$

When the Full Binary Tree is balanced, each recursive step divides the current subtree of size $k$ into two subtrees of approximately equal size ($\approx k / 2$).

The recurrence relation is:

$$
T(n) = 2T(n/2) + O(n)
$$

Applying Case 2 of the **Master Theorem** ($a = 2, b = 2, f(n) = O(n)$ where $n^{\log_b a} = n^{\log_2 2} = n^1$):

$$
T(n) = O(n \log n)
$$

*Note: This is designated as the balanced-tree complexity. Average-case complexity is not formally stated here, as doing so requires defining an explicit probability distribution over the input tree topologies.*

### 2.4 Educational Rationale: Linear Search vs. Hash Map

Precomputing a hash map of node values to their postorder indices allows $O(1)$ boundary lookups, reducing overall time to $O(n)$.

However, this implementation intentionally uses **linear search**:
1. **Conceptual Transparency:** It directly demonstrates physical subarray scanning and boundary tracking.
2. **Language Parity:** It avoids external collection dependencies, maintaining identical semantics across C, C++, Java, and Python.
3. **Pedagogical Value:** Experiencing the $O(n^2)$ search cost provides a clear, motivated justification for why hash maps are introduced in advanced optimizations.

### 2.5 Space Complexity (Approach 1)

Memory is categorized into three distinct components:

1. **Auxiliary Stack Space ($S_{\text{auxiliary}} = O(h)$):**
   The algorithm passes index boundaries (`pre_start`, `pre_end`, `post_start`, `post_end`) rather than copying subarrays. The only auxiliary memory consumed is the recursion call stack:
   * **Balanced Tree:** Height $h = O(\log n)$, giving $O(\log n)$ stack depth.
   * **Worst-Case Comb Tree:** Height $h = O(n)$, giving $O(n)$ stack depth.

   Therefore:

   $$
   S_{\text{auxiliary}} = O(h)
   $$

2. **Input Storage ($O(n)$):**
   The input preorder and postorder traversal arrays are provided by the caller and read in-place. This storage exists prior to execution and is excluded from auxiliary space.

3. **Output Tree Storage ($O(n)$):**
   The algorithm dynamically constructs $n$ binary tree nodes. Output storage is required to return the result and is accounted for separately from auxiliary memory.

---

## 3. Approach 2: Reconstruction Using Explicit Null Markers

### 3.1 Time Complexity: $O(n)$

In this approach, missing children are explicitly encoded in traversals using a null marker (such as `#`).

For a binary tree containing $n$ actual nodes:

$$
\text{Null child positions} = n + 1
$$

The total number of tokens in the augmented traversal sequence is:

$$
n + (n + 1) = 2n + 1
$$

The reconstruction algorithm processes each token in preorder:
* If the token is `#`, it immediately returns `NULL` ($O(1)$).
* If the token is a value, it creates a node and recurses on left and right subtrees ($O(1)$).
* Each token is visited and consumed a constant number of times.
* Subtree boundaries are determined structurally by the null markers, completely eliminating the linear search.

Therefore, the reconstruction time complexity is:

$$
T(n) = O(2n + 1) = O(n)
$$

### 3.2 Auxiliary Space Complexity (Approach 2)

We must distinguish between reconstruction alone and reconstruction with stored postorder validation:

1. **Reconstruction Alone ($O(h)$):**
   When consuming the preorder token stream using a pointer or index, only the recursion call stack is used:

   $$
   S_{\text{reconstruction}} = O(h)
   $$

   * Balanced tree: $O(\log n)$.
   * Skewed tree: $O(n)$.

2. **Reconstruction with Stored Postorder Validation ($O(n)$):**
   If the implementation serializes the reconstructed tree into an in-memory postorder token list to compare against the supplied augmented postorder sequence, that temporary list requires $O(n)$ working space:

   $$
   S_{\text{total auxiliary}} = O(n + h) = O(n)
   $$

   *Note: If validation is performed via an in-order streaming iterator without materializing a second sequence, auxiliary space remains $O(h)$. Implementations that store the generated sequence must report $O(n)$ auxiliary working memory.*

---

## 4. Final Complexity Comparison

| Metric | Base Recursive (Linear Search) | Hash Map Optimized | Explicit Null Markers |
| :--- | :---: | :---: | :---: |
| **Subtree Boundary Identification** | Linear search: O(s) | Hash map lookup: O(1) | Encoded in null markers |
| **Worst-Case Time** | O(n²) | O(n) | O(n) |
| **Balanced-Tree Time** | O(n log n) | O(n) | O(n) |
| **Auxiliary Space (Stack Only)** | O(h) | O(h) | O(h) |
| **Auxiliary Space (with Stored Validation)** | N/A | N/A | O(n) |
| **Additional Lookup Storage** | None (O(1)) | O(n) hash table | None (O(1)) |
| **Output Space** | O(n) | O(n) | O(n) |
| **Reconstruction Uniqueness** | Requires Full Tree assumption | Requires Full Tree assumption | Reconstructs arbitrary binary trees |

---

## 5. Related Documentation

* For the step-by-step algorithm and trace, see [Algorithm Walkthrough](algorithm.md).
* For single-child ambiguity and uniqueness constraints, see [Structural Limitations](limitations.md).
