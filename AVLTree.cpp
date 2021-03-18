#include <iostream>
#include <algorithm>
#include <cmath>

// Forward Declaration of Template that will be used as Member in other Templates
template <class T>
class AVLNode;

template <class T>
class AVLTree
{
private:
    AVLNode <T> *root;

public:
    AVLTree()
    {
        root = nullptr;
    }

    // ADT Interface Functions
    void print_item (AVLNode <T> *node);
    void print_height (T vertex);
    AVLNode <T>* find_min (AVLNode <T> *current);
    AVLNode <T>* find_max (AVLNode <T> *current);
    AVLNode <T>* successor (T x);
    AVLNode <T>* predecessor (T x);
    int height(AVLNode <T> *node);
    AVLNode <T>* search (T x);

    void insert (T x);
    void remove (T x);
    void removeHeight (AVLNode <T> *traversal);

    void inorder (AVLNode <T> *node);
    void preorder (AVLNode <T> *node);
    void postorder (AVLNode <T> *node);

    void right_rotate (AVLNode <T> *vertex);
    void left_rotate (AVLNode <T> *vertex);
    int AVLInvariantCheck (T x);
    int LeftHeavy_or_RightHeavy(AVLNode <T> *BranchedOffVertex);
    void BalanceAdjustment(AVLNode <T> *vertex);
    void BalanceWeight(AVLNode <T> *node);

    T selectRank (int x);
    int getRank (AVLNode <T> *node);

    friend AVLNode <T>;
};

template <class T>
class AVLNode
{
private:
    T item;
    int height;
    int weight;
    AVLNode <T> *left;
    AVLNode <T> *right;
    AVLNode <T> *parent;

public:
    AVLNode (T x)
    {
        item = x;
        height = 0;
        weight = 0;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }


    friend  AVLTree <T>;
};

int main () {
    AVLTree<int> AVL;
}

template <class T>
T AVLTree <T> :: selectRank (int actual_rank)  // Gets value of Node that is of Rank x
{
    int subtree_rank;
    AVLNode <T> *traversal = root;

    // Get Rank of Node you are currently at, by peeking at Weight of Left Child
    // Rank of Node will be in relation to Left Sub-Tree, NOT in relation to ENTIRE Sub-Tree
    subtree_rank = (traversal -> left -> weight) + 1;

    // Comparing SubTree Rank with Actual Rank Value

    if (subtree_rank == actual_rank)   // If Subtree Rank equals Actual Rank
        return traversal->item;        // We have found our Node

    else if (actual_rank < subtree_rank)  // If rank we want is smaller than our current Node's Subtree Rank
        return (traversal -> left). selectRank(actual_rank);  // We progress down Left Child

    else if (actual_rank > subtree_rank)  // If rank we want is larger than our current Node's Subtree Rank
    {
        int number_of_skipped_nodes = getRank(traversal -> left);    // Since left path has nodes SMALLER than the Node we want rank of, we must offset number of nodes accordingly
        return (traversal -> right).selectRank(actual_rank - number_of_skipped_nodes);  // We progress down Right Child
    }
}

template <class T>
int AVLTree<T> :: getRank(AVLNode <T> *node)   // Finds the Rank Value of specified Node
{
    // Calculating initial value of Rank (based on specified Node's LEFT Subtree)
    int rank = (node -> left -> weight) + 1;   // Rank of Node is (Weight of Left Child + 1), since Left Child will have
                                               // (weight number) of nodes that are smaller than Node

    // We want to find the TOTAL number of Nodes smaller than SPECIFIED NODE, as this will tell us
    // the rank easily (Rank of SPECIFIED NODE = TOTAL number of SMALLER Nodes + 1)

    do
    {
        AVLNode <T> *original_position = node;
        node = node -> parent;

        if (node -> right == original_position)
        {
           // Parent's left-subtree has Nodes that are smaller than Original Node
           // Hence the number of these nodes must be added to Rank Value of Original Node

           rank = rank + (node -> left -> weight);   // Adding Parent's "Left-Subtree Number" of Nodes
           rank = rank + 1;                          // Adding Parent itself (it is smaller than Specified Node, since Specified Node is on it's Right Sub-Path
        }

        if (node -> left == original_position)
        {
            // Since SPECIFIED NODE is on the Parent's LEFT Sub-Path, Parent itself is larger than Node, hence not included
            // in caculation of "Nodes smaller than SPECIFIED Node"
            continue;
        }
    } while (node != nullptr);

    return rank;
}

