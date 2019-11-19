/*Bishal Adhikari
ID :- 1001664327
Code to compile in omega:-
gcc -std=c99 lab4driver.c RB.c
./a.out < in.txt
red to black reset has occurred at root!!!
red to black reset has occurred at root!!!

19 -1b-2b..1b0r..2r..
*/
// Bottom-up red-black tree implementation without deletions,
// so free() does not appear.

// July 22, 2016 - modified for CLRS, 3rd ed. insertion

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RB.lab4.h"

char *flatstrpointer;     //a global string that holds the string and tracks the string
link z,head,temp;               // Pointers to sentinel and root
Item NULLitem=(-9999999);  // Data for sentinel
char symbol;
char itemint[10];
int itemorign;
char serialized[200];

int trace=0;  // Controls trace output for insert

link NEW(Item item, link l, link r, int N)
// Allocates and fills in a node
{
link x = malloc(sizeof *x);
x->item = item;
x->l = l;
x->r = r;
x->red = 1;
x->N = N;
return x;
}

void STinit()
{
// Root/sentinel (head/z) is always black
head = (z = NEW(NULLitem, 0, 0, 0));
head->red=0;
}

Item searchR(link h, Key v)
// Recursive search for a key
{
Key t = key(h->item);
if (h == z)
  return NULLitem;
if (eq(v, t))
  return h->item;
if (less(v, t))
  return searchR(h->l, v);
return searchR(h->r, v);
}

Item STsearch(Key v)
{
return searchR(head, v);
}

Item selectR(link h, int i)
// Returns the ith smallest key where i=1 returns the smallest
// key.  Thus, this is like flattening the tree inorder into an array
// and applying i as a subscript.
{
int r = h->l->N+1;

if (h == z)
{
  printf("Impossible situation in selectR\n");
  STprintTree();
  exit(0);
}
if (i==r)
  return h->item;
if (i<r)
  return selectR(h->l, i);
return selectR(h->r, i-r);
}

Item STselect(int k)
{
if (k<1 || k>head->N)
{
  printf("Range error in STselect() k %d N %d\n",k,head->N);
  exit(0);
}
return selectR(head, k);
}

int invSelectR(link h, Key v)
// Inverse of selectR
{
Key t = key(h->item);
int work;

if (h==z)
  return -1;  // v doesn't appear as a key
if (eq(v, t))
  return h->l->N+1;
if (less(v, t))
  return invSelectR(h->l,v);
work=invSelectR(h->r,v);
if (work==(-1))
  return -1;  // v doesn't appear as a key
return 1 + h->l->N + work;
}

int STinvSelect(Key v)
{
return invSelectR(head,v);
}

void fixN(link h)
// Fixes subtree size of h, assuming that subtrees have correct sizes
{
h->N=h->l->N + h->r->N + 1;
}

link rotR(link h)
// Rotate right at h, i.e. flip edge between h & h->l
{
link x = h->l;
h->l = x->r;
x->r = h;

x->N = x->r->N;
fixN(x->r);
return x;
}

link rotL(link h)
// Rotate left at h, i.e. flip edge between h & h->r
{
link x = h->r;
h->r = x->l;
x->l = h;

x->N = x->l->N;
fixN(x->l);
return x;
}

void extendedTraceOn()
{
trace=2;
}

void basicTraceOn()
{
trace=1;
}

void traceOff()
{
trace=0;
}

void tracePrint(char *s,link h)
{
if (trace) {
  if (h==z)
    printf("%s at sentinel\n",s);
  else
    printf("%s at %d\n",s,key(h->item));
  }
}

