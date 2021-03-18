#include <iostream>
#include <algorithm>

// Forward Declaration of Template that will be used as Member in other Templates
template <class T>
class TreeNode;

template <class T>
class BinarySearchTree
{
private:
    TreeNode <T> *root;

public:
    BinarySearchTree() {root = nullptr;}

    // ADT Interface Functions
    TreeNode <T>* find_min (TreeNode <T> *current);
    TreeNode <T>* find_max (TreeNode <T> *current);
    TreeNode <T>* successor (T x);
    TreeNode <T>* predecessor (T x);
    int height(TreeNode <T> *node);
    TreeNode <T>* search (T x);
    void print_item (TreeNode <T> *node);

    void insert (T x);
    void remove (T x);

    void inorder (TreeNode <T> *node);
    void preorder (TreeNode <T> *node);
    void postorder (TreeNode <T> *node);
    void _levelorder();
    void levelorder (TreeNode <T> *node);
    void node_on_level (TreeNode <T> *node, int level);

    friend TreeNode <T>;
};

template <class T>
class TreeNode
{
private:
    T item;
    TreeNode <T> *left;
    TreeNode <T> *right;
    TreeNode <T> *parent;

public:
    TreeNode (T x) {parent = nullptr; left = nullptr; right = nullptr; item = x;}

    friend  BinarySearchTree <T>;
};


int main () {
    BinarySearchTree<int> BST;
    BST.insert(10);
    BST.insert(42);
    BST.insert(36);
    BST.insert(32);
    BST.insert(40);
    BST.insert(98);
    BST.insert(57);

    BST.remove(36);
    BST.print_item( BST.predecessor(40));
    BST.print_item( BST.successor(40));
}


/**
 * @param[in] x      Node Value to search for in the BST
 * @return           Pointer to the Node that was searched for
 *
 * Using a Binary Search-esque Algorithm, this function will attempt to find if a node of value x exists in the tree.
 * If no such node of value x can be found, a nullptr is returned.
 */
template <class T>
TreeNode <T>* BinarySearchTree <T> :: search (T x)
{
    TreeNode <T> *current = root;

    while (current != nullptr)
    {
        if (current -> item == x)       // Target found
            return current;

        else if (current -> item < x)   // If Target larger than Current
            current = current -> right; // Search Right Half of Sub-Tree

        else
            current = current -> left;  // Search Left Half of Sub-Tree
    }

    // If Target cannot be found, return NULL
    return nullptr;
}

/**
 *
 * @param[in] x     Inserts a node of value x
 *
 * This function attempts to insert a node of value x into the BST.
 *
 * 1. We initialize a "traversal" pointer, starting at the Root of the BST.
 *
 * 2. First we check if we should traverse Left (is value of x smaller than current Node that "traversal" is at"?
 * 2.1 If we should traverse Left, we need to do 2 further checks depending on if Left Branch has any other Nodes on them
 * 2.1.1 If yes, we must further traverse down the Left Branch, to check these values first.
 * 2.1.2 If no, then we can just insert the new Node, with value x, here.
 *
 * 3. If didn't need to traverse Left, the ONLY POSSIBLE ALTERNATIVE is to traverse Right (because no duplicate values allowed!)
 * // Similar Logic as traverse Left
 *
 *
 * Several Key Conditions to take note of
 * 1. Are we inserting into an Empty BST?
 *   - If yes, we must attach the root pointer to this new node, and simply return
 *
 * 2. Are we inserting a duplicate value into the BST?
 *   - Depending on Implementation, we might want to reject duplicate node values, or allow for it.
 */

template <class T>
void BinarySearchTree <T> :: insert (T x)
{
    // if Inserting into EMPTY BST
    if (root == nullptr)
    {
        root = new TreeNode<T>(x);
        return;
    }

    TreeNode<T> *current = root;

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
                current -> left = new TreeNode <T> (x);
                (current -> left) -> parent = current;
                return;
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
                current -> right = new TreeNode <T> (x);
                (current -> right) -> parent = current;
                return;
            }
        }
    }
}