template <class T>
void AVLTree <T> :: BalanceAdjustment (AVLNode <T> *vertex)
{
    int scenario = AVLInvariantCheck( vertex -> item);

    if (scenario == 0)
        return;

    // Unbalanced Vertex, Left Heavy
    else if (scenario == 1)
    {
        if (LeftHeavy_or_RightHeavy(vertex -> left) == 2 )   // Vertex->Left is Right Heavy as well
        {
            left_rotate(vertex->left);                       // Left Rotate Vertex->Left
        }

        right_rotate(vertex);
    }

    else // Unbalanced, Right Heavy
    {
        if (LeftHeavy_or_RightHeavy(vertex -> right) == 1 )   // Vertex->Right is Left Heavy as well
        {
            right_rotate(vertex->left);                       // Right Rotate Vertex->Right
        }

        left_rotate(vertex);
    }
}

template <class T>
int AVLTree <T> ::LeftHeavy_or_RightHeavy(AVLNode <T> *BranchedOffVertex)
{
    int left_subtree_height = height( BranchedOffVertex -> left);
    int right_subtree_height = height( BranchedOffVertex -> right);

    // Balanced
    if (left_subtree_height == right_subtree_height)
        return 0;

    // Left Heavy
    else if (left_subtree_height > right_subtree_height)
        return 1;

    // Right Heavy
    else
        return 2;
}

template <class T>
int AVLTree <T> :: AVLInvariantCheck (T x)
{
    AVLNode <T> *NodeToCheck = search (x);

    // Checking Left Subtree Height
    int left_subtree_height;
    if (NodeToCheck -> left == nullptr)
        left_subtree_height = -1;
    else
        left_subtree_height = (NodeToCheck -> left) -> height;

    // Checking Right Subtree Height
    int right_subtree_height;
    if (NodeToCheck -> right == nullptr)
        right_subtree_height = -1;
    else
        right_subtree_height = (NodeToCheck -> right) -> height;


    // If AVL Tree is balanced
    if ( abs(left_subtree_height - right_subtree_height) <= 1 )
        return 0;

    else
    {
        if (left_subtree_height > right_subtree_height)
            return 1; // Left Heavy

        else
            return 2; // Right Heavy
    }
}

template <class T>
void AVLTree <T> :: right_rotate(AVLNode <T>* vertex)
{
    // For Right Rotation of Node Vertex, Vertex must have Left Child
    assert(vertex -> left != NULL);

    AVLNode <T> *vertex_original_parent = vertex -> parent;  // Storing Vertex's Original Parent
    AVLNode <T> *leftie = vertex -> left;                    // Storing Subtree on Vertex's Left

    AVLNode <T> *original_vertex = vertex;
    AVLNode <T> *original_vertex_left_child = vertex -> left;
    AVLNode <T> *original_vertex_right_child = vertex -> right;
    AVLNode <T> *original_vertex_left_child_left_child = vertex -> left -> left;
    AVLNode <T> *original_vertex_left_child_right_child = vertex -> left -> right;


    vertex -> parent = leftie;            // Leftie is now Vertex's Parents
    AVLNode <T> *temp = leftie -> right;  // For usage later
    leftie -> right = vertex;             // Leftie's Child is now Vertex

    // Reconnecting Vertex's Original Parent to Leftie (downwards)
    if (vertex_original_parent != nullptr && vertex_original_parent -> right == vertex)
        vertex_original_parent -> right = leftie;

    else if (vertex_original_parent != nullptr && vertex_original_parent -> left == vertex)
        vertex_original_parent -> left = leftie;

    leftie -> parent = vertex_original_parent;    // Reconnecting Leftie to Vertex Original Parent's (upwards)

    // Leftie's Right will now be Vertex's Left
    vertex -> left = temp;       // Downwards Adjustment (Parent - Child)

    if (temp != nullptr)
        temp -> parent = vertex; // Upwards Adjustment (Child - Parent)

    if ( leftie -> parent == nullptr )
        root = leftie;

    // Updating of Heights
    while (vertex != nullptr)
    {
        vertex -> height = height(vertex);
        vertex = vertex -> parent;
    }

    // Update of Weights
    int weight1, weight2, weight3;

    assert(original_vertex_left_child_left_child != nullptr);
    weight1 = original_vertex_left_child_left_child -> weight;

    if (original_vertex_left_child_right_child != nullptr)   // Possible for it to be NULLPTR
        weight2 = original_vertex_left_child_right_child -> weight;

    else // original_vertex_left_child_right_child must be NULLPTR
        weight2 = 0;

    if (original_vertex_right_child != nullptr)             // Possible for it to be NULLPTR
        weight3 = original_vertex_right_child -> weight;

    else // original_vertex_left_child must be NULLPTR
        weight3 = 0;

    original_vertex_left_child -> weight = weight1 + weight2 + weight3 + 2;
    original_vertex -> weight = weight2 + weight3 + 1;
}

