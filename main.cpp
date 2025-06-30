#include "aco.h"

using namespace std;

int main() {
    srand(time(0));
    int num_items;
    int num_machines;

    ifstream file("in.txt");
    file >> num_items;
    file >> num_machines;

    Aco g(num_items, num_machines, 0, 0, 0, 0);

    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < num_machines; j++) {
            int w;
            file >> w;
            g.add_machine(i, j, w);
        }
    }
    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < num_items; j++) {
            vector<int> p = {i, j};
            int dist = calc(p, g.machines);
            g.add_conn(i, j, dist);
        }
    }

    int Q = 10;

    g.set(1, 10, Q, 0.5);
    g.run(0, 2000, num_items);

    g.save("out.csv");
    return 0;
}