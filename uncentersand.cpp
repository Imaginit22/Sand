#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <fstream>
#include <random> 
#include <queue>
#include <set>
const int THRESHOLD = 4; // Toppling threshold for each cell

//why
//40000 is cool
//240000 is weird
const int GRAINS = 2400000;
const int GRID_SIZE = 100; // Size of the grid 
const int GRID_CENTER = GRID_SIZE / 2;
const int FRAMESIZE = 1;
// Directions for neighbor cells: (up, down, left, right)
const int directions[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

class Sandpile {
public:
    std::vector<std::vector<int> > grid;
    Sandpile() {
        // Initialize a 10x10 grid with random sand grains (for simplicity)
        grid.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
        grid[GRID_SIZE / 2][GRID_SIZE / 2] = GRAINS;
    }

    // Check if the cell is within bounds of the grid
    bool inBounds(int x, int y) {
        return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
    }

    int randomInt(int min, int max) {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }
    
    // Simulate the sandpile dynamics
    int addGrain(int x = -1, int y = -1) {
        std::queue<std::pair<int, int>> toTopple;
        int retnum = 0;

        if (x == -1) {
            x = randomInt(0, GRID_SIZE - 1);
            y = randomInt(0, GRID_SIZE - 1);
        }

        if (grid[x][y] >= THRESHOLD) {
            toTopple.push({ x, y });
        }
        int counter = 0;
        int bigcounter = 0;
        while (!toTopple.empty()) {
            
            auto [cx, cy] = toTopple.front();
            if (grid[cx][cy] < THRESHOLD) {
                toTopple.pop();
            }
            counter++;
            if (counter % FRAMESIZE == 0) {
                break;
            }
            if (grid[cx][cy] < THRESHOLD) continue;

            retnum++;
            grid[cx][cy] -= THRESHOLD;
            //look at center, don't propagate in that direction
            std::set<int> toskip = {};
            if (!(cx == GRID_CENTER) && !(cy == GRID_CENTER)) {
                int xdist = std::abs(cx - GRID_CENTER);
                int ydist = std::abs(cy - GRID_CENTER);
                //REWRITE THIS TO ALLOW FOR MULTIPLE DIRECTIONS
                if (xdist > ydist) {
                    if (cx - GRID_CENTER > 0) {
                        toskip.insert(0);
                    } else {
                        toskip.insert(1);
                    }
                } else if (xdist == ydist) {
                    if (cx - GRID_CENTER > 0) {
                        toskip.insert(0);
                    } else {
                        toskip.insert(1);
                    }
                    if (cy - GRID_CENTER > 0) {
                        toskip.insert(2);
                    } else {
                        toskip.insert(3);
                    }
                } else {
                    if (cy - GRID_CENTER > 0) {
                        toskip.insert(2);
                    } else {
                        toskip.insert(3);
                    }
                }
            }
            
            for (int i = 0; i < 4; i++) {
                if (toskip.find(i) != toskip.end()) continue;
                int nx = cx + directions[i][0];
                int ny = cy + directions[i][1];
                if (inBounds(nx, ny)) {
                    grid[nx][ny]++;
                    if (grid[nx][ny] >= THRESHOLD) {
                        toTopple.push({ nx, ny });
                    }
                }
            }
        }

        return retnum;
    }
    bool checkSymmetry() {
        // Check if the grid is symmetric about the center, 
        std::vector<std::vector<int> > otherQuads = {{1, 0}, {0, 1}, {1, 1}};
        bool evengrid = (GRID_SIZE%2 == 0);
        for (auto d : otherQuads) {
            for (int i = evengrid; i < GRID_CENTER; i++) {
                for (int j = evengrid; j < GRID_CENTER; j++) {
                    std::cout << "I, J " << i << " " << j << std::endl;
                    if (grid[i][j] != grid[GRID_SIZE*d[0] - i * (d[0]*2 - 1)][GRID_SIZE*d[1] - j * (d[1]*2 - 1)]) {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }

};

int main() {
    // Create a sandpile object
    Sandpile sandpile;

    // Run the simulation
    std::ofstream out("topples.txt");
    sandpile.addGrain(GRID_CENTER, GRID_CENTER);
    std::cout << "SYMCHECK " << sandpile.checkSymmetry() << std::endl;
    out.close();
    out.open("output.txt");
    for (auto i : sandpile.grid) {
        for (int j : i) {
            out << j << " ";
        }
        out << std::endl;
    }
    out.close();
    return 0;
}