template <class T>
void AVLTree <T> :: left_rotate(AVLNode <T>* vertex)
{
    // For Left Rotation of Node Vertex, Vertex must have Right Child
    assert(vertex -> right != NULL);

    AVLNode <T> *vertex_original_parent = vertex -> parent;
    AVLNode <T> *rightie = vertex -> right;


    AVLNode <T> *original_vertex = vertex;
    AVLNode <T> *original_vertex_left_child = vertex -> left;
    AVLNode <T> *original_vertex_right_child = vertex -> right;
    AVLNode <T> *original_vertex_right_child_left_child = vertex -> right -> left;
    AVLNode <T> *original_vertex_right_child_right_child = vertex -> right -> right;

    vertex -> parent = rightie;
    AVLNode <T> *temp = rightie -> left;
    rightie -> left = vertex;

   if (vertex_original_parent != nullptr && vertex_original_parent -> right == vertex)
       vertex_original_parent -> right = rightie;

   else if (vertex_original_parent != nullptr && vertex_original_parent -> left == vertex)
       vertex_original_parent -> left = rightie;

   rightie -> parent = vertex_original_parent;

   vertex -> right = temp;

   if (temp != nullptr)
       temp -> parent = vertex;

    if ( rightie -> parent == nullptr )
        root = rightie;

    // Update of Heights
    while (vertex != nullptr)
    {
        vertex -> height = height(vertex);
        vertex = vertex -> parent;
    }

    // Update of Weights
    int weight1, weight2, weight3;

    assert(original_vertex_right_child_right_child != nullptr);
    weight1 = original_vertex_right_child_right_child-> weight;

    if (original_vertex_right_child_left_child != nullptr)   // Possible for it to be NULLPTR
        weight2 = original_vertex_right_child_left_child-> weight;
    else
        weight2 = 0;

    if (original_vertex_left_child != nullptr)
        weight3 = original_vertex_left_child -> weight;
    else
        weight3 = 0;

    original_vertex_right_child -> weight = weight1 + weight2 + weight3 + 2;
    original_vertex -> weight = weight1 + weight2 + 1;
}

template <class T>
AVLNode <T>* AVLTree <T> :: search (T x)
{
    AVLNode <T> *current = root;

    while (current != nullptr)
    {
        if (current -> item == x)        // Target found
            return current;

        else if (current -> item < x)   // If Target larger than Current
            current = current -> right; // Search Right Half of Sub-Tree

        else
            current = current -> left;  // Search Left Half of Sub-Tree
    }

    // If Target cannot be found, return NULL
    return nullptr;
}

