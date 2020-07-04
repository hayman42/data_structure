#include <bits/stdc++.h>
using namespace std;

class AVLTree
{
public:
    int lbalcnt = 0, rbalcnt = 0;
    struct Node
    {
        int value, ldepth = 0, rdepth = 0;
        Node *lchild, *rchild, *parent;
        bool lr;
        Node() { lchild = nullptr, rchild = nullptr; }
        Node(int val, Node *p) : value(val), parent(p) { lchild = nullptr, rchild = nullptr; }
    };

    AVLTree() {}

    void insert(int val)
    {
        insert_node(root, val);
        cnt++;
    }

    int erase(int val)
    {
        select_node(root, val);
        if (flag)
        {
            flag = false;
            return 0;
        }
        else
            return 1;
    }

    int size()
    {
        return cnt;
    }

    Node *root = new Node(~(1 << 31), nullptr);

private:
    int cnt = 0;
    bool flag = false;

    void fix_balance(Node *cur)
    {
        cur->ldepth = (cur->lchild == nullptr ? -1 : max(cur->lchild->ldepth, cur->lchild->rdepth)) + 1;
        cur->rdepth = (cur->rchild == nullptr ? -1 : max(cur->rchild->ldepth, cur->rchild->rdepth)) + 1;
    }

    void rotate_left(Node *cur)
    {
        lbalcnt++;
        Node *tmp = cur->rchild;
        cur->rchild = tmp->lchild;
        if (cur->rchild != nullptr)
            cur->rchild->parent = cur, cur->rchild->lr = false;
        tmp->lchild = cur;
        tmp->parent = cur->parent, tmp->lr = cur->lr;
        if (cur->lr)
            tmp->parent->lchild = tmp;
        else
            tmp->parent->rchild = tmp;
        cur->parent = tmp;
        cur->lr = true;
        fix_balance(cur);
        fix_balance(tmp);
        fix_balance(tmp->parent);
    }

    void rotate_right(Node *cur)
    {
        rbalcnt++;
        Node *tmp = cur->lchild;
        cur->lchild = tmp->rchild;
        if (cur->lchild != nullptr)
            cur->lchild->parent = cur, cur->lchild->lr = true;
        tmp->rchild = cur;
        tmp->parent = cur->parent, tmp->lr = cur->lr;
        if (cur->lr)
            tmp->parent->lchild = tmp;
        else
            tmp->parent->rchild = tmp;
        cur->parent = tmp;
        cur->lr = false;
        fix_balance(cur);
        fix_balance(tmp);
        fix_balance(tmp->parent);
    }

    void balance(Node *cur)
    {
        if (cur->ldepth - cur->rdepth == 2)
        {
            if (cur->lchild->ldepth < cur->lchild->rdepth)
                rotate_left(cur->lchild);
            rotate_right(cur);
        }
        if (cur->ldepth - cur->rdepth == -2)
        {
            if (cur->rchild->rdepth < cur->rchild->ldepth)
                rotate_right(cur->rchild);
            rotate_left(cur);
        }
    }

    void insert_node(Node *cur, int val)
    {
        if (val == cur->value)
            return;
        if (val < cur->value)
        {
            if (cur->lchild == nullptr)
            {
                cur->lchild = new Node(val, cur);
                cur->lchild->lr = true;
            }
            else
                insert_node(cur->lchild, val);

            cur->ldepth = max(cur->lchild->ldepth, cur->lchild->rdepth) + 1;
        }
        if (val > cur->value)
        {
            if (cur->rchild == nullptr)
            {
                cur->rchild = new Node(val, cur);
                cur->rchild->lr = false;
            }
            else
                insert_node(cur->rchild, val);

            cur->rdepth = max(cur->rchild->ldepth, cur->rchild->rdepth) + 1;
        }

        if (cur == root || abs(cur->ldepth - cur->rdepth) < 2)
            return;
        balance(cur);
    }

    void delete_node(Node *cur)
    {
        if (cur->lr)
            cur->parent->lchild = nullptr, cur->ldepth = 0;
        else
            cur->parent->rchild = nullptr, cur->rdepth = 0;
        delete[] cur;
    }

    int select_node(Node *cur, int val)
    {
        if (val == cur->value)
        {
            flag = true;
            if (cur->lchild)
                return cur->value = select_node(cur->lchild, val);
            else if (cur->rchild)
                return cur->value = select_node(cur->rchild, val);
            else
            {
                delete_node(cur);
                return val;
            }
        }

        int ret;
        if (val < cur->value)
        {
            if (cur->lchild == nullptr)
            {
                ret = cur->value;
                if (flag)
                    delete_node(cur);
                return ret;
            }
            else
                ret = select_node(cur->lchild, val);
        }
        if (val > cur->value)
        {
            if (cur->rchild == nullptr)
            {
                ret = cur->value;
                if (flag)
                    delete_node(cur);
                return ret;
            }
            else
                ret = select_node(cur->rchild, val);
        }

        if (cur == root || abs(cur->ldepth - cur->rdepth) < 2)
            return ret;
        balance(cur);
        return ret;
    }
};

void print_elements(AVLTree test)
{
    int j;
    queue<AVLTree::Node *> q;
    q.push(test.root->lchild);
    while (!q.empty())
    {
        int sz = q.size();
        for (j = 0; j < sz; j++)
        {
            AVLTree::Node *cur = q.front();
            cout << cur->value << " ";
            q.pop();
            if (cur->lchild != nullptr)
                q.push(cur->lchild);
            if (cur->rchild != nullptr)
                q.push(cur->rchild);
        }

        cout << "\n";
    }
}

int arr[5000000];
int main()
{
    srand(time(NULL));
    int t = time(NULL);
    int i, j, N = 5000000;
    for (i = 0; i < N; i++)
        arr[i] = i + 1;
    for (i = 0; i < N; i++)
    {
        int tmp = arr[i], j = rand() % N;
        arr[i] = arr[j];
        arr[j] = tmp;
    }
    AVLTree test;
    for (i = 0; i < N; i++)
    {
        int n = arr[i];
        test.insert(n);
    }
    for (i = 0; i < N; i++)
        test.erase(arr[i]);
    cout << time(NULL) - t << "\n"
         << test.lbalcnt << "\n"
         << test.rbalcnt << "\n";

    set<int> s;
    int tt = time(NULL);
    for (i = 0; i < N; i++)
        s.insert(arr[i]);
    for (i = 0; i < N; i++)
        s.erase(arr[i]);
    cout << time(NULL) - tt;
    return 0;
}