/**
 * @pre          The node of value x exists in the BST
 * @param[in] x  Removes a node of value x
 *
 * This function attempts to remove a node of value x from the BST.
 *
 * We initialize a 'traversal' pointer, starting at the root of the BST.
 * The trivial task is to find WHERE the node x is, before we can remove it. We assume that we can do it already.
 *
 * At this point, we have found that traversal -> item == x, and we are now ready to remove. There are THREE scenarios
 *
 * 1.    We are currently at a Leaf Node (no children). This means that height(current node) == 0
 * 1.1   Before we can delete the Leaf Node, we must first ensure that the Pointer of the PARENT of this Leaf Node, that points to this Leaf Node, is set to NULL.
 * 1.2   Once this is done, we can simply delete current.
 *
 * 2.  We are currently at a Node with ONE Child only.
 *     This happens if the Left Branch has a path, but the Right Branch is NULL, or
 *                    the Right Branch has a path, but the Left Branch is NULL
 *
 *    We simply need to link the Node's Parent with the Node's Child, then delete the Node.
 *
 * 2.1    If there is a Left Branch Path but no Right Branch path, then.....
 * 2.1.1  Link Current's Child to Current's Parent
 * 2.1.2  Link Current's Parent to Current's Child (dependent on position of Current relative to Parent!)
 * EDGE CASE --  Current's Parent is NULL. In this case, Current's Child will be the new "head" of the BST, hence we just need to set Current's Child's Parent pointer to be NULL
 *
 * 2.2 If there is a Right Branch Path but no Left Branch path, then.....
 * // Similar logic to 2.1
 *
 *
 * 3    We are currently at a node with TWO CHILD only.
 * 3.1  Store Successor Item of Current into variable temp
 * 3.2  Delete the Successor Node of Current
 * 3.3  Overwrite Current Item with temp (value of Current Successor)
 */

