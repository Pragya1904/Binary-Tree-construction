# Recursive Tree Construction Algorithm

This document explains the recursive algorithm for reconstructing a binary tree from its preorder and postorder traversals, as taught in the [DevDictates](https://youtu.be/MhPQI_-URdc?si=79iNqYZenbszplLm) educational series.

<iframe width="560" height="315" src="https://www.youtube.com/embed/MhPQI_-URdc?si=79iNqYZenbszplLm" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>
---

## 1. Problem Formulation

Given two sequences representing tree traversals:
* **Preorder Traversal:** $\text{Root} \to \text{Left Subtree} \to \text{Right Subtree}$
* **Postorder Traversal:** $\text{Left Subtree} \to \text{Right Subtree} \to \text{Root}$

Assume all node values are distinct. Our goal is to reconstruct the original binary tree structure.

---

## 2. Fundamental Intuition

### 2.1 Root Identification
* In preorder, the very first element of a segment is always the root of that subtree:
  $$\text{root} = \text{preorder}[\text{pre\_start}]$$
* In postorder, the very last element of the corresponding segment must match this root:
  $$\text{postorder}[\text{post\_end}] = \text{root}$$
* Comparing these two tokens validates traversal consistency at each recursive step.

### 2.2 Base Cases
* If `pre_start > pre_end`: The range is empty; return `NULL`.
* If `pre_start == pre_end`: Exactly one element remains; construct and return a leaf node containing that value.

### 2.3 Left Subtree Root Identification
* When a subtree contains two or more nodes, the element immediately following the root in preorder, `preorder[pre_start + 1]`, identifies the root of the **left subtree**.
* In postorder traversal, all nodes in the left subtree precede all nodes in the right subtree, with the left subtree root appearing at the end of the left subtree block.

### 2.4 Linear Search in the Postorder Range
* We locate the left subtree root value within the active postorder range `postorder[post_start .. post_end - 1]` using a linear search.
* Suppose the left root is found at index `idx`.

### 2.5 Calculating Left Subtree Size
* The number of nodes in the left subtree is determined by:
  $$\text{left\_size} = \text{idx} - \text{post\_start} + 1$$

### 2.6 Recursive Index Partitioning
Using index boundaries rather than allocating sub-arrays, the ranges for the child subtrees are:

* **Left Subtree:**
  * Preorder: `[pre_start + 1, pre_start + left_size]`
  * Postorder: `[post_start, idx]`
* **Right Subtree:**
  * Preorder: `[pre_start + left_size + 1, pre_end]`
  * Postorder: `[idx + 1, post_end - 1]`

---

## 3. Base Recursive Pseudocode

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

*Note: In all four language implementations in this repository, explicit index boundaries (`pre_start`, `pre_end`, `post_start`, `post_end`) are passed to recursive calls rather than copying slices.*

---

## 4. Comprehensive Worked Example

Consider the traversals:
* **Preorder:** `A B D E C F G`
* **Postorder:** `D E B F G C A`

Target binary tree:

```text
          A
        /   \
       B     C
      / \   / \
     D   E F   G
```

### Step-by-Step Recursion Trace

| Level | Subtree Range | Preorder Segment | Postorder Segment | Root | Left Child Value | Found Index in Postorder | `left_size` | Partition |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | Full Tree | `[A, B, D, E, C, F, G]` | `[D, E, B, F, G, C, A]` | `A` | `B` | `idx = 2` | $2 - 0 + 1 = 3$ | Left: `B D E` / `D E B`<br>Right: `C F G` / `F G C` |
| **1L** | Left Subtree | `[B, D, E]` | `[D, E, B]` | `B` | `D` | `idx = 0` | $0 - 0 + 1 = 1$ | Left: `D` / `D`<br>Right: `E` / `E` |
| **2LL**| Left-Left Leaf | `[D]` | `[D]` | `D` | — | — | — | Base case: leaf node |
| **2LR**| Left-Right Leaf| `[E]` | `[E]` | `E` | — | — | — | Base case: leaf node |
| **1R** | Right Subtree | `[C, F, G]` | `[F, G, C]` | `C` | `F` | `idx = 3` | $3 - 3 + 1 = 1$ | Left: `F` / `F`<br>Right: `G` / `G` |
| **2RL**| Right-Left Leaf| `[F]` | `[F]` | `F` | — | — | — | Base case: leaf node |
| **2RR**| Right-Right Leaf| `[G]` | `[G]` | `G` | — | — | — | Base case: leaf node |

---

## 5. Single-Child Convention and Limitations

When an internal node has only one child:
* Preorder: `A B`
* Postorder: `B A`

Because `B` immediately follows `A` in preorder, the algorithm treats `B` as the root of the left subtree (`left_size = 1`), leaving the right subtree empty. Thus, the algorithm deterministically attaches single children as **left children**.

* For a full explanation of why ordinary traversals cannot distinguish single left children from single right children, see [Structural Limitations](limitations.md).
* For detailed time and memory complexity derivations, see [Time and Space Complexity Analysis](time_complexity.md).
