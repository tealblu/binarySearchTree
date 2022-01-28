/*
 * Name: Charlie Hartsell
 * Date Submitted: March 26, 2021
 * Lab Section: 004
 * Assignment Name: Lab 7
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

using namespace std;

struct Node {
  int key;
  int size;
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = nullptr; }
};

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, int k)
{
  if (T == nullptr) return new Node(k);
  if (k < T->key) T->left = insert(T->left, k);
  else T->right = insert(T->right, k);
  fix_size(T);
  return T;
}

// returns a vector of key values corresponding to the inorder traversal of T (i.e., the contents of T in sorted order)
vector<int> inorder_traversal(Node *T)
{
  vector<int> inorder;
  vector<int> rhs;
  if (T == nullptr) return inorder;
  inorder=inorder_traversal(T->left);
  inorder.push_back(T->key);
  rhs=inorder_traversal(T->right);
  inorder.insert(inorder.end(), rhs.begin(), rhs.end());
  return inorder;
}

// return a pointer to the node with key k in tree T, or nullptr if it doesn't exist
Node *find(Node *T, int k)
{
  // Create ptr for resultant node
  Node* iterator = T;

  // Loop through tree
  while(iterator != nullptr) {
    if(k > iterator->key) {
      // k greater than current key, move right
      iterator = iterator->right;
    } else if(k < iterator->key) {
      // k less than current key, move left
      iterator = iterator->left;
    } else {
      // k equal to current key, return current node pointer
      return iterator;
    }
  }

  // key not found, return null pointer
  return nullptr;
}

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node *select(Node *T, int r)
{
  assert(T!=nullptr && r>=0 && r<T->size);

  // Create vector of inorder traversal
  vector<int> trav = inorder_traversal(T);

  // Find r'th key
  int k = trav[r];

  // Search T for key and return found node
  return find(T, k);
}

// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  
Node *join(Node *L, Node *R)
{
  // choose either the root of L or the root of R to be the root of the joined tree
  // (where we choose with probabilities proportional to the sizes of L and R)
  
  //Implement Node *join(Node *L, Node *R)

  // if (rand() % N == 0) A; else B; executes A with prob 1/N

  if(L == nullptr && R == nullptr) {
    return nullptr;
  } else if(L == nullptr) {
    return R;
  } else if(R == nullptr) {
    return L;
  } else {
    // Calculate total size of resultant tree
    int totalSize = L->size + R->size;

    // Probability calculation and execution
    if(rand() % (totalSize/L->size) == 0) {
      // Use L as root
      // Inorder traversal of non-root
     vector<int> trav = inorder_traversal(R);

      // Add non-root values to root
      for(int i = 0; i < R->size; i++) {
        L = insert(L, trav[i]);
        fix_size(L);
      }

      // Return root
     return L;
   } else {
      // Use R as root
      // No comments here because it's the exact same as above but with L and R switched
      vector<int> trav = inorder_traversal(L);

      for(int i = 0; i < L->size; i++) {
        R = insert(R, trav[i]);
        fix_size(R);
      }

      return R;
    }
  }
}

// remove key k from T, returning a pointer to the resulting tree.
// it is required that k be present in T
Node *remove(Node *T, int k)
{
  assert(T != nullptr);
  
  //Implement Node *remove(Node *T, int k)
  
  if(T == nullptr) {
    return T;
  }

  if (k < T->key) { // Recurse left
    T->left = remove(T->left, k);
    T->size--;
  } else if (k > T->key) { // Recurse right
    T->right = remove(T->right, k);
    T->size--;
  } else { 
    if (T->left == nullptr) { 
      // No left child, move right
      Node *temp = T->right; 
      delete T; 
      return temp; 
    } else if (T->right == nullptr) { 
      // No right child, move left
      Node *temp = T->left; 
      delete T; 
      return temp; 
    } 

    // Both children exist, merge and delete
    Node* temp = join(T->left, T->right);
    delete T;
    return temp; 
  }

  return T; 
}

// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
void split(Node *T, int k, Node **L, Node **R)
{
  // Create inorder traversal of T
  vector<int> trav = inorder_traversal(T);

  // Initialize L and R base cases
  *L = nullptr;
  *R = nullptr;
  
  // Loop through trav, adding nodes to either L or R depending on key
  for(unsigned int i = 0; i < trav.size(); i++) {
    if(trav[i] <= k) {
      // key less than or equal to k, add to left tree
      *L = insert(*L, trav[i]);
    } else {
      // key greater than k, add to right tree
      *R = insert(*R, trav[i]);
    }
  }
}

// insert key k into the tree T, returning a pointer to the resulting tree
// NOT YET DONE
Node *insert_random(Node *T, int k)
{
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_random k recursively left or right of the root of T
  
  //Implement Node *insert_random(Node *T, int k)

  // Catch the null case
  if(T == nullptr) {
    T = new Node(k);
    return T;
  } else if(rand() % (T->size + 1) == 0) {
    // insert at root of T
    Node *temp = new Node(k);
    split(T, k, &temp->left, &temp->right);
    fix_size(temp);
    return temp;
  } else {
    // recurse left or right
    if(k < T->key) {
      // k less than T's key, insert_random on left
      T->left = insert_random(T->left, k);
    } else {
      // k greater than or equal to T's key, insert_random on right
      T->right = insert_random(T->right, k);
    }
  }

  // Increment size
  T->size++;

  // Return the resultant tree
  return T;
}

void printVector(vector<int> v)
{
    for (unsigned int i=0; i<v.size(); i++)
    {
        cout << v[i] << endl;
    }
}

int main(void)
{
  vector<int> inorder;
  vector<int> A(10,0);
  
  // put 0..9 into A[0..9] in random order
  for (int i=0; i<10; i++) A[i] = i;
  for (int i=9; i>0; i--) swap(A[i], A[rand()%i]);
  cout << "Contents of A (to be inserted into BST):\n";
  printVector(A);
  
  // insert contents of A into a BST
  Node *T = nullptr;
  for (int i=0; i<10; i++) T = insert(T, A[i]);
  
  // print contents of BST (should be 0..9 in sorted order)
  cout << "Contents of BST (should be 0..9 in sorted order):\n";
  inorder=inorder_traversal(T);
  printVector(inorder);

  // test select
  for (int i=0; i<10; i++) {
    Node *result = select(T, i);
    if (!result || result->key != i) cout << "Select test failed for select(" << i << ")!\n";
  }

  // test find: Elements 0..9 should be found; 10 should not be found
  cout << "Elements 0..9 should be found; 10 should not be found:\n";
  for (int i=0; i<11; i++) 
    if (find(T,i)) cout << i << " found\n";
    else cout << i << " not found\n";

  // test split
  Node *L, *R;
  split(T, 4, &L, &R);
  cout << "Contents of left tree after split (should be 0..4):\n";
  inorder=inorder_traversal(L);
  printVector(inorder);
  cout << "Left tree size " << L->size << "\n";
  cout << "Contents of right tree after split (should be 5..9):\n";
  inorder=inorder_traversal(R);
  printVector(inorder);
  cout << "Right tree size " << R->size << "\n";
    
  // test join
  T = join(L, R);
  cout << "Contents of re-joined tree (should be 0..9)\n";
  inorder=inorder_traversal(T);
  printVector(inorder);
  cout << "Tree size " << T->size << "\n";
  
  // test remove
  for (int i=0; i<10; i++) A[i] = i;
  for (int i=9; i>0; i--) swap(A[i], A[rand()%i]);
  for (int i=0; i<10; i++) {
    T = remove(T, A[i]);
    cout << "Contents of tree after removing " << A[i] << ":\n";
    inorder=inorder_traversal(T);
    printVector(inorder);
    if (T != nullptr)
      cout << "Tree size " << T->size << "\n";
  }

  // test insert_random
  cout << "Inserting 1 million elements in order; this should be very fast...\n";
  for (int i=0; i<1000000; i++) T = insert_random(T, i);
  cout << "Tree size " << T->size << "\n";
  cout << "Done\n";
  
  return 0;
}
