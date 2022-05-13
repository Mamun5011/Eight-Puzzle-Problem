#include <bits/stdc++.h>
using namespace std;
#define puzzleDimension 3


struct node
{
    /// stores state
    int state[puzzleDimension][puzzleDimension];

    /// empty_x and empty_y denoted blank tile coOrdinates, G_N denotes the number of  misplaced  tiles, depth denotes moves so far
    int empty_x, empty_y,G_N,H_N, F_N, depth;

    /// stores parent state node of current state
    node* parent;

    node()
    {

        parent=NULL;
        F_N=INT_MAX;
        G_N=0;
        H_N=INT_MAX;
        depth=0;
    }


    void calculateTotalCost()
    {
        F_N= G_N + H_N;
    }


};


struct compareAndSortTile
{
    bool operator()(const node a, const node b) const
    {
        return (a.F_N + a.depth) > (b.F_N + b.depth);
    }
};


struct compareUniformDistance
{
    bool operator()(const node a, const node b) const
    {
        return a.G_N  > b.G_N;
    }
};



class EightPuzzle
{
    int initial_state_blank_Tile_X;
    int initial_state_blank_Tile_Y;
    int depth=-INT_MAX;
    int maxNodeVisited=0;
    enum heuristic { Misplaced_Tile, Manhattan_Distance, Uniform_Cost};
    ///  This is Final State where Value 0 denotes empty space
    int finalState[puzzleDimension][puzzleDimension] =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    /// This is Initial State where Value 0 denotes empty space in the puzzle
    int initalState[puzzleDimension][puzzleDimension] =
    {
        {0, 1, 3},
        {4, 2, 6},
        {7, 5, 8}
    };

public:
    EightPuzzle()
    {
        depth=-INT_MAX;
        maxNodeVisited=0;
        initial_state_blank_Tile_X=0;
        initial_state_blank_Tile_Y=0;

    }

///Print the state at any time stored into Matrix form

    void printPuzzleState(int a[puzzleDimension][puzzleDimension])
    {
        for (int i = 0; i < puzzleDimension; i++)
        {
            for (int j = 0; j < puzzleDimension; j++)cout<<a[i][j]<<" ";
            cout<<endl;
        }

    }


    /// This Function calculate the the number of misplaced tile ( number of non-blank tiles not in their goal position)

    int misPlacedTilesCost(int initialState[puzzleDimension][puzzleDimension], int finalState[puzzleDimension][puzzleDimension])
    {
        int cost = 0;
        for (int i = 0; i < puzzleDimension; i++)
            for (int j = 0; j < puzzleDimension; j++)
                if (initialState[i][j]!=0 && initialState[i][j] != finalState[i][j])cost++;


        return cost;
    }

///Calculates the difference of row and column for a tile mismatch for the manhattanDistanceCost function

    int findRowAndColumnDifferene(int i, int j, int finalState[puzzleDimension][puzzleDimension], int num)
    {
        for (int a = 0; a < puzzleDimension; a++)
            for (int b = 0; b < puzzleDimension; b++)
                if(finalState[a][b]==num) return abs(i-a)+abs(j-b);
    }

/// This Function calculate the manhattan distance of each tile ( difference of row and column for a tile mismatch)

    int manhattanDistanceCost(int initialState[puzzleDimension][puzzleDimension], int finalState[puzzleDimension][puzzleDimension])
    {
        int cost = 0;
        for (int i = 0; i < puzzleDimension; i++)
            for (int j = 0; j < puzzleDimension; j++)
                if (initialState[i][j]!=0)cost+=findRowAndColumnDifferene(i,j,finalState,initialState[i][j]);
        return cost;
    }

/// This Function calculate the uniform cost (uses the misplace tiles count as a uniform cost)

    int calculateUniformCost(int initialState[puzzleDimension][puzzleDimension], int finalState[puzzleDimension][puzzleDimension])
    {
        int cost = 0;
        for (int i = 0; i < puzzleDimension; i++)
            for (int j = 0; j < puzzleDimension; j++)
                if (initialState[i][j]!=0 && initialState[i][j] != finalState[i][j])cost++;


        return cost;
    }


    node expandChild(int x, int y, node s,int finalState[puzzleDimension][puzzleDimension], heuristic mode)
    {
        node v;
        /// set the parent of current state and copy all necessary values to new state which will be explored further again in future if it is necessary
        memcpy(v.state, s.state, sizeof v.state);
        v.empty_x = s.empty_x;
        v.empty_y = s.empty_y;
        v.parent=new node(s);
        int temp = v.state[v.empty_x][v.empty_y];
        v.state[v.empty_x][v.empty_y]=v.state[x][y];
        v.state[x][y] = temp;
        v.empty_x=x;
        v.empty_y=y;
        v.depth=s.depth+1;
        v.G_N= s.G_N+ 1;

        if(mode==Misplaced_Tile)
        {
            v.H_N = misPlacedTilesCost(v.state,finalState);
        }
        else if(mode==Uniform_Cost)
        {
            v.G_N=manhattanDistanceCost(v.state,finalState);
            v.H_N = 0;
        }
        else if(mode==Manhattan_Distance)
        {
            v.H_N = manhattanDistanceCost(v.state,finalState);
        }

        v.calculateTotalCost();

        return v;
    }


