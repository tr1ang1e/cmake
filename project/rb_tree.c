#include "common.h"
#include <string.h>

typedef enum SUBTREE_E
{
    NONE,
    LEFT,
    RIGHT,
} SUBTREE;

typedef struct FoundInfoS
{
    RBTreeNode* pParent;
    RBTreeNode* pNode;
    SUBTREE subtree;
} FoundInfo;

static RBTreeNode NIL = { NULL, BLACK, NULL, NULL, NULL };    // common node for all of empty leafs


// static
static void rb_tree_actualize_root(RBTreeNode** pRoot);
static bool is_key_valid(const char* key);
static FoundInfo rb_tree_find_node(RBTreeNode* pRoot, const char* key);
static RBTreeNode* rb_tree_create_node(RegFieldInfo* pItem);
static void rb_tree_free_memory(RBTreeNode* pNode);
static void rb_tree_rot_left(RBTreeNode* pNode);
static void rb_tree_rot_right(RBTreeNode* pNode);
static bool is_balance_broken(RBTreeNode* pNode);
static void rb_tree_insert_balance(RBTreeNode* pNode);


// not debugged yet
static void rb_tree_delete_black_node(FoundInfo info);
static void rb_tree_delete_balance(FoundInfo info);


/************************************************************************
 *                             COMMON    	                            *
 ************************************************************************/

void rb_tree_init(RBTreeNode** pRoot)
{
    *pRoot = NULL;
}

bool rb_tree_destroy(RBTreeNode** pRoot)
{
    bool result = false;

    do
    {
        if (*pRoot == NULL)
        {
            break;
        }

        rb_tree_free_memory(*pRoot);
        *pRoot = NULL;

        result = true;

    } while (0);

    return false;
}

bool rb_tree_insert(RBTreeNode** pRoot, RegFieldInfo* pField)
{
    /* insertion place:
     *   always to the NIL
     *   exception: red-black tree is empty
     */

    bool result = false;

    do
    { 
        if (pField == NULL)
        {
            break;
        }

        if (!is_key_valid(pField->name))
        {
            break;
        }

        // search place to insert to
        FoundInfo info = rb_tree_find_node(*pRoot, pField->name);

        if (info.pNode != NULL && info.pNode != &NIL)    // node with such key already exists
        {
            break;    // different logic might be implemented
        }

        RBTreeNode* pNode = rb_tree_create_node(pField);

        if (pNode == NULL)    // error while creating node to insert
        {
            break;
        }

        if (*pRoot == NULL)    // insertion to root
        {
            pNode->color = BLACK;
            *pRoot = pNode;
        }
        else
        {
            // insertion
            pNode->parent = info.pParent;
            if (info.subtree == LEFT)
            {
                info.pParent->left = pNode;
            }
            else
            {
                info.pParent->right = pNode;
            }

            // balansing
            rb_tree_insert_balance(pNode);
            rb_tree_actualize_root(pRoot);
        }

        result = true;

    } while (0);

    return result;
}

bool rb_tree_get(RBTreeNode* pRoot, const char* name, RegFieldInfo* pField)    // [in] pRoot = source rbtree, [in] name = unique key, [out] pItem = destination struct
{
    int result = false;

    do
    {
        if (pField == NULL)
        {
            break;
        }

        if (!is_key_valid(name))
        {
            break;
        }

        FoundInfo info = rb_tree_find_node(pRoot, name);

        if (info.pNode == NULL)    // tree is empty
        {
            break;
        }

        if (info.pNode == &NIL)    // node was not found
        {
            break;
        }

        memcpy(pField, info.pNode->data, sizeof(RegFieldInfo));

        result = true;

    } while (0);

    return result;
}

bool is_NIL_same(RBTreeNode* pNIL)
{
    return pNIL == &NIL;
}


/************************************************************************
 *                             STATIC    	                            *
 ************************************************************************/


void rb_tree_actualize_root(RBTreeNode** pRoot)
{
    while ((*pRoot)->parent != NULL)
    {
        *pRoot = (*pRoot)->parent;
    }

    (*pRoot)->color = BLACK;
}

bool is_key_valid(const char* key)
{
    bool result = true;

    size_t keyLen = strnlen(key, REG_FIELD_NAME_LEN);

    if (keyLen > REG_FIELD_NAME_LEN - 1)    // for '\0'
    {
        result = false;
    }

    return result;
}

FoundInfo rb_tree_find_node(RBTreeNode* pRoot, const char* key)
{
    /*           variant                 description
     *
     * >>  { NULL, NULL, NONE }    >>   tree is empty
     * >>  { NULL, ROOT, NONE }    >>   found : node is root
     * >>  { ....  ....  .... }    >>   found : node is not root
     * >>  { ...., &NIL, .... }    >>   not found
     *
     */

    FoundInfo info = { NULL, NULL, NONE };

    do
    {
        if (pRoot == NULL)
        {
            break;
        }

        info.pNode = pRoot;

        while (info.pNode != &NIL)
        {
            int result = strcmp(info.pNode->data->name, key);

            if (result > 0)    // go left
            {
                info.pParent = info.pNode;
                info.pNode = info.pNode->left;
                info.subtree = LEFT;
            }
            else if (result < 0)    // go right
            {
                info.pParent = info.pNode;
                info.pNode = info.pNode->right;
                info.subtree = RIGHT;
            }
            else    // found , info is in actual state
            {
                break;
            }
        }

    } while (0);

    return info;
}

