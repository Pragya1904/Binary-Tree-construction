# Recursive Tree Construction Algorithm

This document details the recursive algorithm for reconstructing a binary tree from its **preorder** and **postorder** traversals, as presented in the [DevDictates](https://youtube.com) educational series.

---

## 1. Problem Formulation

Given two sequences representing tree traversals:
- **Preorder Traversal:** $\text{Root} \to \text{Left Subtree} \to \text{Right Subtree}$
- **Postorder Traversal:** $\text{Left Subtree} \to \text{Right Subtree} \to \text{Root}$

Assume all node values are distinct. Our goal is to reconstruct the original binary tree structure.

---

## 2. Fundamental Intuition

1. **Root Identification:**
   - In **preorder**, the very first element is always the root of the current subtree:
     $$\text{root} = \text{preorder}[\text{pre\_start}]$$
   - In **postorder**, the very last element is always the root of that same subtree:
     $$\text{postorder}[\text{post\_end}] = \text{root}$$
   - Comparing these two provides immediate validation of the input.

2. **Base Cases:**
   - If the range is empty ($\text{pre\_start} > \text{pre\_end}$), return `NULL`.
   - If the range has exactly one element ($\text{pre\_start} == \text{pre\_end}$), return a single leaf node.

3. **Subtree Boundary Identification:**
   - When a subtree has two or more nodes, the element immediately following the root in preorder, $\text{preorder}[\text{pre\_start} + 1]$, must be the root of the **left subtree**.
   - In postorder traversal, all nodes belonging to the left subtree appear together before the right subtree, and the root of the left subtree appears at the **end** of this left subtree block.
   - We perform a **linear search** for this left root value inside the current postorder segment: $\text{postorder}[\text{post\_start} \dots \text{post\_end} - 1]$.
   - Suppose the left root is located at index $\text{idx}$. Then:
     $$\text{left\_size} = \text{idx} - \text{post\_start} + 1$$
   - The left subtree contains $\text{left\_size}$ nodes.

4. **Recursive Partitioning:**
   - **Left Subtree:**
     - Preorder range: $[\text{pre\_start} + 1, \quad \text{pre\_start} + \text{left\_size}]$
     - Postorder range: $[\text{post\_start}, \quad \text{idx}]$
   - **Right Subtree:**
     - Preorder range: $[\text{pre\_start} + \text{left\_size} + 1, \quad \text{pre\_end}]$
     - Postorder range: $[\text{idx} + 1, \quad \text{post\_end} - 1]$

---

## 3. Algorithm Pseudocode

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

*Note: In production and competitive programming implementations, index pointers are passed rather than allocating sub-arrays, avoiding memory copying overhead.*

---

## 4. Comprehensive Worked Example

Consider the following traversals:
- **Preorder:** `A  B  D  E  C  F  G`
- **Postorder:** `D  E  B  F  G  C  A`

```text
               A
             /   \
            B     C
           / \   / \
          D   E F   G
```

### Trace Step-by-Step

| Level | Subtree Range | Preorder Segment | Postorder Segment | Root | Left Child Value | Found Index in Postorder | `left_size` | Partition |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | Full Tree | `[A, B, D, E, C, F, G]` | `[D, E, B, F, G, C, A]` | `A` | `B` | `idx = 2` | $2 - 0 + 1 = 3$ | Left: `B D E` / `D E B`<br>Right: `C F G` / `F G C` |
| **1L** | Left Subtree | `[B, D, E]` | `[D, E, B]` | `B` | `D` | `idx = 0` | $0 - 0 + 1 = 1$ | Left: `D` / `D`<br>Right: `E` / `E` |
| **2LL**| Left-Left Leaf | `[D]` | `[D]` | `D` | — | — | — | Base case: leaf node |
| **2LR**| Left-Right Leaf| `[E]` | `[E]` | `E` | — | — | — | Base case: leaf node |
| **1R** | Right Subtree| `[C, F, G]` | `[F, G, C]` | `C` | `F` | `idx = 3` | $3 - 3 + 1 = 1$ | Left: `F` / `F`<br>Right: `G` / `G` |
| **2RL**| Right-Left Leaf| `[F]` | `[F]` | `F` | — | — | — | Base case: leaf node |
| **2RR**| Right-Right Leaf| `[G]` | `[G]` | `G` | — | — | — | Base case: leaf node |

---

## 5. Single-Child Convention

When an internal node has only one child:
- Preorder: `A  B`
- Postorder: `B  A`

The element `B` follows `A` in preorder, so it is treated as the root of the left subtree:
$$\text{idx} = 0 \implies \text{left\_size} = 1$$
The left subtree receives `[B]`, and the right subtree receives empty ranges. Thus, `B` is conventionally attached as the **left child**. For a detailed discussion of this ambiguity, see [limitations.md](limitations.md).
