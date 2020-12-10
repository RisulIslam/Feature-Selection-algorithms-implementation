// Program to print path from root node to destination node
// for N*N -1 puzzle algorithm using Branch and Bound
// The solution assumes that instance of puzzle is solvable
#include <bits/stdc++.h>
#include <stdlib.h>
#include <ctime>
using namespace std;
#define N 3

// state space tree nodes
int level=-1;
int maxexpand=0;
struct Node
{
    // stores parent node of current node
    // helps in tracing path when answer is found
    Node* parent;

    // stores matrix
    int mat[N][N];

    // stores blank tile cordinates
    int x, y;

    // stores the number of misplaced tiles
    int gn;

    int cost;
    int fn;

    // stores the number of moves so far
    int level;
};

// Function to print N x N matrix
int printMatrix(int mat[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

// Function to allocate a new node
Node* newNode(int mat[N][N], int x, int y, int newX,
              int newY, int level, Node* parent,int gn)
{
    Node* node = new Node;

    // set pointer for path to root
    node->parent = parent;

    // copy data from parent node to current node
    memcpy(node->mat, mat, sizeof node->mat);

    // move tile by 1 postion
    swap(node->mat[x][y], node->mat[newX][newY]);

    // set number of misplaced tiles
    node->cost = INT_MAX;

    // set number of moves so far
    node->level = level;

    // update new blank tile cordinates
    node->x = newX;
    node->y = newY;
    node->gn=gn;

    return node;
}

// botton, left, top, right
int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

// Function to calculate the the number of misplaced tiles
// ie. number of non-blank tiles not in their goal position
int calculateCostMT(int initial[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (initial[i][j] && initial[i][j] != final[i][j])
           count++;
    return count;
}
int calculateCostMD(int initial[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (initial[i][j])
        {
            for (int k = 0; k < N; k++)
                for (int l = 0; l < N; l++)
                {
                    if(final[k][l]==initial[i][j])
                    {
                        count+=abs(i-k)+abs(j-l);
                    }
                }

        }

    return count;
}

// Function to check if (x, y) is a valid matrix cordinate
int isSafe(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// print path from root node to destination node
void printPath(Node* root)
{
    if (root == NULL)
        return;
    level++;
    printPath(root->parent);
    printMatrix(root->mat);

    printf("\n");
}

// Comparison object to be used to order the heap
struct compMT
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->fn + lhs->level) > (rhs->fn + rhs->level);
    }
};
struct compU
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->gn ) > (rhs->gn);
    }
};

// Function to solve N*N - 1 puzzle algorithm using
// Branch and Bound. x and y are blank tile coordinates
// in initial state
void solveMT(int initial[N][N], int x, int y,
           int final[N][N])
{
    // Create a priority queue to store live nodes of
    // search tree;
    maxexpand=0;
    priority_queue<Node*, std::vector<Node*>, compMT> pq;

    // create a root node and calculate its cost
    Node* root = newNode(initial, x, y, x, y, 0, NULL,0);// last one is g(n)
    root->cost = calculateCostMT(initial, final);
    root->fn=root->cost+root->gn;

    // Add root to list of live nodes;
    pq.push(root);


    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    int maxi=-1000;
    while (!pq.empty())
    {
        // Find a live node with least estimated cost
        Node* min = pq.top();

        // The found node is deleted from the list of
        // live nodes

        // Max size of Queue

        int s=pq.size();
        printf("queue size: %d",s);
        if(s > maxi)
        {

            maxi = pq.size();
            printf("max queue size: %d",maxi);
        }


        pq.pop();
        printf("\nThe best Expanding with g(n)=%d cost=%d f(n)= %d\n",min->gn,min->cost,min->fn);
        printMatrix(min->mat);
        maxexpand++;

        // if min is an answer node
        if (min->cost == 0)
        {
            // print the path from root to destination;
            printf("\n\nGOAL !!!!!\n");
            printf("Solution depth: %d\n",min->level);
            printf("Number of Nodes Expanded: %d\n",maxexpand);
            printf("Max Queue Size at any time: %d",maxi);
            //printf("The final Solution is: \n");
            //printPath(min);
            return;
        }
        if(maxexpand>50000)
        {

            printf("\nNot Solvable! \n");
            return;
        }

        // do for each child of min
        // max 4 children for a node
        for (int i = 0; i < 4; i++)
        {
            if (isSafe(min->x + row[i], min->y + col[i]))
            {
                // create a child node and calculate
                // its cost
                Node* child = newNode(min->mat, min->x,
                              min->y, min->x + row[i],
                              min->y + col[i],
                              min->level + 1, min,min->gn+1);
                //child->gn=min->gn++;
                child->cost = calculateCostMT(child->mat, final);
                child->fn=child->cost+child->gn;

                // Add child to list of live nodes
                pq.push(child);
            }
        }
    }
}