    void A_Star_Search(int initialState[puzzleDimension][puzzleDimension], int finalState[puzzleDimension][puzzleDimension], heuristic mode)
    {
        priority_queue<node, std::vector<node>, compareAndSortTile> Q1;  /// Queue for Misplaced Tile heuristic
        priority_queue<node, std::vector<node>, compareUniformDistance> Q2; /// Queue for Uniform Cost Search
        priority_queue<node, std::vector<node>, compareAndSortTile> Q3; /// Queue for Manhattan Distance heuristic

        int nodeVisited=0;
        int maxQueueSIze = 0;
        bool QUEUE_NOT_EMPTY=true;
        ///first state to entry in the Queue
        node startState;
        ///Copy the initial state
        memcpy(startState.state, initialState, sizeof startState.state);
        /// set the Coordinate of blank Tiles
        startState.empty_x=initial_state_blank_Tile_X;
        startState.empty_y = initial_state_blank_Tile_Y;
        /// Find the heuristic Value
        if(mode==Misplaced_Tile)
        {
            startState.H_N = misPlacedTilesCost(initialState,finalState);
        }
        else if(mode==Uniform_Cost)
        {
            startState.H_N = 0;
        }
        else if(mode==Manhattan_Distance)
        {
            startState.H_N = manhattanDistanceCost(initialState,finalState);
        }

        ///Inserting the very first state into the Queue

        startState.calculateTotalCost();
        if(mode==Misplaced_Tile)Q1.push(startState);
        else if(mode==Uniform_Cost)Q2.push(startState);
        else if(mode==Manhattan_Distance)Q3.push(startState);
        else
        {
            cout<<"Wrong heuristic given, please provide Misplaced Tile , Uniform Cost or Manhattan Distance heuristic"<<endl;
            return;
        }



        while(QUEUE_NOT_EMPTY)
        {
            /// Tracking the max Queue Size

            if(mode==Misplaced_Tile)maxQueueSIze=maxQueueSIze<Q1.size()?Q1.size():maxQueueSIze;
            else if(mode==Uniform_Cost)maxQueueSIze=maxQueueSIze<Q2.size()?Q2.size():maxQueueSIze;
            else if(mode==Manhattan_Distance)maxQueueSIze=maxQueueSIze<Q3.size()?Q3.size():maxQueueSIze;

            nodeVisited++;
            node CuroptimalState;

            /// Extract the next state and pop the queue
            if(mode==Misplaced_Tile)
            {
                CuroptimalState =  Q1.top();
                Q1.pop();
            }
            else if(mode==Uniform_Cost)
            {
                CuroptimalState =  Q2.top();
                Q2.pop();
            }
            else if(mode==Manhattan_Distance)
            {
                CuroptimalState =  Q3.top();
                Q3.pop();
            }

            cout<<"\nThe best state to expand with g(n)= "<<CuroptimalState.G_N<<" h(n)= "<<CuroptimalState.H_N<<endl;
            printPuzzleState(CuroptimalState.state);

             ///Misplaced Tiles or Mnahattan distance terminating condition for goal state

            if ((mode==Misplaced_Tile || mode==Manhattan_Distance)&& CuroptimalState.H_N == 0)
            {
                cout<<"\n\nGoal State !\n\n";
                cout<<"Solution depth was  "<<CuroptimalState.depth<<endl;
                cout<<"Number of Nodes Expanded: "<<nodeVisited<<endl;
                cout<<"Max queue size "<<maxQueueSIze<<"\n\n\n";
                return;
            }

            ///Uniform cost terminating condition for goal state

            else if(mode==Uniform_Cost && manhattanDistanceCost(CuroptimalState.state,finalState)==0 )
            {
                cout<<"\n\nGoal State !\n\n";
                cout<<"Solution depth was  "<<CuroptimalState.depth<<endl;
                cout<<"Number of Nodes Expanded: "<<nodeVisited<<endl;
                cout<<"Max queue size "<<maxQueueSIze<<"\n\n\n";
                return;

            }

            ///Move right cell for expanding based on the heuristics

            if(CuroptimalState.empty_x+1>=0 && CuroptimalState.empty_x+1<puzzleDimension)
            {
                node rightchild=expandChild(CuroptimalState.empty_x+1, CuroptimalState.empty_y,CuroptimalState,finalState,mode);
                if(mode==Misplaced_Tile)Q1.push(rightchild);
                else if(mode==Uniform_Cost)Q2.push(rightchild);
                else if(mode==Manhattan_Distance)Q3.push(rightchild);

            }



            /// Move bottom cell for expanding based on the heuristics

            if(CuroptimalState.empty_y-1>=0 && CuroptimalState.empty_y-1<puzzleDimension)
            {
                node bottomchild=expandChild(CuroptimalState.empty_x, CuroptimalState.empty_y-1,CuroptimalState,finalState,mode);
                if(mode==Misplaced_Tile)Q1.push(bottomchild);
                else if(mode==Uniform_Cost)Q2.push(bottomchild);
                else if(mode==Manhattan_Distance)Q3.push(bottomchild);

            }

            /// Move left sell for expanding based on the heuristics

            if(CuroptimalState.empty_x-1>=0 && CuroptimalState.empty_x-1<puzzleDimension)
            {
                node leftchild=expandChild(CuroptimalState.empty_x-1, CuroptimalState.empty_y,CuroptimalState,finalState,mode);
                if(mode==Misplaced_Tile)Q1.push(leftchild);
                else if(mode==Uniform_Cost)Q2.push(leftchild);
                else if(mode==Manhattan_Distance)Q3.push(leftchild);
            }


            /// Move up cell for expanding based on the heuristics

            if(CuroptimalState.empty_y+1>=0 && CuroptimalState.empty_y+1<puzzleDimension)
            {
                node upchild=expandChild(CuroptimalState.empty_x, CuroptimalState.empty_y+1,CuroptimalState,finalState,mode);
                if(mode==Misplaced_Tile)Q1.push(upchild);
                else if(mode==Uniform_Cost)Q2.push(upchild);
                else if(mode==Manhattan_Distance)Q3.push(upchild);
            }

            /// If Queue is empty and we haven't found goal state then it returns failure

            if(mode==Misplaced_Tile)QUEUE_NOT_EMPTY=Q1.size()==0?false:true;
            else if(mode==Uniform_Cost)QUEUE_NOT_EMPTY=Q2.size()==0?false:true;
            else if(mode==Manhattan_Distance)QUEUE_NOT_EMPTY=Q3.size()==0?false:true;

        }

    }


