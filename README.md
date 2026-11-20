# DevDictates — Binary Tree Construction from Preorder and Postorder Traversals

[![Tests](https://img.shields.io/badge/tests-passing-brightgreen.svg)](tests/run_tests.py)
[![Languages](https://img.shields.io/badge/languages-C%20%7C%20C%2B%2B%20%7C%20Java%20%7C%20Python-blue.svg)](#language-implementations)
[![License: DevDictates](https://img.shields.io/badge/license-DevDictates-purple.svg)](LICENSE)
[![GitHub Pages](https://img.shields.io/badge/demo-GitHub%20Pages-orange.svg)](index.html)

A self-contained, multi-language educational repository demonstrating how to reconstruct a **Binary Tree** from its **preorder** and **postorder** traversal sequences. Accompanies the [DevDictates](https://youtube.com) educational Data Structures and Algorithms series.

---

## 1. Problem Statement

Given two traversal sequences of a binary tree with distinct node values:
* **Preorder Traversal:** $\text{Root} \to \text{Left Subtree} \to \text{Right Subtree}$
* **Postorder Traversal:** $\text{Left Subtree} \to \text{Right Subtree} \to \text{Root}$

Construct the corresponding binary tree structure.

### Example

Given:
```text
Preorder:  A B D E C F G
Postorder: D E B F G C A
```

The reconstructed binary tree is:
```text
          A
        /   \
       B     C
      / \   / \
     D   E F   G
```

---

## 2. Interactive Web Visualizer

This repository includes a standalone static web application in [`index.html`](https://pragya1904.github.io/Binary-Tree-construction/) designed for direct deployment to **GitHub Pages**.

Features include:
* **Interactive Tree Canvas:** Dynamic SVG tree generation as nodes and parent-child edges are constructed.
* **Array Range & Pointer Tracker:** Visual highlighting of active preorder and postorder ranges `[pre_start .. pre_end]` and `[post_start .. post_end]`, tracking the root and the linear search scan.
* **Step-by-Step Playback:** Step forward, pause, and auto-play recursive partition steps.
* **Complexity Breakdown:** Visualized mathematical justification of the $\mathcal{O}(n^2)$ worst-case time complexity.
* **Ambiguity Demonstration:** Side-by-side comparison illustrating why single-child trees share identical traversals.

---

## 3. Algorithm Intuition & Pseudocode

### Subtree Boundary Partitioning

1. **Root Identification:**
   * The first token of preorder (`preorder[pre_start]`) is always the root of the current subtree.
   * The last token of postorder (`postorder[post_end]`) must match this root element.
2. **Base Cases:**
   * If `pre_start > pre_end`: return `NULL` (empty subtree).
   * If `pre_start == pre_end`: return a leaf node containing `preorder[pre_start]`.
3. **Locating Subtrees:**
   * The next token, `preorder[pre_start + 1]`, represents the root of the **left subtree**.
   * Perform a **linear search** for this value inside the postorder range `postorder[post_start .. post_end - 1]`.
   * Let `idx` be the position where the left root is located. The number of nodes in the left subtree is:
     $$\text{left\_size} = \text{idx} - \text{post\_start} + 1$$
4. **Recursive Step:**
   * **Left Subtree:**
     * Preorder: `[pre_start + 1, pre_start + left_size]`
     * Postorder: `[post_start, idx]`
   * **Right Subtree:**
     * Preorder: `[pre_start + left_size + 1, pre_end]`
     * Postorder: `[idx + 1, post_end - 1]`

### Base Recursive Pseudocode

```text
BUILD(preorder, postorder):
    if preorder is empty:
        return NULL

    root = new Node(preorder[0])

    if preorder has only one element:
        return root

    leftRoot = preorder[1]

    Find leftRoot in postorder (linear search)
    leftSize = its index + 1

    root.left = BUILD(
        preorder[1 : leftSize + 1],
        postorder[0 : leftSize]
    )

    root.right = BUILD(
        preorder[leftSize + 1 :],
        postorder[leftSize : n - 1]
    )

    return root
```

*Note: In all four language implementations, index boundaries are passed rather than allocating sub-arrays, avoiding memory copying overhead.*

---

## 4. Fundamental Limitation: The Single-Child Ambiguity

Can an arbitrary binary tree be uniquely reconstructed from preorder and postorder traversals alone? **No.**

Consider a parent `A` with a single child `B`:

```text
    Tree 1: Left Child        Tree 2: Right Child
          A                         A
         /                           \
        B                             B
```

Both trees produce the identical traversals:
* **Preorder:** `A B`
* **Postorder:** `B A`

Because preorder visits the root first and postorder visits the root last, neither traversal encodes whether a single child is on the left or the right.

### Resolutions:
1. **Base Approach (Standard Convention):** The algorithm deterministically assumes `preorder[pre_start + 1]` is the left child, attaching single children to the left branch.
2. **Full Binary Tree Constraint:** If every node is guaranteed to have either 0 or 2 children, ambiguity cannot occur, and reconstruction is mathematically unique.
3. **Explicit Null Markers:** Augmenting traversals with `#` tokens for missing children preserves branch orientation.

For a detailed analysis, see [docs/limitations.md](docs/limitations.md).

---

## 5. Complexity Analysis

| Approach | Worst-Case Time | Best-Case Time (Balanced) | Auxiliary Space | Output Space |
| :--- | :---: | :---: | :---: | :---: |
| **Base Recursive (Linear Search)** | $\mathcal{O}(n^2)$ | $\mathcal{O}(n \log n)$ | $\mathcal{O}(h)$ | $\mathcal{O}(n)$ |
| **Hash Map Optimized** | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |

### Justification:
* **Time Complexity $\mathcal{O}(n^2)$:** At each recursive step for a subtree of size $k$, linear search in postorder takes $\mathcal{O}(k)$ time. In a degenerate skewed tree, the total search time is:
  $$\sum_{i=1}^n \mathcal{O}(n - i) = \mathcal{O}\left(\frac{n(n-1)}{2}\right) = \mathcal{O}(n^2)$$
  In a balanced tree, the recurrence is $T(n) = 2T(n/2) + \mathcal{O}(n) = \mathcal{O}(n \log n)$.
* **Auxiliary Space $\mathcal{O}(h)$:** Only index boundaries are maintained; the memory overhead consists purely of call stack frames proportional to tree height $h \in [\lfloor \log_2 n \rfloor, n]$.
* **Output Space $\mathcal{O}(n)$:** Required for allocating the $n$ tree nodes.

For full mathematical proofs, see [docs/complexity_analysis.md](docs/complexity_analysis.md).

---

## 6. Language Implementations

Each implementation adheres to the identical algorithmic logic, input/output contract, and validation checks:

* [C (solution.c)](C/solution.c) — Pure C99/C11, explicit `malloc` and `free_tree` recursion.
* [C++ (solution.cpp)](C++/solution.cpp) — Modern C++17 with `std::unique_ptr<Node>` for RAII safety.
* [Java (Main.java)](Java/Main.java) — Clean Java 8+ implementation with error handling and static helpers.
* [Python (solution.py)](Python/solution.py) — Clean, idiomatic Python 3 reference implementation.

---

## 7. Compiling and Running

All solutions accept a line-oriented standard input format:
* **Line 1:** Space-separated preorder traversal tokens
* **Line 2:** Space-separated postorder traversal tokens

### C
```bash
clang -O2 -Wall -Wextra C/solution.c -o C/solution
./C/solution << 'EOF'
A B D E C F G
D E B F G C A
EOF
```

### C++
```bash
clang++ -O2 -std=c++17 -Wall -Wextra C++/solution.cpp -o C++/solution
./C++/solution << 'EOF'
A B D E C F G
D E B F G C A
EOF
```

### Java
```bash
javac Java/Main.java
java -cp Java Main << 'EOF'
A B D E C F G
D E B F G C A
EOF
```

### Python
```bash
python3 Python/solution.py << 'EOF'
A B D E C F G
D E B F G C A
EOF
```

### Canonical Output Format
```text
STATUS: SUCCESS
PREORDER: A B D # # E # # C F # # G # #
POSTORDER: # # D # # E B # # F # # G C A
```
*(The `#` null markers in the output provide a canonical serialized representation for automated structural verification).*

---

## 8. Unified Automated Testing

The repository provides a single test script that compiles and executes all four implementations against identical test cases:

```bash
python3 tests/run_tests.py
```

### Prerequisites
* Python 3.8+ (standard library only, no external pip packages required)
* C compiler (`clang` or `gcc`)
* C++ compiler (`clang++` or `g++`)
* Java JDK (`javac` and `java` runtime)

*If a compiler or runtime is not installed on the system, the test runner will clearly report the implementation as `SKIPPED` rather than failing or reporting false positives.*

### Test Categories
1. **Single-Node Tree:** Base case verification (`A` / `A`).
2. **Small Balanced Tree:** 3-node balanced partition (`A B C` / `B C A`).
3. **Full 7-Node Tree:** Complete binary tree (`A B D E C F G` / `D E B F G C A`).
4. **Single-Child Ambiguity:** Verification of left-branch attachment convention (`A B` / `B A`).
5. **Skewed Trees:** Linear chain degenerate topologies (`A B C` / `C B A`).
6. **Asymmetric Trees:** Deep left subtrees and deep right subtrees.
7. **Empty Tree:** Null input handling.
8. **Invalid Inputs:** Mismatched roots, length discrepancies, duplicate node values, and inconsistent sets.

---

## 9. Repository Structure

```text
binary-tree-construction/
│
├── index.html                                        # Interactive visualizer for GitHub Pages
├── README.md                                         # Project documentation
├── LICENSE                                           # DevDictates Educational License
├── .gitignore                                        # Build artifacts and cache ignores
│
├── C/
│   └── solution.c                                    # C99 implementation
│
├── C++/
│   └── solution.cpp                                  # C++17 implementation
│
├── Java/
│   └── Main.java                                     # Java implementation
│
├── Python/
│   └── solution.py                                   # Python 3 implementation
│
├── tests/
│   ├── test_cases/
│   │   └── cases.txt                                 # Input test fixtures
│   ├── expected/
│   │   └── expected_outputs.txt                      # Expected canonical outputs
│   └── run_tests.py                                  # Unified test runner
│
├── docs/
│   ├── algorithm.md                                  # Detailed algorithm walkthrough
│   ├── complexity_analysis.md                        # Time & space complexity proofs
│   └── limitations.md                                # Structural ambiguity & constraints
│
└── notes/
    └── DevDictates_Binary_Tree_Preorder_Postorder_Notes.pdf  # Educational PDF notes
```

---

## 10. Educational Resources & PDF Notes

Comprehensive notes for university students and technical interview preparation are included in the repository:
* [DevDictates_Binary_Tree_Preorder_Postorder_Notes.pdf](notes/DevDictates_Binary_Tree_Preorder_Postorder_Notes.pdf)

---

## 11. Credits

* **Series:** DevDictates — Data Structures & Algorithms
* **Author:** Pragya
