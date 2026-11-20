# DevDictates — Binary Tree Construction from Preorder and Postorder Traversals

[![Tests](https://img.shields.io/badge/tests-passing-brightgreen.svg)](tests/run_tests.py)
[![Languages](https://img.shields.io/badge/languages-C%20%7C%20C%2B%2B%20%7C%20Java%20%7C%20Python-blue.svg)](#language-implementations)
[![License: DevDictates](https://img.shields.io/badge/license-DevDictates-purple.svg)](LICENSE)
[![GitHub Pages](https://img.shields.io/badge/demo-GitHub%20Pages-orange.svg)](index.html)

A self-contained, multi-language educational repository demonstrating how to reconstruct a **Binary Tree** from its **preorder** and **postorder** traversal sequences. Accompanies the [DevDictates]([https://youtube.com](https://youtu.be/MhPQI_-URdc?si=4rcDoNg9j7TxBlWZ)) educational Data Structures and Algorithms series.

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
* **Complexity Breakdown:** Visualized mathematical justification of the $O(n^2)$ worst-case time complexity.
* **Ambiguity Demonstration:** Side-by-side comparison illustrating why single-child trees share identical traversals.

---

## 3. Algorithm Intuition

The reconstruction algorithm partitions traversal sequences recursively:

1. **Root Identification:** The first token of preorder (`preorder[pre_start]`) is the root of the current subtree and must match the last token of postorder (`postorder[post_end]`).
2. **Base Cases:** An empty range returns `NULL`; a single-token range returns a leaf node.
3. **Left Subtree Boundary:** The second token (`preorder[pre_start + 1]`) identifies the root of the left subtree. A linear search locates this value in postorder to compute the left subtree size:
   $$\text{left\_size} = \text{idx} - \text{post\_start} + 1$$
4. **Recursive Step:** Subtrees are partitioned recursively using index boundaries without copying subarrays.

For full pseudocode, mathematical intuition, and step-by-step recursion trace, see:
* [Detailed Algorithm Walkthrough](docs/algorithm.md)

---

## 4. Fundamental Limitation: Single-Child Ambiguity

Preorder and postorder traversals alone **cannot uniquely determine** an arbitrary binary tree when internal nodes have only one child.

For example, a parent `A` with a single child `B` produces identical traversals regardless of child orientation:
* **Left Child:** Preorder `A B`, Postorder `B A`
* **Right Child:** Preorder `A B`, Postorder `B A`

### Resolution:
* **Base Convention:** The algorithm deterministically attaches single children to the left branch.
* **Guaranteed Uniqueness:** Unique reconstruction requires either the **Full Binary Tree assumption** (every node has 0 or 2 children) or **explicit null markers** (`#`).

For complete proofs and structural diagrams, see:
* [Structural Limitations](docs/limitations.md)

---

## 5. Complexity Summary

| Approach | Worst-Case Time | Balanced-Tree Time | Auxiliary Space | Output Space |
| :--- | :---: | :---: | :---: | :---: |
| **Base Recursive (Linear Search)** | $O(n^2)$ | $O(n \log n)$ | $O(h)$ (recursion stack) | $O(n)$ |
| **Hash Map Optimized** | $O(n)$ | $O(n)$ | $O(n)$ (map + stack) | $O(n)$ |

* **Time Complexity $O(n^2)$:** In a highly unbalanced Full Binary Tree (e.g., comb-shaped tree), linear search over shrinking subarrays yields an arithmetic series $T(n) = T(n - 2) + O(n) = O(n^2)$.
* **Auxiliary Space $O(h)$:** Only index boundaries are maintained; memory overhead consists strictly of recursion call stack frames proportional to tree height $h \in [O(\log n), O(n)]$.
* **Output Space $O(n)$:** Dynamically allocated tree node storage, accounted for separately from auxiliary memory.

For complete mathematical derivations and Master Theorem proofs, see:
* [Detailed Time and Space Complexity Analysis](docs/time_complexity.md)

---

## 6. Language Implementations

Each implementation adheres to the identical algorithmic logic, input/output contract, and validation checks:

* [C (tree.h, tree.c, reconstruction.c, solution.c)](C/) — Modular C99/C11 with an opaque Tree ADT.
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
clang -O2 -Wall -Wextra C/tree.c C/reconstruction.c C/solution.c -o C/solution
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
│   ├── tree.h                                        # Public Tree ADT interface
│   ├── tree.c                                        # Node representation & memory management
│   ├── reconstruction.h                              # Reconstruction interface
│   ├── reconstruction.c                              # Subtree partitioning algorithm
│   ├── solution.c                                    # CLI entry point & I/O driver
│   └── README.md                                     # Module overview & compilation
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
│   ├── time_complexity.md                            # Comprehensive time & space complexity analysis
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
