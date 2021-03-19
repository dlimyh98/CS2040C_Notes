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
    AVLTree() {root = nullptr;}

    // ADT Interface Functions
    AVLNode <T>* find_min (AVLNode <T> *current);
    AVLNode <T>* find_max (AVLNode <T> *current);
    AVLNode <T>* successor (T x);
    AVLNode <T>* predecessor (T x);

    int height(AVLNode <T> *node);
    AVLNode <T>* search (T x);
    void insert (T x);
    void remove (T x);
    void removeHeight (AVLNode <T> *traversal);
    void print_item (AVLNode <T> *node);
    void print_height (T vertex);
    void print_weight (T vertex);

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
    void print_weight(T vertex);

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
    AVLNode (T x) {item = x; height = 0; weight = 1; parent = nullptr; left = nullptr; right = nullptr;}
    /*
     * Weight of Node is size of the tree ROOTED at THAT NODE
     * - w(leaf) = 1
     * - w(v) = w(v.left) + w(v.right) + 1
     */

    friend  AVLTree <T>;
};

int main () {
    AVLTree<int> AVL;
}

template <class T>
void AVLTree <T> :: _selectRank(int actual_rank)     // Gets value of Node that is of Rank x
{
    T ranked_node = selectRank(actual_rank, root);
    std::cout << ranked_node << " ";
}

/**
 *
 * @param actual_rank     Rank value we are interested in
 * @param traversal       Pointer to node that we are currently 'traversing' at
 * @return                NODE value that has Rank Value "actual_rank"
 *
 * This function attempts to find the NODE value that has the Rank Value "actual_rank"
 * We can do this in TWO parts.
 *
 * Part 1   We obtain the RANK of the node we are CURRENTLY at, by peeking at Weight of Left Child
 *          Note that the RANK value we obtain here is w.r.t the LEFT SUB-TREE of our CURRENT node
 *
 * Part 2   We compare the Rank value calculated above, with the "actual_rank" value we are interested in
 *          - If "actual_rank" is smaller than the Rank Value calculated above, we must travel down the LEFT SUBTREE of the node we are CURRENTLY at
 *            This works, because the LEFT SUBTREE contains all the nodes smaller than the node we are CURRENTLY at, so a node with SMALLER rank will be in this LEFT SUBTREE
 *
 *          - If "actual_rank" is larger than the Rank Value calculated above, we must travel down the RIGHT SUBTREE of the node we are CURRENTLY at
 *            1. Why don't we travel up the 'Parent Chain'? Since we started calculating Rank Value from the ROOT downwards, we will never need to traverse up 'Parent Chain'
 *            2. We must appropriately offset the "actual_rank" value that we are interested in.
 *               When we traverse down the RIGHT SUBTREE, we are implicitly stating that the LEFT SUBTREE contains nodes that are SMALLER than node we are currently at (as it should be)
 *               Hence, we must minus all these LEFT SUBTREE nodes from the "actual_rank" value that we are interested in, so that when we traverse down the RIGHT SUBTREE, we will
 *               be calculating the correctly adjusted "actual_rank" value
 *
 */
template <class T>
T AVLTree <T> :: selectRank (int actual_rank, AVLNode<T> *traversal)
{
    int subtree_rank;

    // Get Rank of Node you are currently at, by peeking at Weight of Left Child
    // Rank of Node will be in relation to Left Sub-Tree, NOT in relation to ENTIRE Sub-Tree
    // We choose Left Subtree because, Left Subtree has ALL the nodes smaller than you currently. Hence, +1 to that gives us exactly the rank
    if (traversal -> left != nullptr)
        subtree_rank = (traversal -> left -> weight) + 1;

    else
        subtree_rank = 1;

    // Comparing SubTree Rank with Actual Rank Value
    if (subtree_rank == actual_rank)   // If Subtree Rank equals Actual Rank
        return traversal->item;        // We have found our Node

    else if (actual_rank < subtree_rank)  // If rank we want is smaller than our current Node's Subtree Rank
        return ( selectRank(actual_rank, traversal -> left));    // We progress down Left Child

    else // (actual_rank > subtree_rank)  -- If rank we want is larger than our current Node's Subtree Rank
    {
        int number_of_skipped_nodes = num_nodes_offset(traversal);    // Since left path has nodes SMALLER than the Node we want rank of, we must offset number of nodes accordingly
        return ( selectRank(actual_rank - number_of_skipped_nodes, traversal -> right));  // We progress down Right Child
    }
}

