// {{{ $VIMCODER$ <-----------------------------------------------------
// vim:filetype=cpp:foldmethod=marker:foldmarker={{{,}}}

#include <algorithm>
#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// }}}

struct TreeNode {

  TreeNode(int k, bool c) : key(k), color(c) {/*empty*/}
  TreeNode(int k) : TreeNode(k, kRed) {/*empty*/}

  static TreeNode *nil;
  static const bool kRed, kBlack;

  TreeNode *p = nil, *left = nil, *right = nil;

  int key = 0;
  bool color = kRed;
};

// init.
const bool TreeNode::kRed = true;
const bool TreeNode::kBlack = false;
TreeNode *TreeNode::nil = new TreeNode(-1, TreeNode::kBlack);


struct Tree {
  TreeNode *root = TreeNode::nil;
};


TreeNode *LeftRotate(Tree &tree, TreeNode *x) {
  auto y = x->right;

  // 1.
  x->right = y->left;
  y->left->p = x;

  // 2.
  if (x == x->p->left) {
    x->p->left = y;
  } else {
    x->p->right = y;
  }
  y->p = x->p;

  // 3.
  y->left = x;
  x->p = y;

  if (x == tree.root) {
    tree.root = y;
  }

  return y;
}


TreeNode *RightRotate(Tree &tree, TreeNode *y) {
  auto x = y->left;

  // 1.
  y->left = x->right;
  x->right->p = y;

  // 2.
  if (y == y->p->left) {
    y->p->left = x;
  } else {
    y->p->right = x;
  }
  x->p = y->p;

  // 3.
  x->right = y;
  y->p = x;

  if (y == tree.root) {
    tree.root = x;
  }

  return x;
}


void InsertFixUp(Tree &tree, TreeNode *z) {

  while (z->p->color == TreeNode::kRed) {
    if (z->p == z->p->p->left) {
      auto y = z->p->p->right;

      if (y->color == TreeNode::kRed) {
        // case 1.1
        z->p->p->color = TreeNode::kRed;
        z->p->color = y->color = TreeNode::kBlack;
        z = z->p->p;
      } else {
        if (z == z->p->right) {
          // case 1.2
          z = LeftRotate(tree, z->p);
          z = z->left;
        }
        // case 1.3
        z->p->p->color = TreeNode::kRed;
        z->p->color = TreeNode::kBlack;
        z = RightRotate(tree, z->p->p);
      }

    } else {
      auto y = z->p->p->left;

      if (y->color == TreeNode::kRed) {
        // case 2.1
        z->p->p->color = TreeNode::kRed;
        z->p->color = y->color = TreeNode::kBlack;
        z = z->p->p;
      } else {
        if (z == z->p->left) {
          // case 2.2
          z = RightRotate(tree, z->p);
          z = z->right;
        }
        // case 2.3
        z->p->p->color = TreeNode::kRed;
        z->p->color = TreeNode::kBlack;
        z = LeftRotate(tree, z->p->p);
      }

    }
  }

  tree.root->color = TreeNode::kBlack;
  
}

