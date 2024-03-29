#include <iostream>
#include <cstring>
#include <fstream>

/*Lina Goto
3/5/24
Your program should read a mathematical expression entered with infix notation, using spaces between each token. It will then output the postfix expression. You should then be able to output infix, prefix, or postfix notation, which is a recursive print from the expression tree.
 */


using namespace std;

/* enumeration */
enum { zero, one, two, three, four, five, six, seven, eight, nine, minus, plus, divide, multiply, power, rparenthesis, lparenthesis, space, none };

static char optype   [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+', '/', '*', '^', ')', '(', ' ', '@'};
static int precedence[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, -1, -1, -1, -1 }; 

typedef struct tree_s {
  char value;
  tree_s *next, *left, *right;
} tree_t;

class shunting {
private:
  tree_t *stack;
  tree_t *queue;
  tree_t *tree;
  
public:
  shunting(char *input);

  tree_t *gendata(char value);

  /* for stack operation */
  void push(char value);
  char pop(void);
  char peek(void);

  /* for queue / enqueue operation */
  void enqueue(char value);
  char dequeue(void);
  
  /* debugging purpose */
  void printstack(void);
  void printqueue(void);

  /* generating tree */
  void gentree(void);

  /* printing tree */
  void print_infix  (tree_t *current);
  void print_prefix (tree_t *current);
  void print_postfix(tree_t *current);
  
};

tree_t *shunting::gendata(char value) {
  tree_t *data = new tree_t;
  data -> value = value;
  data -> next  = NULL;
  data -> left  = NULL;
  data -> right = NULL;
  return data;
}

/* enqueue */
void shunting::enqueue(char value) {
  tree_t *current = queue;
  tree_t *data = gendata(value);
  if (queue == NULL) {
    queue = data;
  } else {
    while(current -> next != NULL) current = current -> next;
    current -> next = data;
  }
}

/* dequeue */
char shunting::dequeue(void) {
  tree_t *current = queue;
  char value = none;
  
  if (queue != NULL) {
    value = queue -> value;
    queue = queue -> next;
    delete current;
  }
  
  return value;
}

/* insert new element on stack */
void shunting::push(char value) {
  tree_t *data = gendata(value);
  data -> next = stack;
  stack = data;
}

/* remove the topmost element of the stack */
char shunting::pop(void) {
  tree_t *current = stack;
  char value = none;
  
  if (stack != NULL) {
    value = stack -> value;
    stack = stack -> next;
    delete current;
  }
  
  return value;
}

/* get the value of the topmost element of the stack */
char shunting::peek(void) {
  char value = none;
  if (stack != NULL) value = stack -> value;
  return value;
}

void shunting::printstack(void) {
  tree_t *current = stack;

  while (current != NULL) {
    cout << optype[current -> value];
    current = current -> next;
    if (current != NULL) cout << ' ';
  }
  cout << endl;
}

shunting::shunting(char *input) {
  char *i = input;
  char cvalue, nvalue;
  
  stack = NULL;
  queue = NULL;
  tree  = NULL;
  
  cout << "Input   : " << input << endl;

  while (*i != '\0') {
    /* data conversion */
    cvalue = zero;
    while ((cvalue < none) && (*i != optype[cvalue])) cvalue ++; 

    if (cvalue < space) {
      if ((cvalue >= zero) && (cvalue <= nine)) {
	/* if token is numbers, just copy directly */
	enqueue(cvalue);
      } else if (cvalue == lparenthesis) {
	/* simply push */
	push(cvalue);
      } else if (cvalue == rparenthesis) {
	/* discard cvalue */
https://github.com/LinaGoto/Shunting-Yard-Algorithm.git	/* retrieve operator until '(' is found */
	do {
	  nvalue = pop();
	  if (nvalue != lparenthesis) {
	    enqueue(nvalue);
	  }
	} while (nvalue != lparenthesis);
      } else if (cvalue <= power) {
	/* operator */
	nvalue = peek();
	/* top is empty or left parenthesis, higher precedence and right associative  */
	if (
	    /* top is empty */
	    (nvalue == none)
	    /* left parenthesis */
	    || (nvalue == lparenthesis)
	    /* higher precedence */
	    || (precedence[cvalue] > precedence[nvalue])
	    /* same precedence and right associative */
	    || ((precedence[cvalue] == precedence[nvalue]) & (cvalue == power))) {
	  /* just push */
	  push(cvalue);
	} else {
	  /* repeat until */
	  while (
		 /* lower precednce */
		 (precedence[cvalue] < precedence[nvalue])
		 /* same precednce and not right associative */
		 || ((precedence[cvalue] == precedence[nvalue]) && (cvalue != power))) {
	    enqueue(pop());
	    nvalue = peek();
	  }
	  /* then push */
	  push(cvalue);
	}
      }
      //      printstack();
    }
    i ++;
  }

  /* retrieve all stack */
  do {
    nvalue = pop();
    enqueue(nvalue);
  } while (nvalue != none);
}

void shunting::gentree(void) {
  char value;
  tree_t *data, *current, *prev;
  
  while ((value = dequeue()) != none) {
    data = gendata(value);

    if ((value >= one) && (value <= nine)) {
      /* simple add */
      current = tree;
      if (tree == NULL) {
	tree = data;
      } else {
	while (current -> next != NULL) current = current -> next;
	current -> next = data;
      }
    } else {
      /* find the second last one and generate new tree */
      current = tree;
      while (current -> next -> next != NULL) current = current -> next;

      /* update pointers */
      data -> left  = current;
      data -> right = current -> next;
      /* clear next pointer */
      data -> left  -> next = NULL;
      data -> right -> next = NULL;

      if (current == tree) {
	tree = data;
      } else {
	/* find out previous pointer */
	prev = tree;
	while (prev -> next != current) prev = prev -> next;
	prev -> next = data;
      }
    }
  }
}

void shunting::printqueue(void) {
  tree_t *current = queue;
  cout << "Encoded : ";
  while ((current != NULL) && (current -> value != none)) {
    cout << optype[current -> value] << ' ';
    current = current -> next;
  }
  cout << endl << endl;
}

void shunting::print_postfix(tree_t *current) {
  /* if current is NULL, it means start printing */
  if (current == NULL) {
    current = tree;
    cout << "Postfix :";
  }
  if (tree != NULL) {
    if (current -> left  != NULL) print_postfix(current -> left);
    if (current -> right != NULL) print_postfix(current -> right);
    cout << ' ' << optype[current -> value];
  }
  if (current == tree) cout << endl;
}

void shunting::print_prefix(tree_t *current) {
  /* if current is NULL, it means start printing */
  if (current == NULL) {
    current = tree;
    cout << "Prefix  :";
  }
  if (tree != NULL) {
    cout << ' ' << optype[current -> value];
    if (current -> left  != NULL) print_prefix(current -> left);
    if (current -> right != NULL) print_prefix(current -> right);
  }
  if (current == tree) cout << endl;
}

void shunting::print_infix(tree_t *current) {
  /* if current is NULL, it means start printing */
  if (current == NULL) {
    current = tree;
    cout << "Infix   :";
  }
  if (tree != NULL) {
    if (current -> left  != NULL) {
      if ((precedence[current -> value] > 0) && (precedence[current -> value] > precedence[current -> right -> value])) cout << " (";
      print_infix(current -> left);
    }
    cout << ' ' << optype[current -> value];
    if (current -> right != NULL) {
      print_infix(current -> right);
      if ((precedence[current -> value] > 0) && (precedence[current -> value] > precedence[current -> right -> value]))  cout << " )";
    }
  }
  if (current == tree) cout << endl;
}

int main(void) {
  /* trial stuff*/
  //char input[] = "1 * 2 + 3";
  //  char input[] = "1 + 2 * 3";
  //    char input[] = "1 * ( 2 + 3 )";
  //  char input[] = "1 - 2 + 3";
  //  char input[] = "1 * 2 ^ 3 + 4";
  //  char input[] = "1 + 2 + 3 + 4";
  //char input[] = "3 + 4 * 2 / ( 1 - 5 ) ^ 2";


    char input [1000];
    cout << "Enter the equation:";
    cin.get (input, 1000);
    cin.get();

    //encode
    shunting shunting (input);
    //print encoded data
    shunting.printqueue();

    // generate tree
    shunting.gentree();
    
    char output [100];
    cout << "Printout by: postfix, infix, or prefix" << endl;
    cin.get (output, 100);
    cin.get();

    if (output[0] == 'P' || output[0] == 'p'){
      if (output[1] == 'O' || output[1] == 'o'){
	shunting.print_postfix(NULL);
      }
      if (output[1] == 'R' || output[1] == 'r'){
	shunting.print_prefix(NULL);
      }
    }
    if (output[0] == 'I' || output[0] == 'i'){
      shunting.print_infix(NULL);
    }
  
  return 0;
}
