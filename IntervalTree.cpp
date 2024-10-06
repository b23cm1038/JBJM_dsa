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

    void Inordertraversal()
    {
        Inordertraversal(Root);
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

    T.Inordertraversal();

    return 0;
}