#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n,m;

vector<vector<int>> grid;
vector<vector<int>> vis;
vector<pair<int,int>> colorsAccToFreeMoves;
vector<int> colorX[26];
vector<int> colorY[26];

bool solved = false;

int dr[4] = {-1,0,1,0};
int dc[4] = {0,-1,0,1};

void colorManager(int color);

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// dfs
void solveThisColor(int row, int col, int ind) {
    int color = colorsAccToFreeMoves[ind].second;
    if (solved) {
        return;
    }

    if (colorX[color][1] == row && colorY[color][1] == col) {
        colorManager(ind+1);
        return;
    }

    int prev = grid[row][col];
    int prevVis = vis[row][col];

    vis[row][col] = 1;
    grid[row][col] = color;

    for (int i = 0; i < 4; i++) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (newRow < n && newCol < m && newRow >= 0 && newCol >= 0) {
            if (vis[newRow][newCol] == 0 || (vis[newRow][newCol] == 2 && grid[newRow][newCol] == color)) {
                solveThisColor(newRow, newCol, ind);
            }

            if (solved) return;
        }
    }
    
    grid[row][col] = prev;
    vis[row][col] = prevVis;
}

void colorManager(int ind) {
    if (ind >= colorsAccToFreeMoves.size()) {
        bool ok = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (vis[i][j] == 0)
                    ok = 0;
            }
        }

        if (ok) {
            solved = 1;
        }

        return;
    }

    if (solved == 1) {
        return;
    }

    int color = colorsAccToFreeMoves[ind].second;
    int row = colorX[color][0];
    int col = colorY[color][0];
    solveThisColor(row, col, ind);
}

int main() {
    cin >> n >> m;

    grid.resize(n, vector<int>(m, -1));
    vis.resize(n, vector<int>(m, 0));

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

    sort(colorsAccToFreeMoves.begin(), colorsAccToFreeMoves.end()); // sorting in ascending order of free moves

    cout << "\nCalculating...\n";

    colorManager(0);

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