template <class T>
void BinarySearchTree <T> :: remove (T x)
{
    TreeNode <T> *current = root;

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

                delete (current);
                return;
            }

            // if Current has ONE CHILD ONLY
            else if ( ( current -> left != nullptr && current -> right == nullptr )
                       || ( current -> left == nullptr && current -> right != nullptr) )
            {

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
                return;
            }

            else // Current must have TWO CHILDREN ONLY
            {
                T temp = ( successor(current->item) ) -> item;    // Storing Successor ITEM of Current in Temp
                remove ( (successor(current->item))->item);   // Remove Successor NODE
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
 * @pre Node with value x exists in the BST
 *
 * @param[in] x  Value of Node to find successor of
 * @return       Pointer to Node that is successor of Node w value x
 *
 * This function attempts to find the Successor for a Node of value x.
 *
 * We initialize a 'traversal' pointer that points to Node we want to find successor of.
 * There will be TWO scenarios.
 *
 * 1. If there is a RIGHT Branch, we find the MINIMUM of the RIGHT SUBTREE (Smallest of the Largest)
 *
 * 2. Else if there is NO RIGHT BRANCH, we must traverse up the "Parent Chain" of the Node we are currently at, to find the Successor.
 *    - Ancestor cannot be nullptr (we must stop at Root of BST in worst case scenario)
 *    - Descendant must always be the RIGHT child of the Ancestor (since Descendant is strictly larger than Ancestor, for finding Successor)
 *    We will keep travelling up the Parent Chain, until the FIRST RIGHT TURN WE CAN MAKE. AFTER turning right for the first time, that node will be our successor
 */

template <class T>
TreeNode <T>* BinarySearchTree <T> :: successor (T x)
{
   TreeNode <T> *current = search (x);   // Get Pointer to Node X

   if ( current -> right != nullptr)        // If there is a RIGHT BRANCH
       return find_min(current -> right);   // Find the MINIMUM of RIGHT SUBTREE (Smallest of the Largest)

   else    // there is NO RIGHT BRANCH
   {
       TreeNode <T> *ancestor = current -> parent;
       TreeNode <T> *descendant = current;

       while (ancestor != nullptr && descendant == ancestor -> right)
       {
           descendant = ancestor;
           ancestor = descendant -> parent;
       }

       if (ancestor == nullptr)   // If x has NO successor (it is the largest in BST)
       {
           return nullptr;
       }

       else
           return ancestor;
   }
}


/**
 * @pre            Node with value x exists in the BST
 * @param[in] x    Value of Node to find Predecessor of
 * @return         Pointer to Node that is Predecessor of Node w value x
 *
 * This function attempts to find the Predecessor for a node w value x.
 * // Basically Reverse Logic of Successor
 *
 * We initialize a 'traversal' pointer that points to Node that we want to find predecessor of.
 * There will be TWO scenarios.
 *
 * 1. If there is a LEFT BRANCH, we find the MAXIMUM OF THE LEFT SUBTREE (Largest of the Smallest)
 *
 * 2. Else there is no LEFT BRANCH, we must traverse up the "Parent Chain" of the node we are currently at, to find the Predecessor.
 *    - Ancestor cannot be nullptr
 *    - Descendant must ALWAYS be the LEFT child of the Ancestor (Since Descendant is strictly smaller than Ancestor, for finding Predecessor)
 *    We will keep travelling up the Parent Chain, until the FIRST LEFT TURN WE CAN MAKE. AFTER taking the LEFT TURN, that node will be the Predecessor.
 */

template <class T>
TreeNode <T> *BinarySearchTree <T> :: predecessor (T x)
{
    TreeNode <T> *current = search (x);   // Get Pointer to Node X

    if ( current -> left != nullptr)      // If there is a LEFT BRANCH
    {
        return find_max(current -> left); // Find the MAXIMUM of LEFT SUBTREE (Largest of the Smallest)
    }

    else   // There is NO Left Branch
    {
        TreeNode <T> *ancestor = current -> parent;
        TreeNode <T> *descendant = current;

        while (ancestor != nullptr && descendant == ancestor -> left)
        {
            descendant = ancestor;
            ancestor = descendant -> parent;
        }

        if (ancestor == nullptr)    // If X has NO predecessor (it is the smallest in BST)
        {
            return nullptr;
        }

        else
            return ancestor;
    }
}


template <class T>
TreeNode <T>* BinarySearchTree <T> :: find_min (TreeNode <T> *current)
{
   while (current -> left != nullptr)
   {
       current = current -> left;
   }

   return current;
}

template <class T>
TreeNode <T>* BinarySearchTree <T> :: find_max (TreeNode <T> *current)
{
    while (current -> right != nullptr)
    {
        current = current -> right;
    }

    return current;
}

template <class T>
void BinarySearchTree <T> :: print_item (TreeNode <T> *node)
{
    if (node == nullptr)
    {
        std::cout << "Error printing Node";
        return;
    }

    std::cout << node -> item << " ";
}

/**
 *
 * @param[in] node   Pointer to Node that we want to find Height of
 * @return           Integer value of Height
 *
 * Height of Node is defined as the number of nodes along the LONGEST PATH, from NODE to Leaf Node
 * For a TreeNode v,
 * h(v) = 0, if v is a leaf node
 * h(v) = max( h(left_subtree_height) + h(right_subtree_height) ) + 1
 * h(null) = -1
 */

template <class T>
int BinarySearchTree <T> :: height (TreeNode <T> *node)
{
    int left_subtree_height = -1;
    int right_subtree_height = -1;

    if (node -> left != nullptr)
        left_subtree_height = height (node -> left);

    if (node -> right != nullptr)
        right_subtree_height = height (node -> right);

    return ( std::max(left_subtree_height, right_subtree_height) + 1 );

}

template <class T>
void BinarySearchTree <T> :: inorder( TreeNode <T> *node)
{
    if (node == nullptr)
        return;

    inorder (node -> left);
    std::cout << node -> item << " " ;
    inorder (node -> right);
}

template <class T>
void BinarySearchTree <T> :: preorder( TreeNode <T> *node)
{
    if (node == nullptr)
        return;

    std::cout << node -> item << " " ;
    preorder( node -> left);
    preorder(node -> right);
}

template <class T>
void BinarySearchTree <T> :: postorder( TreeNode <T> *node)
{
    if (node == nullptr)
        return;

    postorder( node -> left);
    postorder(node -> right);

    std::cout << node -> item << " " ;
}

template <class T>
void BinarySearchTree <T> :: _levelorder()
{
    levelorder(root);
}

template <class T>
void BinarySearchTree <T> :: levelorder(TreeNode <T> *node)
{
    if (node == nullptr)
        return;

    int total_number_of_levels = height(root);

    for (int i = 0; i <= total_number_of_levels; i ++)
    {
        node_on_level(root, i);   // Print ALL the nodes on level i, with reference to root
    }
}

template <class T>
void BinarySearchTree <T> :: node_on_level (TreeNode <T> *node, int level)
{
    // This condition must be checked first, because it is possible to be level == 0 && node == nullptr
    if (node == nullptr)
        return;

    if (level == 0)
    {
        std::cout << node -> item << " ";
        return;
    }

    // Reducing to Base Case
    if (level > 0)
    {
        node_on_level(node -> left, level-1);
        node_on_level(node -> right, level-1);
    }

}