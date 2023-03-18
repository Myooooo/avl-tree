#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// define the node of the AVL tree
struct Node {
    int val;
    Node* left;
    Node* right;
    int height;

    // constructor using node value
    Node(int val) {
        this->val = val;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

// returns the height of node
// 0 if does not exist
int getHeight(Node* node) {
    if(node == nullptr) return 0;
    return node->height;
}

// update the height of node according to its children
void updateHeight(Node* node) {
    int left = getHeight(node->left);
    int right = getHeight(node->right);

    // set height to the largest child height plus one
    if(left > right) {
        node->height = left + 1;
    } else {
        node->height = right + 1;
    }
}

// rotate left at node
// returns the new root node
Node* rotateLeft(Node* node) {
    // store original sub-trees
    Node* right = node->right;
    Node* left = right->left;

    // replace sub-trees
    right->left = node;
    node->right = left;

    // update height of modified nodes
    updateHeight(node);
    updateHeight(right);

    // right node is new root
    return right;
}

// rotate right at node
// returns the new root node
Node* rotateRight(Node* node) {
    // store original sub-trees
    Node* left = node->left;
    Node* right = left->right;

    // replace sub-trees
    left->right = node;
    node->left = right;

    // update height of modified nodes
    updateHeight(node);
    updateHeight(left);

    // left node is new root
    return left;
}

// returns the balance factor of a node
// 0 if does not exist
int getBalanceFactor(Node* node) {
    if(node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// check the balance factor and rebalance at node
// returns the new leaf node
Node* rebalance(Node* node) {

    // root does not exist, terminate
    if(node == nullptr) return nullptr;

    // calculate the balance factor of current node
    int bf = getBalanceFactor(node);
    //cout << node->val << " bf: " << bf << endl;

    // rebalance the tree
    if(bf > 1) {
        // AVL tree is left heavy
        if(getBalanceFactor(node->left) >= 0) {
            // right rotation
            node = rotateRight(node);
        } else {
            // left-right rotation
            node->left = rotateLeft(node->left);
            node = rotateRight(node);
        }
    } else if(bf < -1) {
        // AVL tree is right heavy
        if(getBalanceFactor(node->right) <= 0) {
            // left rotation
            node = rotateLeft(node);
        } else {
            // right-left rotation
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
        }
    }

    return node;
}

// recursively find the node with maximum value in root subtree
Node* findMaxNode(Node* root) {
    if(root == nullptr) return nullptr;
    if(root->right == nullptr) return root;
    return findMaxNode(root->right);
}

// recursively insert val into the AVL tree
// returns the node with val
Node* addNode(int val,Node* root) {

    // root does not exist, create new root node
    if(root == nullptr) return new Node(val);

    // insert according BST rule
    if(val > root->val) {
        // greater than current node, insert to the right
        root->right = addNode(val,root->right);
    } else if (val < root->val) {
        // less than current node, insert to the left
        root->left = addNode(val,root->left);
    } else {
        // input value exist, return itself
        return root;
    }

    // update height of current node
    updateHeight(root);

    // rebalance the tree
    root = rebalance(root);

    // return balanced root
    return root;
}

// recursively delete val from the AVL tree
// returns the new root node
Node* deleteNode(int val,Node* root) {

    // root does not exist, terminate
    if(root == nullptr) return nullptr;

    // delete according to BST rules
    if(val > root->val) {
        // greater than current node, insert to the right
        root->right = deleteNode(val,root->right);
    } else if (val < root->val) {
        // less than current node, insert to the left
        root->left = deleteNode(val,root->left);
    } else {
        // node found
        if(root->left == nullptr && root->right == nullptr) {
            // leaf node, delete directly
            delete root;
            return nullptr;
        } else if(root->left != nullptr && root->right == nullptr) {
            // only left child
            Node* tmp = root->left;
            delete root;
            return tmp;
        } else if(root->left == nullptr && root->right != nullptr) {
            // only right child
            Node* tmp = root->right;
            delete root;
            return tmp;
        } else {
            // two children
            // find the largest node in the left subtree and replace current node
            Node* max = findMaxNode(root->left);
            // replace current node
            root->val = max->val;
            // delete from original subtree
            root->left = deleteNode(max->val,root->left);
        }
    }

    // at this stage the sub-tree of current node has been modified
    // update height of current node
    updateHeight(root);

    // rebalance the tree
    root = rebalance(root);

    // return balanced root
    return root;
}

// traverse the AVL tree preorder from root
// root -> left -> right
void traversePre(Node* root) {
    // root does not exist
    if(root == nullptr) return;

    // traverse the AVL tree root -> left -> right
    cout << root->val << " ";
    traversePre(root->left);
    traversePre(root->right);
}

// traverse the AVL tree postorder from root
// left -> right -> root
void traversePost(Node* root) {
    // root does not exist
    if(root == nullptr) return;

    // traverse the AVL tree left -> right -> root
    traversePost(root->left);
    traversePost(root->right);
    cout << root->val << " ";
}

// traverse the AVL tree inorder from root
// left -> root -> right
void traverseIn(Node* root) {
    // root does not exist
    if(root == nullptr) return;

    // traverse the AVL tree left -> root -> right
    traverseIn(root->left);
    cout << root->val << " ";
    traverseIn(root->right);
}

// delete nodes in the AVL tree and free memory
void deleteTree(Node* root) {
    // root does not exist
    if(root == nullptr) return;

    if(root->left == nullptr && root->right == nullptr) {
        // leaf node
        delete root;
        return;
    } else {
        // none leaf node
        deleteTree(root->left);
        deleteTree(root->right);
    }
}

int main() {

    // get input from terminal
    string input;
    getline(cin,input);

    // format input using string stream
    stringstream ss(input);

    // initialise AVL tree root
    Node* root = nullptr;
    
    // iterate through each operation
    for(string op; ss >> op;) {

        if(op[0] == 'A') {

            // add operation
            // convert value to int
            int val = stoi(op.substr(1));
            root = addNode(val,root);

            // cout << "Adding to tree: " << val << endl;
            // cout << "\t";
            // traversePost(root);
            // cout << endl;

        } else if(op[0] == 'D') {

            // delete operation
            // convert value to int
            int val = stoi(op.substr(1));
            root = deleteNode(val,root);

            // cout << "Deleting from tree: " << val << endl;
            // cout << "\t";
            // traversePost(root);
            // cout << endl;

        } else {
            // finishing move
            // AVL tree is empty
            if(root == nullptr) {
                cout << "EMPTY";
                break;
            }
            
            // traverse and print value
            if(op == "PRE") {
                // traverse preorder
                traversePre(root);
            } else if(op == "POST") {
                // traverse postorder
                traversePost(root);
            } else if(op == "IN") {
                // traverse inorder
                traverseIn(root);
            } else {
                // unknown command
            }
        }
    }

    cout << endl;

    // free up memory
    deleteTree(root);

    return 0;
}
