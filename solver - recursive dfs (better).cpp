#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n,m;

vector<int> parent;
vector<vector<int>> grid;
vector<vector<int>> vis;
vector<pair<int,int>> colorsAccToFreeMoves;
vector<int> colorX[26];
vector<int> colorY[26];

bool solved = false;

int dr[4] = {-1,0,1,0};
int dc[4] = {0,-1,0,1};

void colorManager(int color);

// Disjoint Set/ Union Find/ Merge Set
void initializeDisjointSet(int countOfNodes) {
    parent.resize(countOfNodes+1);
    for (int i = 0; i <= countOfNodes; i++) {
        parent[i] = i;
    }
}

int findParent(int node) {
    int ultimateParent = node;

    // Finding the ultimate parent
    while (parent[ultimateParent] != ultimateParent) {
        ultimateParent = parent[ultimateParent];
    }

    // Path compression
    while (parent[node] != node) {
        int previousNode = parent[node];
        parent[node] = ultimateParent;
        node = previousNode;
    }

    return ultimateParent;
}

void Union(int x, int y) {
    int px = findParent(x);
    int py = findParent(y);
    parent[px] = parent[py];
}

// Function to swap the values of two variables
void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// Checks if there is a colour which cannot be connected 
bool checkForStuckColors(int ind) {
    bool stuckColorExists = false;

    for (int index = ind+1; index < colorsAccToFreeMoves.size(); index++) {
        pair<int,int> element = colorsAccToFreeMoves[index];

        int color = element.second;

        int colorStartPosX = colorX[color][0];
        int colorStartPosY = colorY[color][0];

        int colorEndPosX = colorX[color][1];
        int colorEndPosY = colorY[color][1];

        vector<vector<int>> checkerGrid(n, vector<int>(m, 0));
        vector<vector<int>> labelGrid(n, vector<int>(m, 0));

        for (int row = 0; row < n; row++) {
            for (int col = 0; col < m; col++) {
                if (vis[row][col] == 0)
                    checkerGrid[row][col] = 1;

                if (row == colorStartPosX && col == colorStartPosY)
                    checkerGrid[row][col] = 1;

                if (row == colorEndPosX && col == colorEndPosY)
                    checkerGrid[row][col] = 1;
            }
        }

        // Intializes the parent vector of Disjoint Set
        initializeDisjointSet(n * m);
        int currentLabel = 0;

        for (int row = 0; row < n; row++) {
            for (int col = 0; col < m; col++) {
                if (checkerGrid[row][col] == 0) continue;
                int top = 0, left = 0;
                if (col-1 >= 0 && checkerGrid[row][col-1] == 1)
                    left = labelGrid[row][col-1];

                if (row-1 >= 0 && checkerGrid[row-1][col] == 1)
                    top = labelGrid[row-1][col];
                
                if (top == 0 && left == 0) {
                    currentLabel++;
                    labelGrid[row][col] = currentLabel;
                }

                if (top != 0 && left == 0) {
                    labelGrid[row][col] = findParent(top);
                }

                if (top == 0 && left != 0) {
                    labelGrid[row][col] = findParent(left);
                }

                if (top != 0 && left != 0) {
                    Union(top, left);
                    labelGrid[row][col] = findParent(left);
                }
            }
        }

        int startParent = findParent(labelGrid[colorStartPosX][colorStartPosY]);
        int endParent = findParent(labelGrid[colorEndPosX][colorEndPosY]);
        
        // Check if the two positions of the color belong to the same component
        if (startParent != endParent) {
            stuckColorExists = true;
            break;
        }
    }

    return stuckColorExists;
}   

// Function which returns true if the current state of the grid is solvable or returns false otherwise
bool performValidityCheck(int ind, int currentColor) {
    if (checkForStuckColors(ind)) {
        return false;
    }

    return true;
}

