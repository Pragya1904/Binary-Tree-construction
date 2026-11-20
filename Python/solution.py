#!/usr/bin/env python3
"""
DevDictates - Binary Tree Construction from Preorder and Postorder Traversals.
Language: Python 3
Approach: Base Recursive Subtree Partitioning (Linear Search, O(n^2) worst case, O(h) auxiliary space)
Author: Pragya
"""

import sys


class Node:
    """Binary Tree Node."""
    def __init__(self, val: str):
        self.val = val
        self.left = None
        self.right = None


def to_preorder_with_nulls(root: Node, out: list):
    """Serialize tree to preorder traversal with '#' for null pointers."""
    if root is None:
        out.append("#")
        return
    out.append(root.val)
    to_preorder_with_nulls(root.left, out)
    to_preorder_with_nulls(root.right, out)


def to_postorder_with_nulls(root: Node, out: list):
    """Serialize tree to postorder traversal with '#' for null pointers."""
    if root is None:
        out.append("#")
        return
    to_postorder_with_nulls(root.left, out)
    to_postorder_with_nulls(root.right, out)
    out.append(root.val)


def build_tree_recursive(preorder, pre_start, pre_end, postorder, post_start, post_end):
    """
    Base recursive construction of a binary tree from preorder and postorder traversals.
    
    Algorithm:
    1. The first element in preorder[pre_start] is the root of the current subtree.
    2. The last element in postorder[post_end] must match the root.
    3. If pre_start == pre_end, the subtree is a single leaf node.
    4. Otherwise, preorder[pre_start + 1] is the root of the left subtree.
    5. Find this left root in postorder[post_start .. post_end - 1] via linear search.
    6. Subtree partition size: left_size = idx - post_start + 1.
    7. Recurse for left subtree and right subtree using index boundaries.
    """
    if pre_start > pre_end:
        return None

    root_val = preorder[pre_start]
    if postorder[post_end] != root_val:
        raise ValueError(f"Root mismatch: preorder root '{root_val}' != postorder root '{postorder[post_end]}'")

    root = Node(root_val)
    if pre_start == pre_end:
        return root

    # Next element in preorder is the root of the left subtree
    left_root_val = preorder[pre_start + 1]

    # Linear search in postorder range [post_start .. post_end - 1]
    idx = -1
    for i in range(post_start, post_end):
        if postorder[i] == left_root_val:
            idx = i
            break

    if idx == -1:
        raise ValueError(f"Left child '{left_root_val}' not found in postorder segment [{post_start}..{post_end - 1}]")

    left_size = idx - post_start + 1

    # Recurse on left subtree
    root.left = build_tree_recursive(
        preorder, pre_start + 1, pre_start + left_size,
        postorder, post_start, idx
    )
    # Recurse on right subtree
    root.right = build_tree_recursive(
        preorder, pre_start + left_size + 1, pre_end,
        postorder, idx + 1, post_end - 1
    )

    return root


def construct_tree(preorder, postorder):
    """Validate inputs and construct the binary tree."""
    if len(preorder) != len(postorder):
        raise ValueError(f"Length mismatch: preorder ({len(preorder)}) != postorder ({len(postorder)})")

    if len(preorder) == 0:
        return None

    # Distinct values check
    if len(set(preorder)) != len(preorder):
        raise ValueError("Duplicate node values detected in preorder traversal")
    if len(set(postorder)) != len(postorder):
        raise ValueError("Duplicate node values detected in postorder traversal")
    if set(preorder) != set(postorder):
        raise ValueError("Elements in preorder and postorder traversals do not match")

    return build_tree_recursive(
        preorder, 0, len(preorder) - 1,
        postorder, 0, len(postorder) - 1
    )


def parse_input():
    """
    Read line-oriented input from standard input.
    Supports:
      2-line format:
        Line 1: Preorder tokens
        Line 2: Postorder tokens
      3-line format (with optional mode indicator on Line 1):
        Line 1: mode (e.g. 'base' or 'full')
        Line 2: Preorder tokens
        Line 3: Postorder tokens
    """
    lines = sys.stdin.read().splitlines()
    # Filter empty lines at the very end
    non_empty = [line.strip() for line in lines if line.strip()]
    if not non_empty:
        return [], []

    # Check if first line is a mode name
    if non_empty[0].lower() in ("base", "full", "null-marked", "default"):
        if len(non_empty) < 3:
            # Might be empty preorder or postorder
            pre_str = non_empty[1] if len(non_empty) > 1 else ""
            post_str = non_empty[2] if len(non_empty) > 2 else ""
        else:
            pre_str = non_empty[1]
            post_str = non_empty[2]
    else:
        pre_str = non_empty[0] if len(non_empty) > 0 else ""
        post_str = non_empty[1] if len(non_empty) > 1 else ""

    return pre_str.split(), post_str.split()


def main():
    try:
        preorder, postorder = parse_input()
        root = construct_tree(preorder, postorder)

        pre_out = []
        post_out = []
        to_preorder_with_nulls(root, pre_out)
        to_postorder_with_nulls(root, post_out)

        print("STATUS: SUCCESS")
        print(f"PREORDER: {' '.join(pre_out)}")
        print(f"POSTORDER: {' '.join(post_out)}")
    except Exception as e:
        print("STATUS: ERROR")
        print(f"ERROR: {str(e)}")


if __name__ == "__main__":
    main()