void Insert(Tree &tree, int key) {
  auto y = TreeNode::nil;
  auto x = tree.root;

  while (x != TreeNode::nil) {
    y = x;
    if (x->key == key) {
      return;
    }
    if (key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  auto z = new TreeNode(key);
  if (y == TreeNode::nil) {
    tree.root = z;
  } else {
    z->p = y;
    if (key < y->key) {
      y->left = z;
    } else {
      y->right = z;
    }
  }

  InsertFixUp(tree, z);
}


// standard binary search.
TreeNode *Search(Tree &tree, int key) {
  auto node = tree.root;
  while (node != TreeNode::nil) {
    if (node->key == key) {
      return node;
    }
    if (node->key < key) {
      node = node->right;
    } else {
      node = node->left;
    }
  }
  return nullptr;
}


void Replace(Tree &tree, TreeNode *u, TreeNode *v) {
  // replace u with v.
  if (u == u->p->left) {
    u->p->left = v;
  } else {
    u->p->right = v;
  }
  v->p = u->p;

  if (tree.root == u) {
    tree.root = v;
  }
}


void DeleteFixUp(Tree &tree, TreeNode *x) {
  while (x != tree.root && x->color == TreeNode::kBlack) {
    
    if (x == x->p->left) {
      auto w = x->p->right;
      
      if (w->color == TreeNode::kRed) {
        // case 1.1, convert to case 1.2, 1.3, 1.4.
        w->p->color = TreeNode::kRed;
        w->color = TreeNode::kBlack;
        LeftRotate(tree, x->p);
        w = x->p->right;
      }

      if (w->left->color == TreeNode::kBlack
          && w->right->color == TreeNode::kBlack) {
        // case 1.2.
        w->color = TreeNode::kRed;
        x = x->p;
      } else {
        if (w->left->color == TreeNode::kRed
            && w->right->color == TreeNode::kBlack) {
          // case 1.3, convert to case 1.4.
          w->color = TreeNode::kRed;
          w->left->color = TreeNode::kBlack;
          w = RightRotate(tree, w);
        }
        // case 1.4.
        w->color = w->p->color;
        w->p->color = w->right->color = TreeNode::kBlack;
        LeftRotate(tree, w->p);
        x = tree.root;
      }

    } else {
      auto w = x->p->left;

      if (w->color == TreeNode::kRed) {
        // case 2.1
        w->p->color = TreeNode::kRed;
        w->color = TreeNode::kBlack;
        RightRotate(tree, w->p);
        w = x->p->left;
      }
      
      if (w->left->color == TreeNode::kBlack
          && w->right->color == TreeNode::kBlack) {
        // case 2.2.
        w->color = TreeNode::kRed;
        x = x->p;
      } else {
        if (w->right->color == TreeNode::kRed
            && w->left->color == TreeNode::kBlack) {
          // case 2.3.
          w->color = TreeNode::kRed;
          w->right->color = TreeNode::kBlack;
          w = LeftRotate(tree, w);
        }
        // case 2.4.
        w->color = w->p->color;
        w->left->color = w->p->color = TreeNode::kBlack;
        RightRotate(tree, w->p);
        x = tree.root;
      }

    }
  }

  x->color = TreeNode::kBlack;
}


void Delete(Tree &tree, TreeNode *z) {
  if (z == nullptr) {
    return;
  }
  TreeNode *y = nullptr, *x = nullptr;
  bool y_original_color = TreeNode::kBlack;

  if (z->left == TreeNode::nil || z->right == TreeNode::nil) {
    y = z;
    y_original_color = y->color;

    if (z->left == TreeNode::nil) {
      x = z->right;
    } else {
      x = z->left;
    }
    Replace(tree, y, x);

  } else {
    y = z->right;
    while (y->left != TreeNode::nil) {
      y = y->left;
    }
    x = y->right;

    if (z != y->p) {
      // ignore the case where y == z->right.
      Replace(tree, y, x);
      // bound y to z->right.
      y->right = z->right;
      z->right->p = y;
    }
    Replace(tree, z, y);
    y->left = z->left;
    z->left->p = y;

    y_original_color = y->color;
    y->color = z->color;
  }

  if (y_original_color == TreeNode::kBlack) {
    DeleteFixUp(tree, x);
  }
}


void PrintTree(Tree &tree) {
  queue<TreeNode *> q;
  q.push(tree.root);
  while (!q.empty()) {
    int n = q.size();
    for (int i = 0; i < n; ++i) {
      auto node = q.front();
      q.pop();
      if (node == TreeNode::nil) {
        cout << "#[b] ";
      } else {
        cout << node->key
             << '[' << ((node->color == TreeNode::kBlack) ? 'b' : 'r') << ']'
             << ' ';
        q.push(node->left);
        q.push(node->right);
      }
    }
    cout << endl;
  }
  cout << endl;
}


int main() {
  Tree tree;
  cout << tree.root << endl;

  for (int num = 1; num <= 50; ++num) {
    Insert(tree, num);
    PrintTree(tree);
  }
  cout << "-----------------------------------" << endl;
  for (int num = 1; num <= 50; ++num) {
    auto x = Search(tree, num);
    cout << num << ", " << x << ", " << x->key << endl;
    Delete(tree, x);
    PrintTree(tree);
  }
}
