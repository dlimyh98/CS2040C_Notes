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
    BinarySearchTree()
    {
        root = nullptr;
    }

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
    TreeNode (T x)
    {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        item = x;
    }

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
    BST.insert(61);
    BST.insert(60);
    BST.insert(76);
    BST.insert(69);

    BST.remove(36);
    BST.print_item( BST.predecessor(40));
    BST.print_item( BST.successor(40));
}

template <class T>
TreeNode <T>* BinarySearchTree <T> :: search (T x)
{
    TreeNode <T> *current = root;

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