    void takeinput()
    {
        int initalState[puzzleDimension][puzzleDimension];

        cout<<"Enter the first row:";

        for(int i=0; i<puzzleDimension; i++)
        {
            cin>>initalState[0][i];
            if(initalState[0][i]==0)
            {
                initial_state_blank_Tile_X= 0;
                initial_state_blank_Tile_Y=i;
            }
        }

        cout<<"\nEnter the second row:";

        for(int i=0; i<puzzleDimension; i++)
        {
            cin>>initalState[1][i];
            if(initalState[1][i]==0)
            {
                initial_state_blank_Tile_X= 1;
                initial_state_blank_Tile_Y=i;
            }
        }
        cout<<"\nEnter the third row:";

        for(int i=0; i<puzzleDimension; i++)
        {
            cin>>initalState[2][i];
            if(initalState[2][i]==0)
            {
                initial_state_blank_Tile_X= 2;
                initial_state_blank_Tile_Y=i;
            }
        }

    }

    void solvePuzzle()
    {
        bool inputTaken=false;
        int n;

        // while(!inputTaken)
        while(1)
        {
            cout<<"Welcome to my 8-Puzzle Solver.\n\nType '1' to use a default puzzle, or '2' to create your own"<<endl;
            cin>>n;
            if(n==1)
            {
                cout<<"initial default state has been loaded"<<endl;
                inputTaken=true;
                cout<<"Your initial state: "<<endl;
                printPuzzleState(initalState);
                cout<<"Your Goal state: "<<endl;
                printPuzzleState(finalState);
                cout<<"Default Search:  A* Search, Default Heuristic: Misplaced Tiles"<<endl;
                A_Star_Search(initalState,finalState,Misplaced_Tile);
            }

            else if(n==2)
            {
                /// This is Initial State set by the user where Value 0 denotes empty space in the puzzle

                cout<<"Enter your puzzle, using a zero to represent the blank. Please only enter valid 8-puzzles. \nEnter the puzzle demilimiting the numbers with a space. Type RETURN only when finished.\n\n"<<endl;

                takeinput();
                inputTaken=true;
                cout<<"Your initial state: "<<endl;
                printPuzzleState(initalState);
                cout<<"Your Goal state: "<<endl;
                printPuzzleState(finalState);

                cout<<"Select algorithm. \nPress.....\n (1) for Uniform Cost Search,\n (2) for the Misplaced Tile Heuristic, or\n (3) the Manhattan Distance Heuristic"<<endl;
                int val;
                cin>>val;
                if(val==1)A_Star_Search(initalState,finalState,Uniform_Cost);
                else if(val==2)A_Star_Search(initalState,finalState,Misplaced_Tile);
                else if(val==3)A_Star_Search(initalState,finalState,Manhattan_Distance);
                else
                    cout<<"You have to press between 1 and 3"<<endl;


            }

            else
                cout<<"Invalid input, please press 1 or 2"<<endl;

        }
    }

};

int main()
{
    ///Create a object of eight puzzle problem
    EightPuzzle P;
    /// Call the method of the object
    P.solvePuzzle();
    return 0;
}
