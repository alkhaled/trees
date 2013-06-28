
struct node {
	int key;
	bool red; //true => red, false => black
	struct node *left;
	struct node *right;
	struct node *parent;
};

struct tree{
	struct node *root;
	struct node *leaf;
};

struct node * addNode(struct tree * tree, int n);
void print_inorder (struct tree * tree ,struct node * current);
int height(struct tree * tree, struct node * n);
struct node * successor(struct tree * tree, struct node * current);



///////////red black maintenance functions///////////////////
void left_rotate (struct tree * tree, struct node * current);
void right_rotate (struct tree * tree, struct node * current);
void insert_fixup (struct tree * tree, struct node * inserted);
void insert (struct tree * tree, struct node *current);
void delete ( struct tree * tree , struct node * to_delete);

////////testing functions////////////////////////////////////
bool test (struct tree * tree);
bool red_test (struct tree * tree, struct node * red);
int black_height(struct tree * tree, struct node * n, bool test);


