#include<bits/stdc++.h>
#include"C:\Users\varap\OneDrive\Desktop\dsa project test\cpp-httplib\httplib.h"
#include<mutex>

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

int n, cnt = 0;

//using RB tree to implement IntervalTree
class IntervalTree
{
private:
    class node
    {
    public:
        ll LeftEndPoint, RightEndPoint, MaxRight;
        int Priority; 
        bool isRed;
        int id;
        node *Parent, *Left, *Right;

        node(ll L, ll R, int preference, int j) 
            : LeftEndPoint(L), RightEndPoint(R), Priority(preference), MaxRight(R), 
              Parent(nullptr), Left(nullptr), Right(nullptr), isRed(true),id(j)
        {
            
        }
    };

    node* CreateNode(ll L, ll R,int preference,int j)
    {
        return new node(L,R,preference,j);
    }

    node* Root;

    void Insert(ll L, ll R,int preference,int j)
    {
        //simple BST insertion by comparing leftendpoints
        node* x = Root;
        node* y = nullptr;
        node* z = CreateNode(L,R,preference,j);

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
        cout<<r->id<<' ';

        (r->isRed) ? cout << "R" << endl : cout << "B" << endl;
        // cout << "LeftEndPoint:" << r->LeftEndPoint << ' ';
        // (r->isRed) ? cout << "R" << endl : cout << "B" << endl;
        Inordertraversal(r->Right);
    }

