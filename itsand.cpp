#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <fstream>
#include <random> 
#include <queue>

const int GRID_SIZE = 50; // Size of the grid (10x10)
const int THRESHOLD = 4; // Toppling threshold for each cell

// Directions for neighbor cells: (up, down, left, right)
const int directions[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

class Sandpile {
public:
    std::vector<std::vector<int> > grid;
    Sandpile() {
        // Initialize a 10x10 grid with random sand grains (for simplicity)
        grid.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
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

        grid[x][y]++;
        if (grid[x][y] >= THRESHOLD) {
            toTopple.push({ x, y });
        }

        while (!toTopple.empty()) {
            auto [cx, cy] = toTopple.front();
            toTopple.pop();
            if (grid[cx][cy] < THRESHOLD) continue;

            retnum++;
            grid[cx][cy] -= THRESHOLD;

            for (int i = 0; i < 4; i++) {
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


    std::vector<int> simulate(int iterations = 10) {
        std::vector<int> topplecountcounts;
        for (int i = 0; i < iterations; i++) {
            topplecountcounts.push_back(addGrain());
            if (i % 100 == 0) {
                std::cout << i << std::endl;
            }
        }
        return topplecountcounts;
    }
};

int main() {
    // Create a sandpile object
    Sandpile sandpile;

    // Run the simulation
    std::ofstream out("topples.txt");
    for (int i :sandpile.simulate(GRID_SIZE*GRID_SIZE*2)) {
        out << i << " ";
    }
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
