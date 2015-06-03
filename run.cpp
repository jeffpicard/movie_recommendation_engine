#include <iostream>
#include "engine.h"
#include <math.h>

using namespace std;

int main() {
    int mini = 1;

    string suffix = "";
    if (mini) {
        suffix = "_mini";
    }
    string train_set_filename = "./data/train" + suffix + ".tsv";
    string test_set_filename = "./data/test" + suffix + ".tsv";
    string predicted_set_filename = "./data/predicted_rating" + suffix + ".tsv";


    Engine eng = Engine();

    eng.load_train_set(train_set_filename);
    eng.calculate_bias();
    eng.test(test_set_filename, predicted_set_filename);

    cout << "done" << endl;
    return 0;
}
