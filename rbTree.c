#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/queue.h> 
#include "rbTree.h"


/* implementation of red black trees

	// DEFINITION of red-black trees:

		1. Every node is either red or black. (denoted by true and false in this implementation)
		2. The root and the leaves are black. (leaves can be an external null node to satisfy this property)
		3. The parent of every red node has to be black.
		4. All simple paths (direct path from a node to a leaf) from a node x to a decendant leaf have the same number of black nodes (black height).
	//

	//	DEFINE black height of a node to be the number of black nodes in any simple path from the node to a leaf( not counting the initial node itself).

	// Ensuring that the above properties are preserved guarantees that the tree remains balanced no matter what we insert.
	   However, this is not easy and insert and delete have to be changed and augmented to preserve the rb properties. 

	// Insertion:
		When inserting a new node we must perserve the rb properties, we do this with the following operations:
			1.Color changes
			2.Restructure the tree. This is done by updating the pointers of the tree using rotations 
			  Rotation example: (This preserves the properties of the BST, every nodes left child is still smaller than it and every right child is larger)

					A      left rotate      B  
				   / \    ---------->      / \
				  B   C                   x   A
				 / \       right rotate      / \
                x   y     <-----------      y   C
              This is a constant time operation.   
				
        The actual insert operation involves inserting a new node exactly like we would in a normal BST. We color this
        node red, which may lead to problems since every red node must have a black parent. To solve this problem we move the 
        violation up the tree via recoloring untill we can fix the problem via restructuring. 
        
*/

void left_rotate (struct tree * tree, struct node * current)
{
	printf("left_rotate start\n");
	// current is the node we want to rotate left
	// new_root is its old right child that will be rotated to currents old position

	struct node *new_root = (struct node*) malloc(sizeof(struct node));

	if ( current->right == tree->leaf){
		printf("attempting to rotate a leaf... exiting\n");
		return;
	}

 	new_root = current->right;            // new root is final root of the subtree we will rotate
	current->right = new_root->left;      // make the new roots left subtree the old roots right subtree


	if ( current->right != tree->leaf)          // the new right subtree is not null update its parent
		current->right->parent = current;
	
	new_root->parent = current->parent;   // give the new root the old roots parent


	if ( current->parent == tree->leaf){
		printf("handling root\n");
		exit(1);
	}

	if ( current == current->parent->left)    // if current was a left child
		current->parent->left = new_root;	  // set current's parent's left child to the new root

	if ( current == current->parent->right)   // if current was a right child
		current->parent->right = new_root;	  // set current's parent's right child to the new root

	new_root->left = current;
	current->parent = new_root;			  // update the old root's parent to be the new root

	printf("left_rotate done\n");
}

void right_rotate (struct tree * tree, struct node * current)
{
	printf("right_rotate start\n");
	// current is the node we want to rotate left
	// new_root is its old left child that will be rotated to currents old position

	struct node *new_root = (struct node*) malloc(sizeof(struct node));

	if ( current->left == tree->leaf){
		printf("attempting to rotate a leaf... exiting\n");
		return;
	}

 	new_root = current->left;            // new root is final root of the subtree we will rotate
	current->left = new_root->right;      // make the new roots right subtree the old roots left subtree


	if ( current->left != tree->leaf)          // the new left subtree is not null update its parent
		current->left->parent = current;
	
	new_root->parent = current->parent;   // give the new root the old roots parent


	if ( current->parent == tree->leaf){
		printf("handling root\n");
		exit(1);
	}
	//end TODO
	
	if ( current == current->parent->left)    // if current was a left child
		current->parent->left = new_root;	  // set current's parent's left child to the new root

	if ( current == current->parent->right)   // if current was a right child
		current->parent->right = new_root;	  // set current's parent's right child to the new root

	new_root->right = current;
	current->parent = new_root;			  // update the old root's parent to be the new root

	printf("right_rotate done\n");
}

void insert_fixup (struct tree * tree, struct node * inserted)
{

	printf("fixing up after insert\n");
	struct node * z = inserted;
	struct node * y;

	while(z->parent->red == true){                   //while z.parent is red
		printf("Parent is red\n");
		if(z->parent == z->parent->parent->left){       // if z.parent is a left child
			printf("parent is a left child\n");
		y = z->parent->parent->right;                   // let y equal z.parents's right brother
		
		if(y->red == true){                             // if y is red                         (CASE 1)
			z->parent->red = false;                       // color z.parent black
			y->red = false;                               // color y black
			z->parent->parent->red = true;                // color z.parent.parent red
			z = z->parent->parent;                        // let z = z.parent.parent            (END CASE 1)
		}else {
			if (z == z->parent->right){                // else if z is a right child (perform a rotation next to switch it to case 3)
				z = z->parent;                                   //let z = z.parents            (CASE2)
				left_rotate(tree,z);                             //left rotate z		        (END CASE 2)
			}
			z->parent->red = false;                            // color z.parent black 		    (CASE 3)
			z->parent->parent->red = true;                     // color z.parent.parent red 
			right_rotate(tree,z->parent->parent);              // right rotate z.parent.parent  (END CASE 3)
		}
	
	}else{                                        // else => z.parent is a right child
     		                                      // same as if but reflected ( switch right and left) 
		
		y = z->parent->parent->left;              // let y equal z.parents's right brother
		
		if(y->red == true){                            // if y is red                            (CASE 1)
			z->parent->red = false;                      // color z.parent black
			y->red = false;                              //color y black
			z->parent->parent->red = true;               //color z.parent.parent red
			z = z->parent->parent;                       // let z = z.parent.parent              (END CASE 1)
		}else{
			 if (z == z->parent->left){                // else if z is a right child perform a rotation to switch it to case 3
				z = z->parent;                                   //let z = z.parents             (CASE2)
				right_rotate(tree,z);                            //left rotate z				 (END CASE 2)
			}
			z->parent->red = false;                           // color z.parent black 		     (CASE 3)
			z->parent->parent->red = true;                    // color z.parent.parent red 
			left_rotate(tree,z->parent->parent);			  // left rotate z.parent.parent     (END CASE 3)
		}
	}
}

	tree->root->left->red = false; // color root black
}

