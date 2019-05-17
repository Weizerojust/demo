

#include <iostream>
#include "AvlTree.h"

using namespace std;

AvlTree::AvlTree()
    : BinarySearchTree()
{
}

AvlTree::~AvlTree()
{
}

int AvlTree::height(const BinaryNode *ptr) const
{
    return ptr == nullptr ? -1 : ptr->height;
}

void AvlTree::insert(const long data, BinaryNode* &ptr)
{
    BinarySearchTree::insert(data, ptr);
    rebalance(ptr);
}

void AvlTree::remove(const long data, BinaryNode* &ptr)
{
    BinarySearchTree::remove(data, ptr);
    rebalance(ptr);
}

BinaryNode *AvlTree::rebalance(BinaryNode* &ptr)
{




	if (ptr == nullptr) return ptr;

	    
	    if (height(ptr->left) - height(ptr->right) > 1)
	    {
	        if (height(ptr->left->left) >= height(ptr->left->right))
	        {
	            ptr = singleRightRotation(ptr);
	            cout << "    --- Single right rotation at node "
	                 << ptr->data << endl;
	        }
	        else
	        {
	            ptr = doubleLeftRightRotation(ptr);
	            cout << "    --- Double left-right rotation at node "
	                 << ptr->data << endl;
	        }
	    }

	    
	    else if (height(ptr->right) - height(ptr->left) > 1)
	    {
	        if (height(ptr->right->right) >= height(ptr->right->left))
	        {
	            ptr = singleLeftRotation(ptr);
	            cout << "    --- Single left rotation at node "
	                 << ptr->data << endl;
	        }
	        else
	        {
	            ptr = doubleRightLeftRotation(ptr);
	            cout << "    --- Double right-left rotation at node "
	                 << ptr->data << endl;
	        }
	    }

	    
	    ptr->height = (max(height(ptr->left),
	                       height(ptr->right)) + 1);

	    if (checkBalance(ptr) < 0)
	    {
	        cout << endl << "***** Tree unbalanced!" << endl;
	    }

	    return ptr;
}



BinaryNode *AvlTree::singleRightRotation(BinaryNode *k2)
{
		BinaryNode *temp;
		temp = k2 -> left;
		k2 -> left = temp -> right;
		temp -> right = k2;
		return temp;
}


BinaryNode *AvlTree::doubleLeftRightRotation(BinaryNode *k3)
{
		BinaryNode *temp;
		temp = k3 -> left;
		k3 -> left = singleLeftRotation(temp);
		return singleRightRotation(k3);
}


BinaryNode *AvlTree::doubleRightLeftRotation(BinaryNode *k1)
{
		BinaryNode *temp;
		temp = k1 -> right;
		k1 -> right = singleRightRotation(temp);
		return singleLeftRotation(k1);
}


BinaryNode *AvlTree::singleLeftRotation(BinaryNode *k1)
{
		BinaryNode *temp;
		temp = k1 -> right;
		k1 -> right = temp ->left;
		temp -> left = k1;
		return temp;
}

int AvlTree::checkBalance(BinaryNode *ptr)
{
    if (ptr == nullptr) return -1;

    int leftHeight  = checkBalance(ptr->left);
    int rightHeight = checkBalance(ptr->right);

    if ((abs(height(ptr->left) - height(ptr->right)) > 1)
        || (height(ptr->left)  != leftHeight)
        || (height(ptr->right) != rightHeight))
    {
        return -2;       
    }

    return height(ptr);  
}