template <class T>
int AVLTree<T> :: num_nodes_offset(AVLNode <T> *node)
{
    return ( node -> left -> weight) + 1;
}

template <class T>
void AVLTree<T> :: _getRank(T x)
{
    AVLNode<T> *node = search(x);
    int rank_value = getRank(node);
    std::cout << rank_value << " ";
}

/**
 *
 * @param node     Pointer to Node that we want to find Rank of
 * @return         Returns Rank Value of Node (i.e. how many nodes are smaller than the specified node?)
 *
 * This function will attempt to find the Rank Value of a Node.
 * Note that this Rank Value is w.r.t THE ENTIRE AVL TREE, not just a specific subset/sub-branch of the AVL Tree.
 *
 * The key idea behind this function, is to find the number of nodes that are smaller then our specified Node. We can calculate this in TWO parts.
 *
 * Part 1 - The Left Sub-Tree of our specified Node will have nodes that are SMALLER than our specified node, so they form part of our rank.
 *        - We know the number of nodes in the Left Subtree, by considering the WEIGHT of the LEFT-SUBTREE's FIRST NODE
 *
 * Part 2 - We must travel up the 'Parent Chain', to find other nodes that are smaller than us. TWO more SUB-CONSIDERATIONS.
 * 2.1      When we travel up the 'Parent Chain', the node we WERE at is currently the LEFT CHILD of the node we ARE at
 *          We DO NOT need to consider ANY of these nodes, since the nodes we WERE at will be LARGER than all these nodes
 *
 * 2.2      When we travel up the 'Parent Chain, the node we WERE at is currently the RIGHT CHILD of the node we ARE at
 *          We MUST consider the NODE we ARE at, as well as the ENTIRE LEFT SUBTREE of the NODE WE ARE AT, since these nodes are SMALLER than the node we WERE at.
 *
 */
template <class T>
int AVLTree<T> :: getRank(AVLNode <T> *node)   // Finds the Rank Value of specified Node
{
    // Calculating initial value of Rank (based on specified Node's LEFT Subtree)
    int rank;

    if (node -> left == nullptr)
        rank = 0 + 1;                         // rank(null) = 0, plus 1 to include the CURRENT node

    else
        rank = (node -> left -> weight) + 1;  // Rank of Node is (Weight of Left Child + 1), since Left Child will have
    // (weight number) of nodes that are smaller than Node, plus one to include CURRENT node

    // We want to find the TOTAL number of Nodes smaller than SPECIFIED NODE, as this will tell us
    // the rank easily (Rank of SPECIFIED NODE = TOTAL number of SMALLER Nodes + 1)

    while (node -> parent != nullptr)
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
    }

    return rank;
}

/**
 *
 * @param vertex    Pointer to OFFENDING NODE that needs to be Adjusted
 *
 * This function controls HOW we Left Rotate / Right Rotate the offending node.
 * 1. We find out which Scenario our offending Node falls into
 * 2. We find out whether the vertex's appropriate child is Left or Right or Balanced Heavy
 * 3. We balance accordingly as below
 *
 * Recall that there are FOUR possible scenarios
 *
 * SCENARIO 1 - Node is Left-Right Heavy
 * 1. left_rotate(v.left)
 * 2. right_rotate(v)
 *
 * SCENARIO 2 = Node is Left-Balanced Heavy OR Left-Left Heavy
 * 1. right_rotate(v)
 *
 * SCENARIO 3 - Node is Right-Left Heavy
 * 1. right_rotate(v.right)
 * 2. left_rotate(v)
 *
 * SCENARIO 4 - Node is Right-Balanced Heavy OR Right-Right Heavy
 * 1. left_rotate(v)
 *
 */
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
/**
 *
 * @param x      Value of Node that we want to check AVLInvariant on
 * @return       Returns 0 if balanced
 *               Returns 1 if Left Heavy
 *               Returns 2 if Right Heavy
 */
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

    if (original_vertex_left_child_left_child != nullptr)
        weight1 = original_vertex_left_child_left_child -> weight;
    else
        weight1 = 0;

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

    if (original_vertex_right_child_right_child != nullptr)
        weight1 = original_vertex_right_child_right_child -> weight;
    else
        weight1 = 0;

    if (original_vertex_right_child_left_child != nullptr)          // Possible for it to be NULLPTR
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