RBTreeNode* rb_tree_create_node(RegFieldInfo* pItem)
{
    RBTreeNode* pNode = NULL;

    do
    {
        if (pItem == NULL)
        {
            break;
        }

        // no need to check if key is valid
        // it was already checked in caller = rb_tree_insert()

        // allocate memory for node
        pNode = (RBTreeNode*)malloc(sizeof(RBTreeNode));
        if (pNode == NULL)
        {
            break;
        }

        // data
        pNode->data = pItem;

        // default settings
        pNode->color = RED;
        pNode->left = &NIL;
        pNode->right = &NIL;
        pNode->parent = NULL;

    } while (0);

    return pNode;
}

void rb_tree_free_memory(RBTreeNode* pNode)
{
    // depth-first = left sub-tree
    if (pNode->left != &NIL)
    {
        rb_tree_free_memory(pNode->left);
    }

    // depth-first = right sub-tree
    if (pNode->right != &NIL)
    {
        rb_tree_free_memory(pNode->right);
    }

    // when both left and right children are NIL
    free(pNode);
}

void rb_tree_rot_left(RBTreeNode* pNode)
{
    /* state before left rotation
    *
    *      Parent (don't know if Node == Parent.left or Parent.right)
    *        |
    *       Node
    *      /   \
    *    A      Temp
    *          /   \
    *         B     C
    */

    RBTreeNode* pTemp = pNode->right;
    RBTreeNode* pParent = pNode->parent;

    // Node and B
    pNode->parent = pTemp;
    pNode->right = pTemp->left;
    if (pNode->right != &NIL)
    {
        pNode->right->parent = pNode;
    }


    // Temp
    pTemp->parent = pParent;
    pTemp->left = pNode;

    // Parent
    if (pParent != NULL)
    {
        if (pParent->left == pNode)
        {
            pParent->left = pTemp;
        }
        else
        {
            pParent->right = pTemp;
        }
    }
}

void rb_tree_rot_right(RBTreeNode* pNode)
{
    /* state before right rotation
    *
    *           Parent (don't know if Node == Parent.left or Parent.right)
    *             |
    *            Node
    *           /   \
    *       Temp     A
    *      /    \
    *     C      B
    */

    RBTreeNode* pTemp = pNode->left;
    RBTreeNode* pParent = pNode->parent;

    // Node and B
    pNode->parent = pTemp;
    pNode->left = pTemp->right;
    if (pNode->right != &NIL)
    {
        pNode->left->parent = pNode;
    }

    // Temp
    pTemp->parent = pParent;
    pTemp->right = pNode;

    // Parent
    if (pParent != NULL)
    {
        if (pParent->left == pNode)
        {
            pParent->left = pTemp;
        }
        else
        {
            pParent->right = pTemp;
        }
    }
}

bool is_balance_broken(RBTreeNode* pNode)
{

    if (pNode == NULL)
    {
        return false;
    }

    if (pNode->parent != NULL)
    {
        if (pNode->parent->color == BLACK)
        {
            return false;
        }

        if (pNode->parent->left->color == BLACK && pNode->parent->right->color == BLACK)
        {
            return false;
        }
    }

    return true;
}

void rb_tree_insert_balance(RBTreeNode* pNode)
{
    /* possible state after insertion (X == pNode)
     *
     *  v1       A           v2        A
     *         /   \                 /   \
     *        B     C               B     C
     *         \                   /
     *          X                 X
     *
     *
     *  v3       A           v4        A
     *         /   \                 /   \
     *        B     C               B     C
     *             /                       \
     *            X                         X
     */

    while (pNode != NULL && pNode->color == RED && pNode->parent != NULL && pNode->parent->color == RED)
        // while (is_balance_broken(pNode))
    {
        // v1, v2: X on the LEFT of A
        if (pNode->parent == pNode->parent->parent->left)
        {
            // C is RED
            if (pNode->parent->parent->right->color == RED)
            {
                pNode->parent->color = BLACK;                   // B
                pNode->parent->parent->color = RED;             // A
                pNode->parent->parent->right->color = BLACK;    // C

                pNode = pNode->parent->parent;    // A is next we deal with
            }
            // C is BLACK = impossible to be immidiately after insertion,
            // exception: C is NIL
            else
            {
                // v1: X on the RIGHT of B
                if (pNode == pNode->parent->right)
                {
                    rb_tree_rot_left(pNode->parent);
                    pNode = pNode->left;  // to make following code universal
                }

                pNode->parent->color = BLACK;
                pNode->parent->parent->color = RED;
                rb_tree_rot_right(pNode->parent->parent);

                pNode = pNode->parent;
            }
        }
        // v3, v4: X on the RIGHT of A
        else
        {
            // B is RED
            if (pNode->parent->parent->left->color == RED)
            {
                pNode->parent->color = BLACK;                  // C
                pNode->parent->parent->color = RED;            // A
                pNode->parent->parent->left->color = BLACK;    // B

                pNode = pNode->parent->parent;    // A is next we deal with
            }
            // B is BLACK = impossible to be immidiately after insertion
            // exception: B is NIL
            else
            {
                // v3: X on the LEFT of C
                if (pNode == pNode->parent->left)
                {
                    rb_tree_rot_right(pNode->parent);
                    pNode = pNode->right;  // to make following code universal
                }

                // [ backup : 16.02.2022 ]
                // pNode->color = BLACK;
                // pNode->parent->color = RED;
                // rb_tree_rot_left(pNode->parent->parent);

                pNode->parent->color = BLACK;
                pNode->parent->parent->color = RED;
                rb_tree_rot_left(pNode->parent->parent);

                pNode = pNode->parent;
            }
        }
    }
}