link RBinsert(link h, Item item, int sw, int siblingRed, link hParent)
// CLRS, 3rd ed., RB tree insertion done recursively without parent pointers.
// Also includes tracing.  See 2320 notes.  BPW
// h is present node in search down tree.
// Returns root of modified subtree.
// item is the Item to be inserted.
// sw == 1 <=> h is to the right of its parent.
// siblingRed has color of h's sibling.
// hParent has h's parent to facilitate case 1 color flips.
{
Key v = key(item);
link before;  // Used to trigger printing of an intermediate tree

tracePrint("Down",h);
if (h == z)
  return NEW(item, z, z, 1);  // Attach red leaf

if (eq(v, h->item))
  return h;
else if (less(v, h->item)) {
  tracePrint("Insert left",h);
  before=h->l;
  h->l = RBinsert(h->l, item, 0, h->r->red, h); // Insert in left subtree
  if (trace==2 && before!=h->l)  // Has a rotation occurred?
    STprintTree();
  if (h->l->red) {
    if (h->red)
      if (sw)
        if (siblingRed) {
          tracePrint("Case ~1l",hParent);
          hParent->red = 1;
          hParent->l->red = 0;
          hParent->r->red = 0;
          if (trace==2)
            STprintTree();
          }
        else {
          tracePrint("Case ~2",h);
          h = rotR(h);  // Set up case ~3 after return
          }
      else if (siblingRed) {
        tracePrint("Case 1l",hParent);
        hParent->red = 1;
        hParent->l->red = 0;
        hParent->r->red = 0;
        if (trace==2)
          STprintTree();
        }
      else
        ;  // Future case 3
    else if (!h->r->red && h->l->l->red) {
      tracePrint("Case 3",h);
      h = rotR(h);
      h->red = 0;
      h->r->red = 1;
      }
    }
  }
else {
  tracePrint("Insert right",h);
  before=h->r;
  h->r = RBinsert(h->r, item, 1, h->l->red, h); // Insert in right subtree
  if (trace==2 && before!=h->r)  // Has a rotation occurred?
    STprintTree();
  if (h->r->red) {
    if (h->red)
      if (!sw)
        if (siblingRed) {
          tracePrint("Case 1r",hParent);
          hParent->red = 1;
          hParent->l->red = 0;
          hParent->r->red = 0;
          if (trace==2)
            STprintTree();
          }
        else {
          tracePrint("Case 2",h);
          h = rotL(h);  // Set up case 3 after return
          }
      else if (siblingRed) {
        tracePrint("Case ~1r",hParent);
        hParent->red = 1;
        hParent->l->red = 0;
        hParent->r->red = 0;
        if (trace==2)
          STprintTree();
        }
      else
        ;  // Future case ~3
    else if (!h->l->red && h->r->r->red) {
      tracePrint("Case ~3",h);
      h = rotL(h);
      h->red = 0;
      h->l->red = 1;
      }
    }
  }

fixN(h);
tracePrint("Up",h);
return h;
}

void STinsert(Item item)
{
head = RBinsert(head, item, 0, 0, NULL);
if (head->red)
  printf("red to black reset has occurred at root!!!\n");
head->red = 0;
}

void checkRed(link h,int redParent)
// Verifies property 3 in notes
{
if (redParent && h->red)
{
  printf("Red property problem at %d\n",key(h->item));
  STprintTree();
  exit(0);
}
if (h==z)
  return;
checkRed(h->l,h->red);
checkRed(h->r,h->red);
}

int leftPathBlackHeight(link h)
// Counts black nodes on path to the minimum
{
if (h==z)
  return !(h->red);
return leftPathBlackHeight(h->l) + !(h->red);
}

void checkBlack(link h,int blackCount)
// Checks that all paths downward from a node have the same
// number of black nodes
{
if (h==z) {
  if (blackCount==!(h->red))
    return;
  else {
    printf("Black height problem!\n");
    STprintTree();
    exit(0);
    }
  }
if (h->red)
{
  checkBlack(h->l,blackCount);
  checkBlack(h->r,blackCount);
}
else
{
  checkBlack(h->l,blackCount-1);
  checkBlack(h->r,blackCount-1);
}
}

Key lastInorder;    // Saves key from last node processed

void checkInorder(link h)
// Checks that inorder yields keys in ascending order
{
if (h==z)
  return;

checkInorder(h->l);
if (!less(lastInorder,h->item))
{
  printf("Inorder error\n");
  STprintTree();
  exit(0);
}
lastInorder=key(h->item);
checkInorder(h->r);
}

int checkN(link h)
// Verifies that subtree sizes are correct
{
int work;

if (h==z)
{
  if (h->N!=0)
  {
    printf("Count for sentinel is %d, should be 0\n",h->N);
    STprintTree();
    exit(0);
  }
}
else
{
  work=checkN(h->l) + checkN(h->r) + 1;
  if (h->N!=work)
  {
    printf("Count for key %d is %d, should be %d\n",key(h->item),h->N,work);
    STprintTree();
    exit(0);
  }
}
return h->N;
}