/**
 *
 * @param x   Value of Node to be inserted
 *
 * // This code shares alot of similarities with BinarySearchTree, consult ADTBinarySearchTree for more info. We will just focus on AVL-Tree specific considerations.
 *
 * Key Considerations when inserting a node for AVL-Tree
 * 1. Must update the heights of ALL the PARENTS of the node that was just inserted
 * 2. Must rebalance the Tree if needed
 * 3. Must update the weights of ALL the PARENTS of the node that was just inserted
 *
 * Consideration 1 - How do we update the Heights?
 * - When a node is inserted, only the PARENTS of the removed node are affected. The CHILDREN are not affected.
 * - Hence, we just need to traverse up the 'Parent Chain', and update all the heights.
 *
 * Consideration 2 - How do we rebalance the Tree after insertion?
 * - Recall that for AVL Tree, the invariant is that abs( v.left(height) - v.right(height) ) <= 1
 * - Hence if the heights of the Tree is messed up after insertion, we must rebalance the AVL Tree
 * - Rebalance by walking up the 'Parent Chain', and rotating Nodes that are out of balance
 * - Rotations are done ON the OFFENDING node. (Note that maximum number of Rotations after INSERTIOn is 2)
 *
 * Consideration 3 - How do we update the Weights of Tree after insertion?
 * - Travel up the 'Parent Chain', adding 1 from the weight while doing so
 *
 */

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

/**
 *
 * @param x   Value of Node to be removed
 *
 * // This code shares alot of similarities with BinarySearchTree, consult ADTBinarySearchTree for more info. We will just focus on AVL-Tree specific considerations.
 *
 * Key Considerations when removing a node for AVL-Tree
 * 1. Must update the heights of ALL the PARENTS of the node that was just removed
 * 2. Must rebalance the Tree if needed
 * 3. Must update the weights of ALL the PARENTS of the node that was just removed
 *
 * Consideration 1 - How do we update the Heights?
 * - When a node is removed, only the PARENTS of the removed node are affected. The CHILDREN are not affected.
 * - Hence, we just need to traverse up the 'Parent Chain', and update all the heights.
 *
 * Consideration 2 - How do we rebalance the Tree after removal?
 * - Recall that for AVL Tree, the invariant is that abs( v.left(height) - v.right(height) ) <= 1
 * - Hence if the heights of the Tree is messed up after removal, we must rebalance the AVL Tree
 * - Rebalance by walking up the 'Parent Chain', and rotating Nodes that are out of balance
 * - Rotations are done ON the OFFENDING node. (Note that maximum number of rotations after Deletion is O(log n))
 *
 * Consideration 3 - How do we update the Weights of Tree after removal?
 * - Travel up the 'Parent Chain', subtracting 1 from the weight while doing so
 *
 */
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

/**
 * @param node    Pointer to Node that has just been inserted/deleted
 *
 * This function will balance the weight of the AVl Tree, after a node has been removed from the AVL Tree.
 * We will travel up the 'Parent Chain' from the Node, and update all the heights along the way by subtracting 1.
 */

template <class T>
void AVLTree <T> :: BalanceWeight(AVLNode <T> *node)
{
    while (node != nullptr)
    {
        node -> weight -= 1;
        node = node -> parent;
    }
}

/**
 * @param traversal   Pointer to Node that has just been inserted/deleted
 *
 * This function is used to update the Heights, when we use an Insertion/Deletion operation on the AVL Tree.
 * We will travel up the 'Parent Chain' from the Node, and update all the heights along the way.
 */
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
void AVLTree <T> :: print_weight(T vertex)
{
    AVLNode<T> *node = search(vertex);
    std::cout << node -> weight << " ";
}
