#include<bits/stdc++.h>
using namespace std;

#define ll  long long 
#define elif else if

ll Max(ll a, ll b, ll c)
{
    if(a > b)
    {
        if(a > c)   return a;
        return c;
    }

    else
    {
        if(b > c)   return b;
        return c;
    }
}

//using RB tree to implement IntervalTree
class IntervalTree
{
private:
    class node
    {
        public:

            ll LeftEndPoint;
            ll RightEndPoint;
            ll MaxRight = 0;
            ll Priority;
            node* Parent;
            node* Left;
            node* Right;
            bool isRed ;

        node(ll L, ll R)
        {
            LeftEndPoint = L;
            RightEndPoint = R;
            Left = nullptr;
            Right = nullptr;
            Parent = nullptr;
            //all new nodes are red when created because it is easier to solve double red problem than black depth problem
            isRed = true;
        }

    };

    node* CreateNode(ll L, ll R)
    {
        return new node(L,R);
    }

    node* Root;

    void Insert(ll L, ll R)
    {
        //simple BST insertion by comparing leftendpoints
        node* x = Root;
        node* y = nullptr;
        node* z = CreateNode(L,R);

        while(x)
        {
            y = x;
            if(L < x->LeftEndPoint) x = x->Left;
            else    x = x->Right;

            //when new node is inserted maxright of all ancestors can change so doing it simultaneously
            if(R > y->MaxRight)    y->MaxRight = R;
        }

        if(!y)
        {  
            Root = z;
            return;
        }    
        else    (L < y->LeftEndPoint)   ?   y->Left = z : y->Right = z;

        z->Parent = y;
        UpdateMaxheight(z);
        //to recover RB prop's that are violated
        Rebalance(z);
    }

    void LeftRotate(node* x)
    {
        node* y = x->Right;
        x->Right = y->Left;

        if(y->Left) y->Left->Parent = x;
        y->Parent = x->Parent;

        if(!x->Parent)  Root = y;
        elif(x == x->Parent->Left)  x->Parent->Left = y;
        else    x->Parent->Right = y;

        y->Left = x;
        x->Parent = y;

        y->MaxRight = x->MaxRight;
        UpdateMaxheight(x);

        return;
    }

    void RightRotate(node* x)
    {
        node* y = x->Left;
        x->Left = y->Right;

        if(y->Right) y->Right->Parent = x;
        y->Parent = x->Parent;

        if(!x->Parent)  Root = y;
        elif(x == x->Parent->Right)  x->Parent->Right = y;
        else    x->Parent->Left = y;

        y->Right = x;
        x->Parent = y;

        y->MaxRight = x->MaxRight;
        UpdateMaxheight(x);

        return;
    }

    void Rebalance(node* n)
    {
        while(n->Parent && n->Parent->Parent && n->Parent->isRed)
        {
            if(n->Parent == n->Parent->Parent->Left)
            {
                node* y = n->Parent->Parent->Right;
            
                if (y && y->isRed)
                {
                    //node inserted has red uncle(parent's sibling)
                    //solved by pushing down grandparent's blackness, then iteratively moving 2 levels up, max till root
                    n->Parent->isRed = false;
                    y->isRed = false;
                    n->Parent->Parent->isRed = true;
                    n = n->Parent->Parent;
                }
                else
                {
                    //can be fixed by rotations leftright or right 
                    if(n == n->Parent->Right)
                    {
                        n = n->Parent;
                        LeftRotate(n);
                    }

                    n->Parent->isRed = false;
                    n->Parent->Parent->isRed = true;

                    RightRotate(n->Parent->Parent);
                }
            }

            else
            {
                node* y = n->Parent->Parent->Left;
            
                if (y && y->isRed)
                {
                    //node inserted has red uncle(parent's sibling)
                    //solved by pushing down grandparent's blackness, then iteratively moving 2 levels up, max till root
                    n->Parent->isRed = false;
                    y->isRed = false;
                    n->Parent->Parent->isRed = true;
                    n = n->Parent->Parent;
                }
                else
                {
                    //can be fixed by rotations rightleft or left 
                    if(n == n->Parent->Left)
                    {
                        n = n->Parent;
                        RightRotate(n);
                    }

                    n->Parent->isRed = false;
                    n->Parent->Parent->isRed = true;

                    LeftRotate(n->Parent->Parent);
                }
            }
        }

        Root->isRed = false;
    }

