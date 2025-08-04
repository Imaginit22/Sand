#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <fstream>
#include <cmath>
#include <string>

const int THRESHOLD = 4; // Toppling threshold for each cell
const int GRAINS = 12; // Total grains
const int GRID_SIZE = 2 * std::sqrt(GRAINS / 3); // Size of the grid
const int FRAMESIZE = 1;
// Directions for neighbor cells: (up, down, left, right)
const int directions[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

void write_data_to_file(const std::vector<std::vector<int>>& data) {
    const std::string tmp_path = "output.tmp";
    const std::string final_path = "output.txt";

    std::ofstream out(tmp_path);
    if (!out) {
        throw std::runtime_error("Failed to open temporary file for writing.");
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            out << row[i];
            if (i < row.size() - 1) out << " ";
        }
        out << "\n";
    }
    out.close();  // very important to close before renaming

    if (std::rename(tmp_path.c_str(), final_path.c_str()) != 0) {
        throw std::runtime_error("Failed to rename temp file to final output.");
    }
}

class Sandpile {
public:
    std::vector<std::vector<int>> grid; // The sandpile grid

    Sandpile() {
        // Initialize grid with all zeros and add grains to the center
        grid.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
        grid[GRID_SIZE / 2][GRID_SIZE / 2] = GRAINS;
    }

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
        int topplesCount = 0;

        // If no specific coordinates are provided, randomly select a position
        if (x == -1) {
            x = randomInt(0, GRID_SIZE - 1);
            y = randomInt(0, GRID_SIZE - 1);
        }

        // Start the toppling process if the initial grain exceeds the threshold
        if (grid[x][y] >= THRESHOLD) {
            toTopple.push({x, y});
        }

        // This array will help track which cells are in the queue to avoid duplicates
        std::vector<std::vector<bool>> inQueue(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
        inQueue[x][y] = true;

        int counter = 0;
        int bigCounter = 0;

        // While the queue is not empty, continue the toppling process
        while (!toTopple.empty()) {
            auto [cx, cy] = toTopple.front();
            toTopple.pop();
            inQueue[cx][cy] = false;

            // Continue toppling until the cell is no longer over the threshold
            while (grid[cx][cy] >= THRESHOLD) {
                topplesCount++;
                grid[cx][cy] -= THRESHOLD;

                // Check the neighbors of the current cell
                for (int i = 0; i < 4; i++) {
                    int nx = cx + directions[i][0];
                    int ny = cy + directions[i][1];
                    if (inBounds(nx, ny)) {
                        grid[nx][ny]++;
                        // If the neighbor reaches the threshold, add it to the queue
                        if (grid[nx][ny] >= THRESHOLD && !inQueue[nx][ny]) {
                            toTopple.push({nx, ny});
                            inQueue[nx][ny] = true;
                        }
                    }
                }
            }

            // Periodically write data to file for tracking the simulation state
            counter++;
            if (counter % FRAMESIZE == 0) {
                if (bigCounter) {
                    std::cout << "Big Counter: " << bigCounter << std::endl;
                }
                counter = 0;
                bigCounter++;
                write_data_to_file(grid);
                std::cout << "Remaining Grains at Center: " << grid[GRID_SIZE / 2][GRID_SIZE / 2] << std::endl;
            }
        }

        return topplesCount;
    }
};

int main() {
    // Create a Sandpile object to simulate the dynamics
    Sandpile sandpile;

    // Open output file for logging the topplings
    std::ofstream out("topples.txt");
    sandpile.addGrain(GRID_SIZE / 2, GRID_SIZE / 2); // Add grains at the center
    out.close();

    // Write the final grid state to file
    out.open("output.txt");
    for (const auto& row : sandpile.grid) {
        for (size_t j = 0; j < row.size(); ++j) {
            out << row[j] << " ";
        }
        out << std::endl;
    }
    out.close();

    return 0;
}