void insert (struct tree * tree, struct node *to_insert)
{
	bool done = false;
	to_insert->left = to_insert->right = tree->leaf; // set the new nodes leaves to the null leaf

	tree->root->key = to_insert->key; // set the dummy roots key to the new value so that insert always goes left of the root;
	struct node * current = tree->root;
	while(!done)
	{
		printf("checking node: %d against to_insert node %d and ::",current->key, to_insert->key);
		if(current->key >= to_insert->key){
			printf("Going left\n");
			if ( current->left != tree->leaf)
				current = current->left;
			else{
				printf("current.left is empty... putting new node in\n");
				done = true;
				current->left = to_insert;
				to_insert->parent = current;
			}
		}else{
			printf("Going right\n");
			if(current->right != tree->leaf){
				current = current->right;
			}
			else{
				printf("current.right is empty... putting new node in\n");
				done = true;
				current->right = to_insert;	
				to_insert->parent = current;

			}
		}
	}

	to_insert-> red = true;
	insert_fixup(tree, to_insert);
	return;
} 

void addNode(struct tree * tree, int n)
{
	struct node *to_add = (struct node*) malloc(sizeof(struct node));

//	printf("Creating new node: ");
	to_add->key = n;
	to_add->right = tree->leaf;
	to_add->left = tree->leaf;
	to_add->parent = NULL;

	// = {n,NULL,NULL};
//	printf("%d\n", to_add->key );
//	printf("inserting\n");
	insert(tree, to_add);
}

void tree_init(struct tree * tree){

	tree->leaf = (struct node*) malloc(sizeof(struct node));
	tree->leaf->left= tree->leaf->right = tree->leaf;
	tree->leaf->red = false;
	tree->leaf->key = -1;

	tree->root = (struct node*) malloc(sizeof(struct node));
	tree->root->left = 	tree->root->right = tree->leaf;
	tree->root->red = false;
	tree->root->key = -11;



	printf("done with init\n");
}

int height(struct tree * tree, struct node * n)
{
	if ( n == tree->leaf)
		return 0;

	int left = height(tree, n->left);
	int right = height(tree ,n->right);

	if ( left > right)
		return left+1;
	else return right +1;
}
void print_inorder (struct tree * tree ,struct node * current)
{
	if (current->left != tree->leaf)
		print_inorder(tree, current->left);
	
	printf ("%d\n",current->key);

	if (current->right != tree->leaf)
		print_inorder(tree, current->right);

}
bool red_test(struct tree * tree, struct node * n)
{
	if ( n == tree->leaf)
		return true;

	bool left = n->left->red;
	bool right = n->right->red;
	if (n->red && n->left->red)
		return false;
	if (n->red && n->right->red)
		return false;
	
	return ((red_test(tree, n->left)) && red_test(tree, n->right));
}

int black_height(struct tree * tree, struct node * n, bool test)
{


	int left;  
	if (n->left == tree->leaf) //covers the case of a black leaf or a red leaf with a black sentinel
		return 1;
	else left = black_height(tree, n->left,test);

	int right;  
	if (n->right == tree->leaf) // same as above
		return 1;
	else right = black_height(tree, n->left,test);

	if(right != left)
		test = false;
	else 
		if (n->red == false)
			return right+1;
		else return right;
	}

	bool test (struct tree * tree)
	{

		bool equal_black_height =true;
		equal_black_height =  black_height(tree,tree->root->left,equal_black_height);

		return (red_test(tree,tree->root->left) && equal_black_height);
	}
	int main(){

		struct tree * tree = (struct tree*) malloc(sizeof(struct tree));
		tree_init(tree);
		printf("%d\n",tree->leaf->key );

		struct node root = { 2, NULL, NULL,NULL, NULL};
		struct node test4 = {3,NULL,NULL, NULL,NULL};
		struct node test2 = {8,NULL,NULL, NULL,NULL};

		struct node test3 = {9,NULL,NULL, NULL,NULL};
		insert (tree, &test3);

		insert ( tree, &test2);


		insert ( tree, &test4);

		insert (tree , &root);

		printf("/////\n");
		print_inorder(tree, tree->root->left);
//	print_preorder(root);
//printf("////%d \n", height(&root));
/////////////////////
		int i = 0;
		while( i >= 0)
		{
			printf(" Next Element= ");
			scanf("%d",&i);
			addNode(tree, i);
		}
		printf("\n");




		print_inorder(tree,tree->root->left);
		printf("%d\n",height(tree,tree->root->left));
		if (test(tree))
			printf("good\n");
		return 0;

	}