    void UpdateMaxheight(node* n)
    {
        ll l,r;
        (n->Left == nullptr) ? l = 0 : l = n->Left->MaxRight;
        (n->Right == nullptr) ? r = 0 : r = n->Right->MaxRight;

        n->MaxRight = Max(n->RightEndPoint, l, r);  
    }

    node* FindNode(ll L){
        node* curr = Root;
        while(curr!=nullptr){
            if(L<curr->LeftEndPoint)
                curr = curr->Left;
            elif(L>curr->LeftEndPoint)
                curr = curr->Right;
            else
                return curr;
        }
        return nullptr;
    }
    
    void TransplantNode(node* target,node* replacement){
        if(target->Parent==nullptr)
            Root = replacement;
        elif(target==target->Parent->Left)
            target->Parent->Left = replacement;
        else
            target->Parent->Right = replacement;
        if(replacement!=nullptr)
            replacement->Parent = target->Parent;
    }
    
    node* MinimumNode(node* curr){
        while(curr->Left!=nullptr)
            curr = curr->Left;
        return curr;
    }
    
    void UpdateAncestorsMaxRight(node* ancestor){
        while(ancestor!=nullptr){
            UpdateMaxheight(ancestor);
            ancestor=ancestor->Parent;
        }
    }
    
    void DeleteFix(node* target){
        while(target!=Root && !target->isRed){
            if(target->Parent==nullptr)
                break;
            if(target==target->Parent->Left){
                node* sibling = target->Parent->Right;
                if(sibling!=nullptr && sibling->isRed){
                    sibling->isRed = false;
                    target->Parent->isRed = true;
                    LeftRotate(target->Parent);
                    sibling = target->Parent->Right;
                }
            if(sibling!=nullptr){
                sibling->isRed=target->Parent->isRed;
                if(sibling->Right!=nullptr)
                    sibling->Right->isRed = false;
                target->Parent->isRed = false;
                LeftRotate(target->Parent);
                target = Root;
            }
            else{
                if(sibling->Right==nullptr || !sibling->Right->isRed){
                    if(sibling->Left!=nullptr)
                        sibling->Left->isRed = false;
                    sibling->isRed = true;
                    RightRotate(sibling);
                    sibling = target->Parent->Right;
                }
                if(sibling!=nullptr){
                    sibling->isRed=target->Parent->isRed;
                    if(sibling->Right!=nullptr)
                        sibling->Right->isRed = false;
                }
                target->Parent->isRed = false;
                LeftRotate(target->Parent);
                target= Root;
                
            }
        }
        
        else{
            node* sibling = target->Parent->Left;
            if(sibling!=nullptr && sibling->isRed){
                sibling->isRed = false;
                target->Parent->isRed = true;
                RightRotate(target->Parent);
                sibling = target->Parent->Left;
            }
            if((sibling==nullptr || (sibling->Right == nullptr || !sibling->Right->isRed) && (sibling->Left==nullptr || !sibling->Left->isRed))){
                if(sibling!=nullptr)
                    sibling->isRed = true;
                    target = target->Parent;
            }
            else{
                if(sibling->Left==nullptr || !sibling->Left->isRed){
                    if(sibling->Right!=nullptr)
                        sibling->Right->isRed = false;
                    sibling->isRed = true;
                    LeftRotate(sibling);
                    sibling = target->Parent->Left;
                }
                if(sibling!=nullptr){
                    sibling->isRed=target->Parent->isRed;
                    if(sibling->Left!=nullptr) 
                        sibling->Left->isRed = false;
                    
                }
                target->Parent->isRed=false;
                RightRotate(target->Parent);
                target=Root;
            }
                
        }
        
        }
        if(target!=nullptr)
            target->isRed = false;
    }
    