// Recursive function which traces the path of a color
void solvePuzzle(int row, int col, int ind) {
    int color = colorsAccToFreeMoves[ind].second;
    
    if (solved) {
        return;
    }

    if (ind == colorsAccToFreeMoves.size()) {
        bool ok = (vis != vector<vector<int>>(n, vector<int>(m, 0)));

        if (ok) {
            solved = 1;
        }
        return;
    } 

    int prev = grid[row][col];
    int prevVis = vis[row][col];

    if (colorX[color][1] == row && colorY[color][1] == col) {
        vis[row][col] = 1;
        
        // Move to next color state
            if (ind+1 == colorsAccToFreeMoves.size()) {
                solvePuzzle(-1, -1, ind+1);
            } else {
                int newColor = colorsAccToFreeMoves[ind+1].second;
                int nextRow = colorX[newColor][0];
                int nextCol = colorY[newColor][0];
                solvePuzzle(nextRow, nextCol, ind+1);
            }
        // End


        vis[row][col] = prevVis;
        return;
    }

    vis[row][col] = 1;
    grid[row][col] = color;

    // Checking if the current state of the grid is solvable
    bool isValidState = performValidityCheck(ind, color);
    if (!isValidState) {
        vis[row][col] = prevVis;
        grid[row][col] = prev;
        return;
    }

    // Continuing with current state of the grid
    for (int i = 0; i < 4; i++) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (newRow < n && newCol < m && newRow >= 0 && newCol >= 0) {
            if (vis[newRow][newCol] == 0 || (vis[newRow][newCol] == 2 && grid[newRow][newCol] == color)) {
                solvePuzzle(newRow, newCol, ind);
            }

            if (solved) return;
        }
    }
    
    grid[row][col] = prev;
    vis[row][col] = prevVis;
}

int main() {
    cin >> n >> m;

    grid.resize(n, vector<int>(m, -1));
    vis.resize(n, vector<int>(m, 0));

    // Takes the puzzle from the user
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char color;
            cin >> color;

            if (color == '.') {
                continue;
            }

            int colorCode = color - 'A';

            grid[i][j] = colorCode;
            colorX[colorCode].push_back(i);
            colorY[colorCode].push_back(j);
            vis[i][j] = 2;
        }
    }

    // Returns the free moves available at position (r, c)
    auto checkFreeMoves = [](int r, int c) -> int {
        int freeMoves = 0;
        for (int k = 0; k < 4; k++) {
            int newR = r + dr[k];
            int newC = c + dc[k];
            freeMoves += newR >= 0 && newC >= 0 && newR < n && newC < m && (grid[newR][newC] == -1);
        }
        return freeMoves;
    };

    for (int colorCode = 0; colorCode < 26; colorCode++) {
        if (colorX[colorCode].size() == 0) continue;
        int r1 = colorX[colorCode][0], c1 = colorY[colorCode][0];
        int r2 = colorX[colorCode][1], c2 = colorY[colorCode][1];
        
        int freeMoves1 = checkFreeMoves(r1, c1);
        int freeMoves2 = checkFreeMoves(r2, c2);
        if (freeMoves1 > freeMoves2) {
            swap(colorX[colorCode][0], colorX[colorCode][1]);
            swap(colorY[colorCode][0], colorY[colorCode][1]);
            swap(freeMoves1, freeMoves2);
        }  
        colorsAccToFreeMoves.push_back({freeMoves1, colorCode});
    }

    // sorting in ascending order of free moves
    sort(colorsAccToFreeMoves.begin(), colorsAccToFreeMoves.end()); 

    cout << "\nCalculating...\n";

    int startColor = colorsAccToFreeMoves[0].second;
    int startX = colorX[startColor][0];
    int startY = colorY[startColor][0];
    solvePuzzle(startX, startY, 0);

    if (!solved) {
        cout << "\nNot possible to solve the given puzzle :(\n";
        return 0;
    } 
    
    cout << "\nYes it is possible to solve the puzzle as follows :)\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char color = grid[i][j] + 'A';
            cout << color << " ";
        }
        cout << "\n";
    }

    return 0;
}