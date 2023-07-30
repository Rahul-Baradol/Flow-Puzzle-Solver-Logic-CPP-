#include <iostream>
#include <vector>

using namespace std;

int n,m;

vector<vector<int>> grid;
vector<vector<int>> vis;
int colorX[26];
int colorY[26];

bool solved = false;
int colorsPresence[26];

void colorManager(int color);

// dfs
void solveThisColor(int row, int col, int color) {
    if (solved) {
        return;
    }

    if (grid[row][col] == color && ((colorX[color] != row) || (colorY[color] != col))) {
        colorManager(color+1);
        return;
    }

    int prev = grid[row][col];
    int prevVis = vis[row][col];

    vis[row][col] = 1;
    grid[row][col] = color;

    int dr[4] = {-1,0,1,0};
    int dc[4] = {0,-1,0,1};
    for (int i = 0; i < 4; i++) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (newRow < n && newCol < m && newRow >= 0 && newCol >= 0) {
            if (vis[newRow][newCol] == 0) {
                solveThisColor(newRow, newCol, color);
            }

            if (vis[newRow][newCol] == 2 && grid[newRow][newCol] == color) {
                solveThisColor(newRow, newCol, color);
            }

            if (solved) return;
        }
    }
    
    grid[row][col] = prev;
    vis[row][col] = prevVis;
}

void colorManager(int color) {
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
        }

        return;
    }

    if (solved == 1) {
        return;
    }

    if (colorsPresence[color] == 0) {
        colorManager(color+1);
        return;
    }


    int row = colorX[color];
    int col = colorY[color];
    solveThisColor(row, col, color);
}

int main() {
    cin >> n >> m;

    grid.resize(n, vector<int>(m, -1));
    vis.resize(n, vector<int>(m, 0));

    for (int i = 0; i < 26; i++) {
        colorsPresence[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char color;
            cin >> color;

            if (color == '.') {
                continue;
            }

            int colorCode = color - 'A';

            colorsPresence[colorCode] = 1;
            grid[i][j] = colorCode;
            colorX[colorCode] = i;
            colorY[colorCode] = j;
            vis[i][j] = 2;
        }
    }

    colorManager(0);

    if (!solved) {
        cout << "\nNot possible to solve the given puzzle :(\n";
    } else {
        cout << "\nYes it is possible to solve the puzzle as follows :)\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                char color = grid[i][j] + 'A';
                cout << color << " ";
            }
            cout << "\n";
        }
    }

    return 0;
}