void solveU(int initial[N][N], int x, int y,
           int final[N][N])
{
    // Create a priority queue to store live nodes of
    // search tree;
    maxexpand=0;
    priority_queue<Node*, std::vector<Node*>, compU> pq;

    // create a root node and calculate its cost
    Node* root = newNode(initial, x, y, x, y, 0, NULL,0);// last one is g(n)
    root->cost = calculateCostMT(initial, final);
    //root->fn=root->cost+root->gn;

    // Add root to list of live nodes;
    pq.push(root);


    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    int maxi=-1000;
    while (!pq.empty())
    {
        // Find a live node with least estimated cost
        Node* min = pq.top();

        // The found node is deleted from the list of
        // live nodes
        int s=pq.size();
        printf("queue size: %d",s);
        if(s > maxi)
        {

            maxi = pq.size();
            printf("max queue size: %d",maxi);
        }

        pq.pop();
        printf("\nThe best Expanding with g(n)=%d \n",min->gn);
        printMatrix(min->mat);
        maxexpand++;

        // if min is an answer node
        if (min->cost == 0) // Goal Test for Uniform cost search
        {
            // print the path from root to destination;
            printf("\n\nGOAL !!!!!\n");
            printf("Solution depth: %d\n",min->level);
            printf("Number of Nodes Expanded: %d\n",maxexpand);
            printf("Max Queue Size at any time: %d",maxi);
            //printf("The final Solution is: \n");
            //printPath(min);
            return;
        }
        if(maxexpand>50000)
        {

            printf("\nNot Solvable! \n");
            return;
        }

        // do for each child of min
        // max 4 children for a node
        for (int i = 0; i < 4; i++)
        {
            if (isSafe(min->x + row[i], min->y + col[i]))
            {
                // create a child node and calculate
                // its cost
                Node* child = newNode(min->mat, min->x,
                              min->y, min->x + row[i],
                              min->y + col[i],
                              min->level + 1, min,min->gn+1);
                //child->gn=min->gn++;
                child->cost = calculateCostMT(child->mat, final);
                //child->fn=child->cost+child->gn;

                // Add child to list of live nodes
                pq.push(child);
            }
        }
    }
}

void solveMD(int initial[N][N], int x, int y,
           int final[N][N])
{
    // Create a priority queue to store live nodes of
    // search tree;
    maxexpand=0;
    priority_queue<Node*, std::vector<Node*>, compMT> pq;

    // create a root node and calculate its cost
    Node* root = newNode(initial, x, y, x, y, 0, NULL,0);// last one is g(n)
    root->cost = calculateCostMD(initial, final);
    root->fn=root->cost+root->gn;

    // Add root to list of live nodes;
    pq.push(root);


    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    int maxi=-1000;
    while (!pq.empty())
    {
        // Find a live node with least estimated cost
        Node* min = pq.top();

        // The found node is deleted from the list of
        // live nodes

        int s=pq.size();
        printf("queue size: %d",s);
        if(s > maxi)
        {

            maxi = pq.size();
            printf("max queue size: %d",maxi);
        }

        pq.pop();
        printf("\nThe best Expanding with g(n)=%d h(n)=%d f(n)= %d\n",min->gn,min->cost,min->fn);
        printMatrix(min->mat);
        maxexpand++;

        // if min is an answer node
        if (min->cost == 0)
        {
            // print the path from root to destination;
            printf("\n\nGOAL !!!!!\n");
            printf("Solution depth: %d\n",min->level);
            printf("Number of Nodes Expanded: %d\n",maxexpand);
            printf("Max Queue Size at any time: %d",maxi);
            //printf("The final Solution is: \n");
            //printPath(min);
            return;
        }
        /*
        if(maxexpand>50000)
        {

            printf("\nNot Solvable! \n");
            return;
        }
        */

        // do for each child of min
        // max 4 children for a node
        for (int i = 0; i < 4; i++)
        {
            if (isSafe(min->x + row[i], min->y + col[i]))
            {
                // create a child node and calculate
                // its cost
                Node* child = newNode(min->mat, min->x,
                              min->y, min->x + row[i],
                              min->y + col[i],
                              min->level + 1, min,min->gn+1);
                //child->gn=min->gn++;
                child->cost = calculateCostMD(child->mat, final);
                child->fn=child->cost+child->gn;

                // Add child to list of live nodes
                pq.push(child);
            }
        }
    }
}

