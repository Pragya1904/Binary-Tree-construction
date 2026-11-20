/**
 * DevDictates - Binary Tree Construction from Preorder and Postorder Traversals.
 * Language: C++ (C++17)
 * Approach: Base Recursive Subtree Partitioning (Linear Search, O(n^2) worst case, O(h) auxiliary space)
 * Memory: std::unique_ptr for safe RAII ownership
 * Author: Pragya
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <unordered_set>
#include <algorithm>

struct Node {
    std::string val;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    explicit Node(std::string v) : val(std::move(v)), left(nullptr), right(nullptr) {}
};

void serialize_preorder_with_nulls(const Node *root, std::vector<std::string> &out) {
    if (!root) {
        out.push_back("#");
        return;
    }
    out.push_back(root->val);
    serialize_preorder_with_nulls(root->left.get(), out);
    serialize_preorder_with_nulls(root->right.get(), out);
}

void serialize_postorder_with_nulls(const Node *root, std::vector<std::string> &out) {
    if (!root) {
        out.push_back("#");
        return;
    }
    serialize_postorder_with_nulls(root->left.get(), out);
    serialize_postorder_with_nulls(root->right.get(), out);
    out.push_back(root->val);
}

std::unique_ptr<Node> build_tree_recursive(const std::vector<std::string> &preorder,
                                           int pre_start, int pre_end,
                                           const std::vector<std::string> &postorder,
                                           int post_start, int post_end) {
    if (pre_start > pre_end) {
        return nullptr;
    }

    const std::string &root_val = preorder[pre_start];
    if (postorder[post_end] != root_val) {
        throw std::runtime_error("Root mismatch: preorder root '" + root_val +
                                 "' != postorder root '" + postorder[post_end] + "'");
    }

    auto root = std::make_unique<Node>(root_val);
    if (pre_start == pre_end) {
        return root;
    }

    const std::string &left_root_val = preorder[pre_start + 1];

    // Linear search in postorder[post_start .. post_end - 1]
    int idx = -1;
    for (int i = post_start; i < post_end; ++i) {
        if (postorder[i] == left_root_val) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        throw std::runtime_error("Left child '" + left_root_val + "' not found in postorder segment");
    }

    int left_size = idx - post_start + 1;

    root->left = build_tree_recursive(preorder, pre_start + 1, pre_start + left_size,
                                      postorder, post_start, idx);
    root->right = build_tree_recursive(preorder, pre_start + left_size + 1, pre_end,
                                       postorder, idx + 1, post_end - 1);

    return root;
}

std::vector<std::string> tokenize(const std::string &line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

int main() {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(std::cin, line)) {
        lines.push_back(line);
    }

    // Determine preorder and postorder strings
    std::string pre_str;
    std::string post_str;

    if (!lines.empty()) {
        std::string first_trim = trim(lines[0]);
        std::vector<std::string> first_toks = tokenize(first_trim);

        if (!first_toks.empty() &&
            (first_toks[0] == "base" || first_toks[0] == "full" ||
             first_toks[0] == "null-marked" || first_toks[0] == "default")) {
            pre_str = (lines.size() > 1) ? lines[1] : "";
            post_str = (lines.size() > 2) ? lines[2] : "";
        } else {
            pre_str = lines[0];
            post_str = (lines.size() > 1) ? lines[1] : "";
        }
    }

    std::vector<std::string> preorder = tokenize(pre_str);
    std::vector<std::string> postorder = tokenize(post_str);

    try {
        if (preorder.size() != postorder.size()) {
            throw std::runtime_error("Length mismatch: preorder (" +
                                     std::to_string(preorder.size()) + ") != postorder (" +
                                     std::to_string(postorder.size()) + ")");
        }

        if (preorder.empty()) {
            std::cout << "STATUS: SUCCESS\n";
            std::cout << "PREORDER: #\n";
            std::cout << "POSTORDER: #\n";
            return 0;
        }

        // Distinct check
        std::unordered_set<std::string> pre_set(preorder.begin(), preorder.end());
        if (pre_set.size() != preorder.size()) {
            throw std::runtime_error("Duplicate node values detected in preorder traversal");
        }
        std::unordered_set<std::string> post_set(postorder.begin(), postorder.end());
        if (post_set.size() != postorder.size()) {
            throw std::runtime_error("Duplicate node values detected in postorder traversal");
        }
        if (pre_set != post_set) {
            throw std::runtime_error("Elements in preorder and postorder traversals do not match");
        }

        auto root = build_tree_recursive(preorder, 0, static_cast<int>(preorder.size()) - 1,
                                         postorder, 0, static_cast<int>(postorder.size()) - 1);

        std::vector<std::string> out_pre;
        std::vector<std::string> out_post;
        serialize_preorder_with_nulls(root.get(), out_pre);
        serialize_postorder_with_nulls(root.get(), out_post);

        std::cout << "STATUS: SUCCESS\n";
        std::cout << "PREORDER:";
        for (const auto &tok : out_pre) {
            std::cout << " " << tok;
        }
        std::cout << "\n";

        std::cout << "POSTORDER:";
        for (const auto &tok : out_post) {
            std::cout << " " << tok;
        }
        std::cout << "\n";

    } catch (const std::exception &e) {
        std::cout << "STATUS: ERROR\n";
        std::cout << "ERROR: " << e.what() << "\n";
    }

    return 0;
}
