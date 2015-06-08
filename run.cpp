#include <iostream>
#include "engine.h"
#include <math.h>

using namespace std;

int main() {
    int mini = 0;

    string suffix = "";
    if (mini) {
        suffix = "_mini";
    }
    string train_set_filename = "./data/20m/train" + suffix + ".tsv";
    string test_set_filename = "./data/20m/test" + suffix + ".tsv";
    string predicted_set_filename = "./data/20m/predicted_rating" + suffix + ".tsv";


    Engine eng = Engine();

    eng.load_sets(train_set_filename, test_set_filename);
    eng.get_real_rate();
    eng.calculate_bias();
    eng.train();
    eng.plot();
    eng.test(test_set_filename, predicted_set_filename);

    cout << "done" << endl;
    return 0;
}