    void Delete(ll L, ll R) {
        node* target = FindNode(L);
        if(target == nullptr || target->LeftEndPoint != L || target->RightEndPoint != R)
            return;
        
        // Open the file in append mode
        ofstream file("deleted_tasks.txt", ios::app);
        if (file.is_open()) {
            file << "LeftEndPoint: " << target->LeftEndPoint << ", RightEndPoint: " << target->RightEndPoint << "\n";
            file.close();
        } else {
            cout << "Unable to open file";
        }
        
        node* replacement = target, *child = nullptr;
        bool replacement_original_colour = replacement->isRed;
        if (target->Left == nullptr) {
            child = target->Right;
            TransplantNode(target, target->Right);
            UpdateAncestorsMaxRight(target->Parent);
        } else if (target->Right == nullptr) {
            child = target->Left;
            TransplantNode(target, target->Left);
            UpdateAncestorsMaxRight(target->Parent);
        } else {
            replacement = MinimumNode(target->Right);
            replacement_original_colour = replacement->isRed;
            child = replacement->Right;
            if (replacement->Parent == target) {
                if (child != nullptr) 
                    child->Parent = replacement;
            } else {
                TransplantNode(replacement, replacement->Right);
                replacement->Right = target->Right;
                if (replacement->Right != nullptr)
                    replacement->Right->Parent = replacement;
                UpdateAncestorsMaxRight(replacement->Parent);
            }        
            TransplantNode(target, replacement);
            replacement->Left = target->Left;
            if (replacement->Left != nullptr) 
                replacement->Left->Parent = replacement;
            replacement->isRed = target->isRed;
            UpdateAncestorsMaxRight(replacement);
        }
        if (!replacement_original_colour)
            DeleteFix(child);
        delete target;    
    }
    
    void  Inordertraversal(node* r) const
    {
        if(!r)  return;
        Inordertraversal(r->Left);
        cout << "LeftEndPoint:" << r->LeftEndPoint << ' ' << "RightEndPoint:" << r->RightEndPoint << ' '  << "MaxRightEndpoint:" << r->MaxRight << ' ';
        (r->isRed) ? cout << "R" << endl : cout << "B" << endl;
        // cout << "LeftEndPoint:" << r->LeftEndPoint << ' ';
        // (r->isRed) ? cout << "R" << endl : cout << "B" << endl;
        Inordertraversal(r->Right);
    }
   bool doOverlap(ll L1, ll R1, ll L2, ll R2) {
        return L1 <= R2 && L2 <= R1;
    }
     node* GetPriorityInterval(node* curr, ll L, ll R, ll &minRange) {
    if (!curr) return nullptr;

    node* priorityInterval = nullptr;

    if (doOverlap(L, R, curr->LeftEndPoint, curr->RightEndPoint)) {
        ll currRange = curr->RightEndPoint - curr->LeftEndPoint;
        if (currRange < minRange) {
            minRange = currRange;
            priorityInterval = curr;
        }
    }

    // Check left and right children if they might contain overlapping intervals
    if (curr->Left && curr->Left->MaxRight >= L) {
        node* leftPriority = GetPriorityInterval(curr->Left, L, R, minRange);
        if (leftPriority) priorityInterval = leftPriority;
    }
    
    node* rightPriority = GetPriorityInterval(curr->Right, L, R, minRange);
    if (rightPriority) priorityInterval = rightPriority;

    return priorityInterval;
}
    node* overlapSearch(node* root, ll L, ll R) {
        if (!root) {
            return nullptr;
        }
        // Check if the current node’s interval overlaps with the given interval
        if (doOverlap(root->LeftEndPoint, root->RightEndPoint, L, R)) {
            return root;
        }
        // If left child exists and its MaxRight endpoint is greater or equal to the lower bound of the query interval
        if (root->Left && root->Left->MaxRight >= L) {
            return overlapSearch(root->Left, L, R);
        }

        // Recur on the right child otherwise
        return overlapSearch(root->Right, L, R);
    }

public:
    IntervalTree()
    {
        Root = nullptr;
    }
    ~IntervalTree() {}

    void   insert(ll L, ll R)
    {
        Insert(L,R);
    }

    void remove(ll L, ll R)
    {
        Delete(L,R);
    }

    void Inordertraversal()
    {
        Inordertraversal(Root);
    }
    node* getPriorityInterval(ll L, ll R) {
        ll minRange = LLONG_MAX;
        return GetPriorityInterval(Root, L, R, minRange);
    }

  node* searchOverlap(ll L, ll R) {
        return overlapSearch(Root, L, R);
    }
};


int main()
{
    IntervalTree    T;
    //sample input
    T.insert(16,25);
    T.insert(25,30);
    T.insert(14,19);
    T.insert(19,20);
    T.insert(56,70);
    T.insert(13,21);
    T.insert(12,33);

   auto priorityInterval = T.getPriorityInterval(16, 18);
    if (priorityInterval) {
    cout << "Priority Interval: (" << priorityInterval->LeftEndPoint << ", " << priorityInterval->RightEndPoint << ")" << endl;
   } else {
    cout << "No overlapping interval found." << endl;
   }

    cout<<"Before deletion:"<<endl;
    T.Inordertraversal();

    T.remove(19,20);
    T.remove(25,30);

    cout<<"After deletion:"<<endl;
    T.Inordertraversal();

    return 0;
}