void verifyRBproperties()
// Checks all required properties.
// If a fatal problem is found, the tree is printed before exit(0)
{
int lpbHeight;

if (head->red)
  printf("Root is not black!\n");
if (z->red)
  printf("Sentinel is not black!\n");
lastInorder=(-99999999);
checkInorder(head);
checkRed(head,0);
lpbHeight=leftPathBlackHeight(head);
checkBlack(head,lpbHeight);
checkN(head);
}

void printTree(link h,int depth,int bhAbove)
{
int i,bhBelow;

if (h==z)
{
  if (bhAbove!=1)
  {
    for (i=0;i<depth;i++)
      printf("     ");
    printf("Black-height issue detected at sentinel\n");
  }

  return;
}

if ((h->red))
  bhBelow=bhAbove;
else
  bhBelow=bhAbove-1;
printTree(h->r,depth+1,bhBelow);
for (i=0;i<depth;i++)
  printf("     ");
if (h->red)
  printf("[%d %d %d]\n",key(h->item),h->N,bhBelow);
else
  printf("(%d %d %d)\n",key(h->item),h->N,bhBelow);
printTree(h->l,depth+1,bhBelow);
}

void STprintTree()
{
printTree(head,0,leftPathBlackHeight(head));
}

void fixAllN(link h)
// Recomputes subtree sizes for an otherwise correct tree
{
if (h->l)
  fixAllN(h->l);
else
  h->l=z;
if (h->r)
  fixAllN(h->r);
else
  h->r=z;
fixN(h);
}

void cleanUpUnbalanced(link h)
// Checks a tree constructed elsewhere (like RB.loadAndGo.c)
{
fixAllN(h);
head=h;
z->red=0;
verifyRBproperties();
}
link unflattening()
{
  int sign;//holds -ve or +ve value of the item
  char color;//holds 1 for red and 0 for black node
  int itemval=0;//holds the value of the item
    if(*flatstrpointer == '.'){
      flatstrpointer++;
      return z; //'.' represents the branch termination
    }
    if(*flatstrpointer == '-'){
      sign = -1;//if the item is negative
      flatstrpointer++;//traverseing the string
    }else if(*flatstrpointer == '+'){
      sign = 1;
      flatstrpointer++;
    }
    else{
      sign = 1;
    }
    while (*flatstrpointer != '\0')//traverses the string unless loop broken
    {
      if (*flatstrpointer=='b') {
        flatstrpointer++;//traveres the string
        color = 0;//since the color is black
        break;// breaks the loop to create a node from the data we have
      }
      else if(*flatstrpointer=='r'){
        flatstrpointer++;//traverses the string
        color =1;//since the color is red
        break;// breaks the loop to create a node from the data we have
      }
        itemval = itemval*10 + (*flatstrpointer-'0');//converting the string to int and saving the item value
        flatstrpointer++;//traveres the string

    }
    itemval = itemval*sign;
    link tempx = NEW(itemval,0,0,0);
    tempx->red = color;
    tempx->l = unflattening();
    tempx->r = unflattening();

    fixN(tempx);
    return tempx;

}
void STdeserialize(char *str){
  flatstrpointer = str;
  head = unflattening();
  if(!(*flatstrpointer)){
    verifyRBproperties();
  }
}
void subSTserialize(link temp)
{
  if(temp->item == NULLitem){
    sprintf(itemint,"%c",'.');//string terminator symbol if branch ends
    strcat(serialized,itemint);
    return ;
  }
  itemorign = temp->item;
  sprintf(itemint,"%d",itemorign);//saving the item(int) in itemint(string)
  getchar();//to handel empty stting input
  strcat(serialized, itemint);//collecting the itemint(string) into serialized(global)

  if(temp->red == 1){
    symbol ='r';
  } else{
    symbol = 'b';
  }
  strncat(serialized,&symbol,1);//collecting the color code in the searilized
  subSTserialize(temp->l);
  subSTserialize(temp->r);
}
char* STserialize(){
  subSTserialize(head);
  return serialized;
}
