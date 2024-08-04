#pragma once

using namespace std;

// Enum representing the color of a node in the Red-Black Tree.
enum Color { RED, BLACK };

// Structure representing a node in the Red-Black Tree.
struct Node {
    string state; // State name.
    list<hashTableVars> diseases; // List of disease records associated with the state.
    bool color; // Color of the node (RED or BLACK).
    Node* left; // Pointer to the left child.
    Node* right; // Pointer to the right child.
    Node* parent; // Pointer to the parent node.

    // Constructor to initialize a node with a state and disease information.
    Node(const string& key, const hashTableVars& info) : state(key), color(RED), left(nullptr), right(nullptr), parent(nullptr) {
        diseases.push_back(info);
    }
};

// Class representing a Red-Black Tree.
class RBTree {
private:
    Node* root; // Pointer to the root node of the tree.
    Node* TNULL; // Pointer to the sentinel null node used to simplify tree operations.

    /**
     * Initialize a NULL node (sentinel) with default values.
     * @param node The node to initialize.
     * @param parent The parent of the NULL node.
     */
    void initializeNULLNode(Node* node, Node* parent) {
        node->state = "";
        node->color = BLACK;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
    }

    /**
     * Helper function to search for a node with a given key in the tree.
     * @param node The node to start the search from.
     * @param key The key to search for.
     * @return The node with the given key, or TNULL if not found.
     */
    Node* searchTreeHelper(Node* node, const string& key) {
        if (node == TNULL || key == node->state) {
            return node;
        }

        if (key < node->state) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    /**
     * Balance the tree after inserting a new node.
     * @param k The newly inserted node.
     */
    void balanceInsert(Node* k) {
        Node* u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            }
            else {
                u = k->parent->parent->right;

                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                }
                else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    /**
     * Perform a left rotation on the given node.
     * @param x The node to rotate.
     */
    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    /**
     * Perform a right rotation on the given node.
     * @param x The node to rotate.
     */
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

public:
    // Constructor to initialize the Red-Black Tree.
    RBTree() {
        TNULL = new Node("", hashTableVars("", 0, 0, ""));
        TNULL->color = BLACK;
        root = TNULL;
    }

    /**
     * Insert a new record into the Red-Black Tree.
     * If the state already exists, update the disease information if necessary.
     * @param key The state key.
     * @param info The disease information to insert.
     */
    void insert(const string& key, const hashTableVars& info) {
        Node* node = new Node(key, info);
        node->parent = nullptr;
        node->left = TNULL;
        node->right = TNULL;
        node->color = RED;

        Node* y = nullptr;
        Node* x = this->root;

        // Find the appropriate place to insert the new node
        while (x != TNULL) {
            y = x;
            if (node->state == x->state) {
                for (auto& entry : x->diseases) {
                    if (entry.disease == info.disease && entry.year == info.year && entry.isMortality == info.isMortality) {
                        if (info.deathCount > entry.deathCount) {
                            entry.deathCount = info.deathCount;
                        }
                        // Found the duplicate, no need to add new entry
                        delete node;  // Free the allocated memory for the new node
                        return;
                    }
                }
                // If no exact match found, add the new info
                x->diseases.push_back(info);
                delete node;  // Free the allocated memory for the new node
                return;
            }
            if (node->state < x->state) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        // Set the parent of the new node
        node->parent = y;
        if (y == nullptr) {
            root = node;
        }
        else if (node->state < y->state) {
            y->left = node;
        }
        else {
            y->right = node;
        }

        // Fix any violations of the Red-Black Tree properties
        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        balanceInsert(node);
    }

    /**
     * Display the death count for a given state and disease.
     * @param state The state to query.
     * @param disease The disease to query.
     */
    void displayDeathCount(const string& state, const string& disease) {
        Node* node = searchTreeHelper(this->root, state);
        if (node == TNULL) {
            cout << "State " << state << " not found.\n";
            return;
        }

        bool diseaseFound = false;
        for (const auto& entry : node->diseases) {
            if (entry.disease == disease) {
                cout << "State: " << state << " Disease: " << disease
                     << " Year: " << entry.year
                     << " Death Count: " << entry.deathCount
                     << " Mortality: " << entry.isMortality << "\n";
                diseaseFound = true;
            }
        }

        if (!diseaseFound) {
            cout << "Disease " << disease << " not found in state " << state << ".\n";
        }
    }
};
