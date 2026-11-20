# Complexity Analysis

This document provides a mathematical breakdown of the time and space complexity for the recursive binary tree reconstruction algorithm implemented in DevDictates.

---

## 1. Summary Comparison

| Metric | Base Recursive (Linear Search) | Hash Map Optimized |
| :--- | :---: | :---: |
| **Worst-Case Time** | $\mathcal{O}(n^2)$ | $\mathcal{O}(n)$ |
| **Best-Case Time (Balanced Tree)** | $\mathcal{O}(n \log n)$ | $\mathcal{O}(n)$ |
| **Average-Case Time** | $\mathcal{O}(n \log n)$ | $\mathcal{O}(n)$ |
| **Auxiliary Space (Call Stack)** | $\mathcal{O}(h)$ | $\mathcal{O}(n)$ (due to map) |
| **Output Space** | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |

*Here, $n$ denotes the total number of nodes, and $h$ denotes the height of the tree ($h \in [\lfloor \log_2 n \rfloor, n]$).*

---

## 2. Time Complexity Breakdown

### 2.1 Per-Node Work

At each recursive invocation processing a subtree of size $k$:
1. **Root Extraction & Validation:** $\mathcal{O}(1)$ operations.
2. **Boundary Search:** A linear search is performed across the postorder segment of size $k - 1$ to find the index of the left child root. In the worst case, this requires $k - 1$ comparisons:
   $$T_{\text{search}}(k) = \mathcal{O}(k)$$
3. **Partitioning:** Calculating boundary indices requires $\mathcal{O}(1)$ arithmetic.

### 2.2 Worst-Case Scenario: $\mathcal{O}(n^2)$

The worst-case occurs when the binary tree is degenerate (e.g., completely skewed as a single-child chain, or a full tree where one branch is consistently minimal):
- The tree has height $h = n$.
- At the $i$-th recursive step, the search segment has size $n - i$.
- The total time is the sum of an arithmetic progression:
  $$T(n) = \sum_{i=1}^{n} \mathcal{O}(n - i) = \mathcal{O}\left(\frac{n(n - 1)}{2}\right) = \mathcal{O}(n^2)$$

### 2.3 Best-Case Scenario: $\mathcal{O}(n \log n)$

When the tree is perfectly balanced:
- At each step, the left and right subtrees each contain approximately $\frac{n - 1}{2}$ nodes.
- The linear search scans half the array: $\mathcal{O}(n)$ work at the current level.
- The recurrence relation is:
  $$T(n) = 2\,T\left(\frac{n}{2}\right) + \mathcal{O}(n)$$
- By Case 2 of the **Master Theorem** ($a = 2, b = 2, f(n) = \Theta(n)$ where $n^{\log_b a} = n^1$):
  $$T(n) = \Theta(n \log n)$$

### 2.4 Why Not Use a Hash Map?

Precomputing a hash table that maps each node value to its postorder index reduces the boundary search to $\mathcal{O}(1)$ average time, resulting in an overall time complexity of $\mathcal{O}(n)$.

**Educational Rationale:**
In the DevDictates curriculum, this implementation intentionally uses **linear search**:
1. It avoids external data structure overhead and maintains identical semantics across C, C++, Java, and Python.
2. It visually demonstrates the physical scanning of array partitions.
3. It clearly motivates the need for hashing in algorithm optimization.

---

## 3. Space Complexity Breakdown

### 3.1 Auxiliary Space: $\mathcal{O}(h)$

Auxiliary space measures only the extra temporary memory used by the algorithm during its execution, excluding the input and output.
- The algorithm uses index pointers (`pre_start`, `pre_end`, `post_start`, `post_end`) and does not copy sub-arrays.
- The memory consumed is strictly the **call stack frames**:
  - **Balanced Tree:** $h = \mathcal{O}(\log n)$, yielding $\mathcal{O}(\log n)$ stack depth.
  - **Skewed Tree:** $h = \mathcal{O}(n)$, yielding $\mathcal{O}(n)$ stack depth in the worst case.
- Thus, auxiliary space is strictly $\mathcal{O}(h)$.

### 3.2 Output Space: $\mathcal{O}(n)$

The constructed tree consists of $n$ dynamically allocated `Node` structures (in C, C++, Java, and Python). Each node stores its value and two pointers. This storage is required to hold the returned result and is classified as **output space**.