template <class T>
void AVLTree <T> :: insert (T x)
{
    /* Optional Method to ensure that No Duplicate Node is inserted
    TreeNode<T> *duplicate_check = search(x);
    if (duplicate_check == nullptr)
        return;
    */

    // if Inserting into EMPTY AVL
    if (root == nullptr)
    {
        root = new AVLNode<T>(x);
        return;
    }

    AVLNode<T> *current = root;
    AVLNode<T> *NodeJustInsertedMantainBalance = nullptr;
    AVLNode <T> *NodeJustInsertedMantainWeight = nullptr;

    while ( current != nullptr)
    {
        if ( x < current -> item)
        {
            if ( current -> left != nullptr)
            {
                current = current -> left;
                continue;
            }

            else
            {
                current -> left = new AVLNode <T> (x);
                NodeJustInsertedMantainBalance = current -> left;
                NodeJustInsertedMantainWeight = current -> left;
                (current -> left) -> parent = current;
                break;
                //return;
            }
        }

        else // x > current -> item
        {
            if (current -> right != nullptr)
            {
                current = current -> right;
                continue;
            }

            else
            {
                current -> right = new AVLNode <T> (x);
                NodeJustInsertedMantainBalance = current -> right;
                NodeJustInsertedMantainWeight = current -> right;
                (current -> right) -> parent = current;
                break;
                //return;
            }
        }
    }

    // Function to update Height of INSERTED NODE
    while (current != nullptr)
    {
        current -> height = std::max ( height(current -> left), height(current -> right) ) + 1;
        current = current -> parent;
    }

    // Mantain Weight of Inserted Node's CHAIN of Parents (up to the Root of AVLTree)
    // Only need to be concerned with Node's CHAIN of Parents, since inserting Node will cause number of Nodes under
    // these CHAIN of parents to be increased by exactly one.
    do
    {
        NodeJustInsertedMantainWeight = NodeJustInsertedMantainWeight -> parent;
        NodeJustInsertedMantainWeight -> weight += 1;
    } while (NodeJustInsertedMantainWeight -> parent != nullptr);

    // Mantain AVL Tree Balance
    while (NodeJustInsertedMantainBalance != nullptr)
    {
        BalanceAdjustment(NodeJustInsertedMantainBalance);
        NodeJustInsertedMantainBalance = NodeJustInsertedMantainBalance -> parent;
    }
}

template <class T>
void AVLTree <T> :: remove (T x)
{
    AVLNode <T> *traversal = nullptr;
    AVLNode <T> *current = root;

    while (current != nullptr)
    {
        if (current -> item == x)
        {
            // if Current is Leaf Node (no children)
            if ( height (current) == 0 )
            {
                if ( (current-> parent) -> right == current )
                    (current->parent) -> right = nullptr;

                else
                    (current->parent) -> left = nullptr;


                traversal = current -> parent;
                delete (current);
                removeHeight(traversal);
                BalanceAdjustment(traversal);
                BalanceWeight(traversal);
                return;
            }

                // if Current has ONE CHILD ONLY
            else if ( ( current -> left != nullptr && current -> right == nullptr )
                      || ( current -> left == nullptr && current -> right != nullptr) )
            {
                traversal = current -> parent;

                if (current->right == nullptr)  // Current must have a LEFT sub-tree
                {
                    (current->left)->parent = current->parent;  // Linking Below Child to Above Parent

                    // Linking Above Parent to Below Child (dependent on Current Position)
                    if ((current->parent != nullptr) && (current->parent)->right == current)     // Below Child is on Current Right
                    {
                        (current->parent)->right = current->left;   // Above Parent Right will link to Below Child (on Current's Left)
                    }

                    else if ((current->parent != nullptr) && (current->parent)->left == current)  // Below Child is on Current Left
                        (current->parent)->left = current->left;  // Above Parent Left will link to Below Child (on Current's Left)

                    else
                    {
                        assert (current -> parent == nullptr);
                        (current-> left) -> parent = nullptr;
                        root = current -> left;
                    }
                }

                else  // Current must have a RIGHT sub-tree
                {
                    (current->right)->parent = current->parent;  // Linking Child to Parent

                    // Linking Above Parent to Below Child (dependent on Current Position)
                    if ((current->parent != nullptr) && (current->parent)->right == current)     // Below Child is on Current Right
                    {
                        (current->parent)->right = current->right;   // Above Parent Right will link to Below Child (on Current's Left)
                    }

                    else if ((current->parent != nullptr) && (current->parent)->left == current)  // Below Child is on Current Left
                        (current->parent)->left = current->right;  // Above Parent Left will link to Below Child (on Current's Left)

                    else
                    {
                        assert (current -> parent == nullptr);
                        (current -> right) -> parent = nullptr;
                        root = current -> right;
                    }
                }

                delete (current);
                removeHeight(traversal);
                BalanceAdjustment(traversal);
                BalanceWeight(traversal);
                return;
            }

            else // Current must have TWO CHILDREN ONLY
            {
                T temp = ( successor(current->item) ) -> item;    // Storing Successor ITEM of Current in Temp
                remove ( (successor(current->item))->item);    // Remove Successor NODE
                current -> item = temp;                           // Change Current to Successor ITEM VALUE

                return;
            }
        }

        else if (current -> item < x)
            current = current -> right;

        else if (current -> item > x)
            current = current -> left;
    }
}

template <class T>
void AVLTree <T> :: BalanceWeight(AVLNode <T> *node)
{
    while (node != nullptr)
    {
        node -> weight -= 1;
        node = node -> parent;
    }
}

