/**
 *  * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

#define bool _Bool
#define true 1
#define false 0

bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    if (p == q)
        return true;
    if (!p || !q)
        return false;
    if (p->val != q->val)
        return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
