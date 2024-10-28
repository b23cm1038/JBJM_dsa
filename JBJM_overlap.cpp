#include <iostream>
using namespace std;


struct Interval {
    int low, high;
};


struct IntervalTreeNode {
    Interval interval;       
    int max;                
    IntervalTreeNode *left;
    IntervalTreeNode *right;
};


IntervalTreeNode* newNode(Interval i) {
    IntervalTreeNode *temp = new IntervalTreeNode;
    temp->interval = i;
    temp->max = i.high;
    temp->left = temp->right = nullptr;
    return temp;
}


IntervalTreeNode* insert(IntervalTreeNode* root, Interval i) {
    if (!root)
        return newNode(i);

   
    if (i.low < root->interval.low)
        root->left = insert(root->left, i);
    else
        root->right = insert(root->right, i);

    
    if (root->max < i.high)
        root->max = i.high;

    return root;
}


bool doOverlap(Interval i1, Interval i2) {
    return (i1.low <= i2.high && i2.low <= i1.high);
}


Interval* overlapSearch(IntervalTreeNode* root, Interval i) {
    if (!root) {
        return NULL;
    }

    
    if (doOverlap(root->interval, i)){
        return &root->interval;
    }

    if (root->left && root->left->max >= i.low){
        return overlapSearch(root->left, i);
    }
    
    return overlapSearch(root->right, i);
}


void inOrder(IntervalTreeNode* root) {
    if (!root) return;
    inOrder(root->left);
    cout << "[" << root->interval.low << ", " << root->interval.high << "]"
         << " max = " << root->max << endl;
    inOrder(root->right);
}

int main() {
    Interval intervals[] = {{15, 20}, {10, 30}, {17, 19}, {5, 20},
                            {12, 15}, {30, 40}};
    int n = sizeof(intervals) / sizeof(intervals[0]);
    IntervalTreeNode *root = nullptr;

    
    for (int i = 0; i < n; i++)
        root = insert(root, intervals[i]);

    cout << "In-order traversal of the constructed Interval Tree:\n";
    inOrder(root);

    Interval x = {6, 7}; 
    cout << "\nSearching for interval [" << x.low << ", " << x.high << "]\n";
    Interval *res = overlapSearch(root, x);

    if (res)
        cout << "Overlaps with [" << res->low << ", " << res->high << "]\n";
    else
        cout << "No overlapping interval found\n";

    return 0;
}