// Driver code
int main()
{
    // Initial configuration
    // Value 0 is used for empty space
    int initial[N][N] =
    {
        {0, 1, 3},
        {4, 2, 6},
        {7, 5, 8}
    };

    // Solvable Final configuration
    // Value 0 is used for empty space
    int final[N][N] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    // Blank tile coordinates in initial
    // configuration
    printf("Default Initial State: \n");
    printMatrix(initial);
    printf("Default Goal State: \n");
    printMatrix(final);
    while(1)
    {
        int inpchg;
        printf("\n\n\nPress 1 for default/current puzzle Or Press 2 to enter your own puzzle : ");
        scanf("%d",&inpchg);
        if(inpchg==2)
        {
            printf("Enter your puzzle, use a zero to represent the blank.\n");
            printf("\n\nStarting Entering the Initial State.... \n\n");
            for (int k = 0; k < N; k++)
            {
                printf("Enter row %d, use space between numbers: ",k+1);
                scanf("%d %d %d",&initial[k][0],&initial[k][1],&initial[k][2]);
            }
            printf("\n\nStarting Entering the Goal State.... \n\n");
            for (int k = 0; k < N; k++)
            {
                printf("Enter row %d, use space between numbers: ",k+1);
                scanf("%d %d %d",&final[k][0],&final[k][1],&final[k][2]);
            }


        }
        printf("\nStarting with initial state: \n");
        printMatrix(initial);
        printf("\nStarting with Goal state: \n");
        printMatrix(final);


         int blankx,blanky;
         for (int k = 0; k < N; k++)
            for (int l = 0; l < N; l++)
            {
                if(initial[k][l]==0)
                {
                    blankx=k;
                    blanky=l;
                    break;
                }
            }
        printf("\nStarting Initial Blank is in: %d %d\n",blankx,blanky);
        int choice;
        printf("\nEnter your choice of algorithm. \n\t1. Uniform Cost Search \n\t2. A* with the Misplaced Tile heuristic. \n\t3. A* with the Manhattan distance heuristic.\n\t4. ALL Algorithms. \nState Your choice number: ");
        scanf("%d",&choice);
        while(1)
        {
            if(choice==1)
            {
                printf("Your Algorithm Choice: Uniform Cost Search\n");
                solveU(initial, blankx, blanky, final);
                break;
            }
            else if(choice==2)
            {
                printf("Your Algorithm Choice: A* Search with Misplaced Tiles Heuristic.\n");
                solveMT(initial, blankx, blanky, final);
                break;
            }
            else if(choice==3)
            {
                printf("Your Algorithm Choice: A* Search with Manhattan Distance Heuristic.\n");
                solveMD(initial, blankx, blanky, final);
                break;
            }
            else if(choice==4){
                printf("Your Algorithm Choice: ALL\n");
                int stime,etime;
                stime=clock();
                solveU(initial, blankx, blanky, final);
                etime=clock();
                cout<<"\nExecution Time UCS: "<<(etime-stime)/(double(CLOCKS_PER_SEC)*1000)<<endl;


                stime=clock();
                solveMT(initial, blankx, blanky, final);
                etime=clock();
                cout<<"\nExecution Time for AMT: "<<(etime-stime)/(double(CLOCKS_PER_SEC)*1000)<<endl;

                stime=clock();
                solveMD(initial, blankx, blanky, final);
                etime=clock();
                cout<<"\nExecution Time for AMD: "<<(etime-stime)/(double(CLOCKS_PER_SEC)*1000)<<endl;
                break;
            }
            else
            {
                printf("Wrong Input! Enter choice number again.\n");
                scanf("%d",&choice);
            }
        }



    }



    return 0;
}