    void InorderTraversalToString(node* r, stringstream& ss) const
    {
        if (!r) return;

        InorderTraversalToString(r->Left, ss);
        
    ss << "ID: " << r->id 
       << " | Interval: [" << r->LeftEndPoint << ", " << r->RightEndPoint << "]"
       << " | Priority: " << r->Priority << "\n";

        InorderTraversalToString(r->Right, ss);
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

    mutex treeMutex;

    IntervalTree()
    {
        Root = nullptr;
    }
    ~IntervalTree() {}

    void   insert(ll L, ll R,int preference)
    {
        cnt++;
        lock_guard<mutex> guard(treeMutex);
        cnt > n ? usingCheckAndModify(L,R,preference,n) : Insert(L,R,preference,cnt);
    }

    string getInorderIntervals()
    {
        lock_guard<mutex> guard(treeMutex);
        stringstream ss;
        InorderTraversalToString(Root, ss);
        return ss.str();
    }

    void StartServer()
    {
        httplib::Server svr;

        // Endpoint to insert an interval
        svr.Post("/insert", [this](const httplib::Request& req, httplib::Response& res) {
            ll L = stoll(req.get_param_value("L"));
            ll R = stoll(req.get_param_value("R"));
            ll Priority = stoll(req.get_param_value("Priority"));
            //ll id = stoll(req.get_param_value("id"));
            //change needed here for insert function parameters!!!
            this->insert(L, R, Priority);

            res.set_content("Interval inserted successfully\n", "text/plain");
        });

         svr.Post("/delete", [this](const httplib::Request& req, httplib::Response& res) {
            ll L1 = stoll(req.get_param_value("L1"));
            ll R1 = stoll(req.get_param_value("R1"));
            
            this->remove(L1, R1);

            res.set_content("Interval deleted successfully\n", "text/plain");
         });

         svr.Post("/Modify", [this](const httplib::Request& req, httplib::Response& res) {
            ll L2 = stoll(req.get_param_value("L2"));
            ll R2 = stoll(req.get_param_value("R2"));
            ll L3 = stoll(req.get_param_value("L3"));
            ll R3 = stoll(req.get_param_value("R3"));
            int P = stoll(req.get_param_value("P"));
            
            this->modify(L2, R2, L3, R3, P);

            res.set_content("Interval modified successfully\n", "text/plain");
         });

        

        svr.Get("/get_inorder", [this](const httplib::Request& req, httplib::Response& res) {
            res.set_content(this->getInorderIntervals(), "text/plain");
        });

            svr.Post("/set_n", [](const httplib::Request& req, httplib::Response& res) {
            if (req.has_param("n")) {
                n = std::stoi(req.get_param_value("n"));
                res.set_content("Number of employees set to " + std::to_string(n) + "\n", "text/plain");
            } else {
                res.status = 400;
                res.set_content("Missing parameter 'n'\n", "text/plain");
            }
            });

        cout << "Server started at http://localhost:8080" << endl;
        svr.listen("localhost", 8080);
    }

    void remove(ll L, ll R)
    {
        //Inordertraversal(Root);
        lock_guard<mutex> guard(treeMutex);
        Delete(L,R);
        //Inordertraversal(Root);
    }

    void Inordertraversal()
    {
        lock_guard<mutex> guard(treeMutex);
        Inordertraversal(Root);
    }
    node* getPriorityInterval(ll L, ll R) {
        ll minRange = LLONG_MAX;
        return GetPriorityInterval(Root, L, R, minRange);
    }

  node* searchOverlap(ll L, ll R) {
        return overlapSearch(Root, L, R);
    }
  void checkAndModifyInterval(node*curr,ll L, ll R, vector<bool>&overlapArray,vector<node*>&overlappingnodes,int priority,int n){
      if(!curr) return;
      
      if (doOverlap(L, R, curr->LeftEndPoint, curr->RightEndPoint)){
        node* overlapNode = curr;  
         overlapArray[overlapNode->id - 1] = true;
        if(overlapNode->Priority<priority){   
                overlappingnodes.push_back(overlapNode);
        }

        
      }
      if (curr->Left && curr->Left->MaxRight >= L){
         checkAndModifyInterval(curr->Left,L,R,overlapArray,overlappingnodes,priority,n); 
      }
      checkAndModifyInterval(curr->Right,L,R,overlapArray,overlappingnodes,priority,n);
      return;
  }
 
 void usingCheckAndModify(ll L, ll R,int priority,int n){
     vector<bool>overlapArray(n,false);
     vector<node*>overlappingnodes;
     checkAndModifyInterval(this->Root,L,R,overlapArray,overlappingnodes,priority,n);
     for (int i = 0; i < n; i++) {
        if (!overlapArray[i]) {
            // If a corresponding value is still false, insert a new node for that
            Insert(L, R, priority, i + 1);
            
            cout << "Inserting new interval: [" << L << ", " << R << "] with priority "<< priority<<" for worker ";
            cout<<i+1;
            cout << endl;

            return;
        }
    }
    if(!overlappingnodes.empty()){
        sort(overlappingnodes.begin(), overlappingnodes.end(),[](node* a, node* b){return a->Priority < b->Priority;});

       auto overlapNode= overlappingnodes[0];
        
        std::cout << "Deleting lower-priority node: "
                            << "LeftEndPoint = " << overlapNode->LeftEndPoint
                            << ", RightEndPoint = " << overlapNode->RightEndPoint << " worker ";
            cout <<  overlapNode->id << endl;
            Insert(L,R,priority,overlapNode->id);
            Delete(overlapNode->LeftEndPoint,overlapNode->RightEndPoint);
            return;
        
    
    
    }
    else{
        cout<<"This interval cannot be inserted"<<endl;
        return;
    }
    
 } 

 void modify(ll left,ll right, ll L ,ll R,int p){
    // node* rescheduling = FindNode(left);
    // Insert(L,R,p,cnt);
    // Delete(left,right);
    return;
 } 
};

int main()
{
    IntervalTree    T;
    T.StartServer();
    //sample input
    //int n = 5;
    //svr.Post("/set_n", set_n_handler);
    
    // T.insert(91,100,1,1);
    // T.insert(92,100,2,2);
    // T.insert(93,100,2,3);
    // T.insert(94,100,0,4);
    // T.insert(95,100,1,5);

    // cout<<"Before Adding new intervals:"<<endl;
    // T.Inordertraversal();
    // T.usingCheckAndModify(101,110,n);
    // T.usingCheckAndModify(102,104,n);
    // T.usingCheckAndModify(89,92,n);
    // cout<<"After addition:"<<endl;
    // T.Inordertraversal();

    return 0;
}

// int main() {
//     IntervalTree T;

//     // Sample input - Insert initial intervals with start, end, priority, and task ID
//     int n = 5;
//     T.insert(91, 100, 1, 1);  // Interval 1: [91,100] with priority 1 and task ID 1
//     T.insert(92, 100, 2, 2);  // Interval 2: [92,100] with priority 2 and task ID 2
//     T.insert(93, 100, 2, 3);  // Interval 3: [93,100] with priority 2 and task ID 3
//     T.insert(94, 100, 0, 4);  // Interval 4: [94,100] with priority 0 and task ID 4
//     T.insert(95, 100, 1, 5);  // Interval 5: [95,100] with priority 1 and task ID 5

//     // Display the intervals before adding new intervals
//     cout << "Before Adding new intervals:" << endl;
//     T.Inordertraversal();  // Assuming this function prints the intervals

//     // Adding new intervals with `usingCheckAndModify`, now with priority as the third parameter
//     T.usingCheckAndModify(101, 110, 1, n);  // New interval [101,110] with priority 1
//     T.usingCheckAndModify(102, 104, 2, n);  // New interval [102,104] with priority 2
//     T.usingCheckAndModify(89, 92, 1, n);   // New interval [89,92] with priority 1

//     // Display the intervals after adding new intervals
//     cout << "After addition:" << endl;
//     T.Inordertraversal();  // Assuming this function prints the updated intervals

//     return 0;
// }
