⏱️ Time and Space Complexity Analysis

<div align="center">

Binary Tree Construction Using Preorder and Postorder

DevDictates · DSA Field Notes

</div>

⸻

1. Complexity Analysis: Overview

We consider two approaches to reconstructing a binary tree:

1. Recursive reconstruction using the Full Binary Tree assumption and linear search.
2. Reconstruction using explicit null markers.

Our analysis distinguishes between:

* Time complexity.
* Auxiliary space complexity.
* Recursion stack space.
* Output space.
* Input storage.

Let:

* n = Number of actual nodes in the binary tree.
* h = Height of the binary tree.

⸻

2. Approach 1: Full Binary Tree + Linear Search

Algorithm Recap

For every recursive call:

1. Identify the root using the first preorder element.
2. Identify the left subtree’s root using the next preorder element.
3. Perform a linear search for that element in the current postorder range.
4. Calculate the left subtree’s size.
5. Recursively construct the left and right subtrees.

The important operation here is the linear search.

Time Complexity

Let’s break down the operations.

Operation	Time Complexity
Identify the root	O(1)
Identify the left subtree’s root	O(1)
Search for the left subtree’s root in postorder	O(s)
Calculate the left subtree’s size	O(1)
Construct the left subtree	Recursive
Construct the right subtree	Recursive

Here, s represents the number of elements in the current postorder search range.

Per-Node Processing

Every node is constructed exactly once.

Therefore, the total node creation and constant-time processing cost is:

O(n)

However, identifying the left subtree boundary requires a linear search.

The search may inspect a substantial portion of the current subtree.

Worst-Case Time Complexity

Consider a Full Binary Tree where each recursive level has:

* One leaf child.
* One larger subtree.

This creates an increasingly smaller search range, but the search can still inspect nearly all nodes in that range.

The total searching work can be represented as:

T(n) = T(n - 2) + O(n)

Expanding:

T(n) = O(n) + O(n - 2) + O(n - 4) + ...

This is an arithmetic series.

Therefore:

T(n) = O(n²)

Worst-case time complexity: O(n²).

Why Not O(n)?

Although each node is created only once, we repeatedly search for subtree boundaries.

The same portions of the postorder traversal may be examined across different recursive calls.

This repeated searching is what increases the worst-case time complexity.

⸻

3. Auxiliary Space Complexity: Approach 1

We need to distinguish between three types of memory.

A. Recursive Call Stack

Every recursive call remains active until its child subtrees have been constructed.

The maximum number of simultaneously active recursive calls depends on the height of the tree.

Therefore:

O(h)

For a balanced tree:

h = O(log n)

For a highly unbalanced tree:

h = O(n)

Hence:

Tree Shape	Recursion Stack
Balanced tree	O(log n)
Worst-case height	O(n)

B. Input Storage

The preorder and postorder arrays are already supplied as input.

We only read from them.

We do not create copies or modify them.

Therefore, their existing storage is not counted as additional auxiliary space.

C. Output Storage

We construct a new binary tree containing n nodes.

The tree itself requires:

O(n)

However, output storage is excluded when reporting auxiliary space complexity.

Final Auxiliary Space

The algorithm uses the recursive call stack and no additional lookup structure.

Therefore:

Auxiliary Space = O(h)

Worst case:

O(n)

⸻

4. Approach 2: Explicit Null Markers

Algorithm Recap

Instead of restricting the tree to be full, we explicitly represent missing children using a null marker.

For example:

#

represents a missing child.

We use augmented preorder and postorder traversals.

The reconstruction algorithm:

1. Reads the next preorder token.
2. If the token is #, returns NULL.
3. Otherwise, creates a node.
4. Recursively constructs its left child.
5. Recursively constructs its right child.
6. Returns the constructed node.
7. Generates augmented postorder and compares it with the supplied sequence.

This preserves the original tree structure, including nodes with only one child.

Time Complexity

Let n be the number of actual nodes.

A binary tree with n actual nodes contains exactly:

n + 1

null child positions.

Therefore, the augmented traversal contains:

n + (n + 1) = 2n + 1

tokens.

Every token is processed a constant number of times.

The operations include:

Operation	Time Complexity
Read each preorder token	O(1) per token
Construct each actual node	O(1) per node
Process each null marker	O(1) per marker
Generate augmented postorder	O(n)
Compare generated and supplied postorder	O(n)

Total:

O(2n + 1) = O(n)

Time complexity: O(n).

Unlike the first approach, we do not repeatedly search for subtree boundaries.

The structural information is already encoded in the null markers.

⸻

5. Auxiliary Space Complexity: Approach 2

A. Recursive Call Stack

The recursive reconstruction depth depends on the tree height.

Therefore:

O(h)

For a balanced tree:

O(log n)

For a skewed tree:

O(n)

B. Input Storage

The augmented preorder and postorder sequences are supplied as input.

Their existing storage is excluded from auxiliary space.

C. Output Storage

The constructed tree requires:

O(n)

This is output space and is excluded from auxiliary space complexity.

D. Additional Working Space

The implementation should use an index into the preorder sequence rather than creating new arrays or copied subarrays.

The generated postorder sequence, if stored separately for validation, requires O(n) additional working space.

Therefore, there are two ways to report the implementation’s auxiliary space:

Reconstruction alone:

O(h)

Reconstruction plus stored postorder validation sequence:

O(n + h) = O(n)

This distinction is important.

If postorder validation is performed using a streaming comparison rather than storing a second sequence, the additional storage can be reduced.

⸻

6. Final Complexity Comparison

Metric	Full Binary Tree + Linear Search	Explicit Null Markers
Node creation	O(n)	O(n)
Boundary identification	Repeated linear search	Encoded in markers
Total time	O(n²) worst case	O(n)
Recursion stack	O(h)	O(h)
Additional lookup storage	O(1)	O(1)
Output tree	O(n)	O(n)
Unique reconstruction	Yes, under assumptions	Yes, with valid markers

Important implementation detail

The O(h) auxiliary-space result for null-marker reconstruction applies to the recursive reconstruction itself, assuming the input tokens are supplied and consumed without copying.

If the implementation additionally materialises a separate postorder output sequence for validation, that sequence uses O(n) working space.

Always state which implementation details are included in the space analysis.

⸻

7. Final Takeaways

1. The linear-search recursive approach processes every node once but may repeatedly search the traversal array.
2. This gives O(n²) worst-case time complexity.
3. Its auxiliary space is O(h), primarily due to recursion.
4. Explicit null markers preserve missing-child information.
5. Null-marker reconstruction takes O(n) time because each token is processed a constant number of times.
6. The constructed tree requires O(n) output space.
7. Input storage and output storage must be distinguished from auxiliary working memory.

⸻

<div align="center">

DevDictates

Understand the cost, not just the code.

— Pragya ♡

</div>