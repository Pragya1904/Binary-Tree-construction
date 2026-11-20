#!/usr/bin/env python3
"""
Binary tree construction from preorder and postorder traversals.
DevDictates
"""
import sys


class Node:
    def __init__(self, val: str):
        self.val = val
        self.left = None
        self.right = None


def serialize_preorder(root: Node, out: list):
    """Serialize tree to preorder traversal with '#' for null pointers."""
    if root is None:
        out.append("#")
        return
    out.append(root.val)
    serialize_preorder(root.left, out)
    serialize_preorder(root.right, out)


def serialize_postorder(root: Node, out: list):
    """Serialize tree to postorder traversal with '#' for null pointers."""
    if root is None:
        out.append("#")
        return
    serialize_postorder(root.left, out)
    serialize_postorder(root.right, out)
    out.append(root.val)


def build_tree(preorder, pre_start, pre_end, postorder, post_start, post_end):
    if pre_start > pre_end:
        return None

    root_val = preorder[pre_start]
    if postorder[post_end] != root_val:
        raise ValueError(f"Root mismatch: preorder root '{root_val}' != postorder root '{postorder[post_end]}'")

    root = Node(root_val)
    if pre_start == pre_end:
        return root

    left_root = preorder[pre_start + 1]

    # Linear search for left subtree root in postorder segment
    idx = -1
    for i in range(post_start, post_end):
        if postorder[i] == left_root:
            idx = i
            break

    if idx == -1:
        raise ValueError(f"Left child '{left_root}' not found in postorder segment")

    left_size = idx - post_start + 1

    root.left = build_tree(
        preorder, pre_start + 1, pre_start + left_size,
        postorder, post_start, idx
    )
    root.right = build_tree(
        preorder, pre_start + left_size + 1, pre_end,
        postorder, idx + 1, post_end - 1
    )

    return root


def validate_traversals(preorder, postorder):
    if len(preorder) != len(postorder):
        raise ValueError(f"Length mismatch: preorder ({len(preorder)}) != postorder ({len(postorder)})")

    if len(preorder) == 0:
        return

    if len(set(preorder)) != len(preorder):
        raise ValueError("Duplicate node values detected in preorder traversal")

    if len(set(postorder)) != len(postorder):
        raise ValueError("Duplicate node values detected in postorder traversal")

    if set(preorder) != set(postorder):
        raise ValueError("Elements in preorder and postorder traversals do not match")


def reconstruct_tree(preorder, postorder):
    validate_traversals(preorder, postorder)
    if not preorder:
        return None
    return build_tree(preorder, 0, len(preorder) - 1, postorder, 0, len(postorder) - 1)


def parse_input():
    lines = sys.stdin.read().splitlines()
    non_empty = [line.strip() for line in lines if line.strip()]
    if not non_empty:
        return [], []

    if non_empty[0].lower() in ("base", "full", "null-marked", "default"):
        pre_str = non_empty[1] if len(non_empty) > 1 else ""
        post_str = non_empty[2] if len(non_empty) > 2 else ""
    else:
        pre_str = non_empty[0] if len(non_empty) > 0 else ""
        post_str = non_empty[1] if len(non_empty) > 1 else ""

    return pre_str.split(), post_str.split()


def main():
    try:
        preorder, postorder = parse_input()
        root = reconstruct_tree(preorder, postorder)

        pre_out = []
        post_out = []
        serialize_preorder(root, pre_out)
        serialize_postorder(root, post_out)

        print("STATUS: SUCCESS")
        print(f"PREORDER: {' '.join(pre_out)}")
        print(f"POSTORDER: {' '.join(post_out)}")
    except ValueError as e:
        print("STATUS: ERROR")
        print(f"ERROR: {e}")


if __name__ == "__main__":
    main()
