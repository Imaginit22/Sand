#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <fstream>
#include <random> 
#include <queue>

const int GRID_SIZE = 10; // Size of the grid (n x n)
const int THRESHOLD = 4; // Toppling threshold for each cell

// Directions for neighbor cells: (up, down, left, right)
const int directions[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

class Sandpile {
public:
    std::vector<std::vector<int> > grid;

    Sandpile() {
        // Initialize a 10x10 grid with random sand grains
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
        int retnum = 0;

        if (x == -1) {
            x = randomInt(0, GRID_SIZE-1);
            y = randomInt(0, GRID_SIZE-1);
        }
        
        grid[x][y]++;
        if (grid[x][y] == THRESHOLD) {
            grid[x][y] = 0;
            for (auto i : directions) {
                int newx = x + i[0];
                int newy = y + i[1];
                if (inBounds(newx, newy)) {
                    retnum += addGrain();
                }
            }
            return retnum + 1;
        } else {
            return 0;
        }
    }

    void simulate(int iterations = 10) {
        for (int i = 0; i < iterations; i++) {
            int topples = addGrain();
            if (i % 100 == 0) {
                std::cout << i << std::endl;
                std::cout << "Topples:" << topples << std::endl;
            }
        }
    }
};

int main() {
    // Create a sandpile object
    Sandpile sandpile;

    // Run the simulation
    sandpile.simulate(100);
    std::cout << sandpile.grid[1][1];
    std::ofstream out("output.txt");
    for (auto i : sandpile.grid) {
        for (int j : i) {
            out << j << " ";
        }
        out << std::endl;
    }
    out.close();
    return 0;
}