template <class T>
void AVLTree <T> :: removeHeight(AVLNode <T> *traversal)
{
    while (traversal != NULL)
    {
        traversal -> height = std::max( height(traversal -> left), height(traversal -> right) ) + 1;
        traversal = traversal -> parent;
    }
}

template <class T>
void AVLTree <T> :: inorder(AVLNode <T> *node)
{
    if (node == nullptr)
        return;

    inorder (node -> left);
    std::cout << node -> item << " " ;
    inorder (node -> right);
}

template <class T>
void AVLTree <T> :: preorder(AVLNode <T> *node)
{
    if (node == nullptr)
        return;

    std::cout << node -> item << " " ;
    preorder( node -> left);
    preorder(node -> right);
}

template <class T>
void AVLTree <T> :: postorder(AVLNode <T> *node)
{
    if (node == nullptr)
        return;

    postorder( node -> left);
    postorder(node -> right);

    std::cout << node -> item << " " ;
}

template <class T>
int AVLTree <T> :: height (AVLNode <T> *node)
{
    int left_subtree_height = -1;
    int right_subtree_height = -1;

    if (node == nullptr)    // If pointing to NULL, height of that Node is zero
        return -1;

    if (node -> left != nullptr)
        left_subtree_height = height (node -> left);

    if (node -> right != nullptr)
        right_subtree_height = height (node -> right);

    return ( std::max(left_subtree_height, right_subtree_height) + 1 );

}

template <class T>
AVLNode <T>* AVLTree <T> :: successor (T x)
{
    AVLNode <T> *current = search (x);   // Get Pointer to Node X

    if (current == nullptr)
        std::cout << "No such Node exists in the AVL Tree.";

    /* Alternative Implementation of Successor value where Node is not in the BST initially
    if (current == nullptr)
    {
        T successor_value = successor(x-1);
        return successor_value;
    }
    */

    if ( current -> right != nullptr)        // If there is a RIGHT BRANCH
        return find_min(current -> right);   // Find the MINIMUM of RIGHT SUBTREE (Smallest of the Largest)

    else    // there is NO RIGHT BRANCH
    {
        AVLNode <T> *ancestor = current -> parent;
        AVLNode <T> *descendant = current;

        while (ancestor != nullptr && descendant == ancestor -> right)
        {
            descendant = ancestor;
            ancestor = descendant -> parent;
        }

        if (ancestor == nullptr)   // If x has NO successor (it is the largest in AVL)
        {
            return nullptr;
        }

        else
            return ancestor;
    }
}

template <class T>
AVLNode <T>* AVLTree <T> :: predecessor (T x)
{
    AVLNode <T> *current = search (x);   // Get Pointer to Node X

    if (current == nullptr)
        std::cout << "No such Node exists in the AVL Tree.";

    if ( current -> left != nullptr)      // If there is a LEFT BRANCH
    {
        return find_max(current -> left); // Find the MAXIMUM of LEFT SUBTREE (Largest of the Smallest)
    }

    else   // There is NO Left Branch
    {
        AVLNode <T> *ancestor = current -> parent;
        AVLNode <T> *descendant = current;

        while (ancestor != nullptr && descendant == ancestor -> left)
        {
            descendant = ancestor;
            ancestor = descendant -> parent;
        }

        if (ancestor == nullptr)    // If X has NO predecessor (it is the smallest in AVL)
        {
            return nullptr;
        }

        else
            return ancestor;
    }
}

template <class T>
AVLNode <T>* AVLTree <T> :: find_min (AVLNode <T> *current)
{
    while (current -> left != nullptr)
    {
        current = current -> left;
    }

    return current;
}

template <class T>
AVLNode <T>* AVLTree <T> :: find_max (AVLNode <T> *current)
{
    while (current -> right != nullptr)
    {
        current = current -> right;
    }

    return current;
}

template <class T>
void AVLTree <T> :: print_item (AVLNode <T> *node)
{
    if (node == nullptr)
    {
        std::cout << "Error printing Node";
        return;
    }

    std::cout << node -> item << " ";
}

template <class T>
void AVLTree <T> :: print_height (T vertex)
{
    AVLNode <T>* node = search(vertex);
    std::cout << "Height of Node " << node -> item << ", is " << node -> height;
    std::cout << std::endl;
}