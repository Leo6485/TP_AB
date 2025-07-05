#include "aco.h"

using namespace std;

int main() {
    srand(time(0));
    int num_items;
    int num_machines;

    ifstream file("in.txt");
    file >> num_items;
    file >> num_machines;

    Aco g(num_items, num_machines, 1.33, 1, 100, 0.6);
    
    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < num_machines; j++) {
            int w;
            file >> w;
            g.add_item_machine_time(i, j, w);
        }
    }

    g.calculate_raw_times();
    g.run(0, num_items + num_machines, num_items);

    return 0;
}
