#include "aco.h"

using namespace std;

int main() {
    srand(time(0));
    int num_items;
    int num_machines;

    ifstream file("in_7.txt");
    file >> num_items;
    file >> num_machines;

    Aco g(num_items, num_machines, 1.33, 5, 100, 0.8);
    
    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < num_machines; j++) {
            int w;
            file >> w;
            g.add_item_machine_time(i, j, w);
        }
    }

    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < num_items; j++) {
            if (i == j) continue;
            vector<int> p = {i, j};
            int dist = calcula_ociosidade(p, g.machines);
            g.add_item_distance(i, j, dist);
        }
    }
    g.run(0, 200, num_items * 12);

    return 0;
}