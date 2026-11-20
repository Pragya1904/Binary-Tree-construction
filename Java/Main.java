/**
 * DevDictates - Binary Tree Construction from Preorder and Postorder Traversals.
 * Language: Java (Java 8+)
 * Approach: Base Recursive Subtree Partitioning (Linear Search, O(n^2) worst case, O(h) auxiliary space)
 * Author: Pragya
 */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.*;

public class Main {

    static class Node {
        String val;
        Node left;
        Node right;

        Node(String val) {
            this.val = val;
            this.left = null;
            this.right = null;
        }
    }

    public static void serializePreorderWithNulls(Node root, List<String> out) {
        if (root == null) {
            out.add("#");
            return;
        }
        out.add(root.val);
        serializePreorderWithNulls(root.left, out);
        serializePreorderWithNulls(root.right, out);
    }

    public static void serializePostorderWithNulls(Node root, List<String> out) {
        if (root == null) {
            out.add("#");
            return;
        }
        serializePostorderWithNulls(root.left, out);
        serializePostorderWithNulls(root.right, out);
        out.add(root.val);
    }

    public static Node buildTreeRecursive(List<String> preorder, int preStart, int preEnd,
                                          List<String> postorder, int postStart, int postEnd) {
        if (preStart > preEnd) {
            return null;
        }

        String rootVal = preorder.get(preStart);
        if (!postorder.get(postEnd).equals(rootVal)) {
            throw new IllegalArgumentException("Root mismatch: preorder root '" + rootVal +
                    "' != postorder root '" + postorder.get(postEnd) + "'");
        }

        Node root = new Node(rootVal);
        if (preStart == preEnd) {
            return root;
        }

        String leftRootVal = preorder.get(preStart + 1);

        // Linear search in postorder[postStart .. postEnd - 1]
        int idx = -1;
        for (int i = postStart; i < postEnd; i++) {
            if (postorder.get(i).equals(leftRootVal)) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            throw new IllegalArgumentException("Left child '" + leftRootVal + "' not found in postorder segment");
        }

        int leftSize = idx - postStart + 1;

        root.left = buildTreeRecursive(preorder, preStart + 1, preStart + leftSize,
                postorder, postStart, idx);
        root.right = buildTreeRecursive(preorder, preStart + leftSize + 1, preEnd,
                postorder, idx + 1, postEnd - 1);

        return root;
    }

    public static void main(String[] args) {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            List<String> lines = new ArrayList<>();
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }

            String preStr = "";
            String postStr = "";

            if (!lines.isEmpty()) {
                String firstTrim = lines.get(0).trim();
                String[] firstToks = firstTrim.isEmpty() ? new String[0] : firstTrim.split("\\s+");

                if (firstToks.length > 0 &&
                        (firstToks[0].equalsIgnoreCase("base") ||
                         firstToks[0].equalsIgnoreCase("full") ||
                         firstToks[0].equalsIgnoreCase("null-marked") ||
                         firstToks[0].equalsIgnoreCase("default"))) {
                    preStr = lines.size() > 1 ? lines.get(1) : "";
                    postStr = lines.size() > 2 ? lines.get(2) : "";
                } else {
                    preStr = lines.get(0);
                    postStr = lines.size() > 1 ? lines.get(1) : "";
                }
            }

            List<String> preorder = new ArrayList<>();
            for (String s : preStr.trim().split("\\s+")) {
                if (!s.isEmpty()) preorder.add(s);
            }

            List<String> postorder = new ArrayList<>();
            for (String s : postStr.trim().split("\\s+")) {
                if (!s.isEmpty()) postorder.add(s);
            }

            if (preorder.size() != postorder.size()) {
                throw new IllegalArgumentException("Length mismatch: preorder (" + preorder.size() +
                        ") != postorder (" + postorder.size() + ")");
            }

            if (preorder.isEmpty()) {
                System.out.println("STATUS: SUCCESS");
                System.out.println("PREORDER: #");
                System.out.println("POSTORDER: #");
                return;
            }

            Set<String> preSet = new HashSet<>(preorder);
            if (preSet.size() != preorder.size()) {
                throw new IllegalArgumentException("Duplicate node values detected in preorder traversal");
            }

            Set<String> postSet = new HashSet<>(postorder);
            if (postSet.size() != postorder.size()) {
                throw new IllegalArgumentException("Duplicate node values detected in postorder traversal");
            }

            if (!preSet.equals(postSet)) {
                throw new IllegalArgumentException("Elements in preorder and postorder traversals do not match");
            }

            Node root = buildTreeRecursive(preorder, 0, preorder.size() - 1,
                    postorder, 0, postorder.size() - 1);

            List<String> outPre = new ArrayList<>();
            List<String> outPost = new ArrayList<>();
            serializePreorderWithNulls(root, outPre);
            serializePostorderWithNulls(root, outPost);

            System.out.println("STATUS: SUCCESS");
            System.out.println("PREORDER: " + String.join(" ", outPre));
            System.out.println("POSTORDER: " + String.join(" ", outPost));

        } catch (Exception e) {
            System.out.println("STATUS: ERROR");
            System.out.println("ERROR: " + e.getMessage());
        }
    }
}
