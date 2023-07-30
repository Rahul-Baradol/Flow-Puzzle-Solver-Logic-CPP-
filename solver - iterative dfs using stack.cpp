#include <iostream>
#include <vector>

#define push push_back
#define pop pop_back

using namespace std;

int n,m;

int dr[4] = {1,0,-1,0};
int dc[4] = {0,1,0,-1};

vector<vector<int>> grid; 
vector<vector<int>> pastGrid;

vector<int> colorPresent;
vector<int> colorX[26];
vector<int> colorY[26];
vector<int> colorConnection;

vector<vector<int>> vis;
vector<vector<int>> pastVis;

bool solved = 0;

void establishColorConnection() {
    for (int i = 1; i < colorPresent.size(); i++) {
        colorConnection[colorPresent[i-1]] = colorPresent[i];
    }
}

// row, col, color
vector<vector<int>> st;

vector<pair<int,int>> path;

vector<vector<int>> deflections;

void dfs() {
    int initalColor = colorPresent[0];
    int initialX = colorX[initalColor][0];
    int initialY = colorY[initalColor][0];

    st.push({initialX, initialY, initalColor});

    while (!st.empty()) {
        vector<int> top = st.back();
        st.pop(); 

        int row = top[0];
        int col = top[1];
        int color = top[2];

        int destX = colorX[color][1];
        int destY = colorY[color][1];

        if (row == destX && col == destY) {
            int newColor = colorConnection[color];
            
            if (newColor == -1) {
                solved = 1;
                return;
            }
            
            int newRow = colorX[newColor][0];
            int newCol = colorY[newColor][0];

            st.push({newRow, newCol, newColor});
            continue;
        }

        path.push({row, col});
        grid[row][col] = color;
        vis[row][col] = 1;

        bool ok = 0;
        for (int k = 0; k < 4; k++) {
            int newRow = row + dr[k];
            int newCol = col + dc[k];

            if (newRow < 0 || newCol < 0 || newRow >= n || newCol >= m) continue;
            if (vis[newRow][newCol] == 1) continue;

            bool condition = (newRow == colorX[color][1]) && (newCol == colorY[color][1]);
            if (vis[newRow][newCol] == 2 && !condition) continue;

            if (ok) {
                deflections[row][col]++;
            }
            ok = 1;
            st.push({newRow, newCol, color});
        }

        if (!ok && path.size()) {
            pair<int,int> tmp = path.back();
            while (path.size()) {
                if (deflections[tmp.first][tmp.second] > 0) {
                    deflections[tmp.first][tmp.second]--;
                    break;
                }
                path.pop();

                grid[tmp.first][tmp.second] = pastGrid[tmp.first][tmp.second];
                vis[tmp.first][tmp.second] = pastVis[tmp.first][tmp.second];

                if (path.size())
                    tmp = path.back();
                else
                    break;
            }

            grid[row][col]  = pastGrid[row][col];
            vis[row][col] = pastVis[row][col];
        }
    }
}

int main() {
    cin >> n >> m;

    grid.resize(n, vector<int>(m, -1));
    colorConnection.resize(26, -1);
    vis.resize(n, vector<int>(m, 0));
    deflections.resize(n, vector<int>(m, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char color;
            cin >> color;

            if (color == '.') {
                continue;
            }

            int colorCode = color - 'A';
            if (colorX[colorCode].size() == 0) {
                colorPresent.push_back(colorCode);
            }
            grid[i][j] = colorCode;
            colorX[colorCode].push_back(i);
            colorY[colorCode].push_back(j);
            vis[i][j] = 2;
        }
    }

    establishColorConnection();

    pastGrid = grid;
    pastVis = vis;

    cout << "\nCalculating...\n";
    dfs();

    if (!solved) {
        cout << "\nIt is not possible to solve the given puzzle.\n";
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