#include <bits/stdc++.h>

using namespace std;

int n,m;

vector<vector<int>> puzzle;
map<char, pair<int,int>> colorIndexMap;

bool solved = false;
int colorsPresence[26];

void colorManager(int color, vector<vector<int>> vis, vector<vector<int>> grid);

// dfs
void solveThisColor(int row, int col, vector<vector<int>> vis, vector<vector<int>> grid,int color, int recursionLimit) {
    if (solved) {
        return;
    }
    vis[row][col] = 1;

    if (grid[row][col] == color && ((colorIndexMap[color].first) != row || (colorIndexMap[color].second != col))) {
        colorManager(color+1, vis, grid);
        return;
    }

    grid[row][col] = color;
    int dr[4] = {-1,0,1,0};
    int dc[4] = {0,-1,0,1};
    for (int i = 0; i < 4; i++) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (newRow < n && newCol < m && newRow >= 0 && newCol >= 0) {
            if (vis[newRow][newCol] == 0) {
                solveThisColor(newRow, newCol, vis, grid, color, 0);
            }

            if (vis[newRow][newCol] == 2 && grid[newRow][newCol] == color) {
                solveThisColor(newRow, newCol, vis, grid, color, 0);
            }
        }
    }
}

void colorManager(int color, vector<vector<int>> vis, vector<vector<int>> grid) {
    if (color >= 26) {
        bool ok = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (vis[i][j] == 0)
                    ok = 0;
            }
        }

        if (ok) {
            solved = 1;
            puzzle = grid;
        }

        return;
    }

    if (solved == 1) {
        return;
    }

    if (colorsPresence[color] == 0) {
        colorManager(color+1, vis, grid);
        return;
    }


    int row = colorIndexMap[color].first;
    int col = colorIndexMap[color].second;
    solveThisColor(row, col, vis, grid,color,0);
}

int main() {
    cin >> n >> m;

    puzzle.resize(n, vector<int>(m, -1));
    vector<vector<int>> vis(n, vector<int>(m, 0));

    memset(colorsPresence, 0, sizeof(colorsPresence));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char color;
            cin >> color;

            if (color == '.') {
                continue;
            }

            int colorCode = color - 'A';

            colorsPresence[colorCode] = 1;
            puzzle[i][j] = colorCode;
            colorIndexMap[colorCode] = {i, j};
            vis[i][j] = 2;
        }
    }

    colorManager(0, vis, puzzle);

    if (!solved) {
        cout << "Not possible to solve the given puzzle :(\n";
    } else {
        cout << "\nYes it is possible to solve the puzzle as follows :)\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                char color = puzzle[i][j] + 'A';
                cout << color << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}