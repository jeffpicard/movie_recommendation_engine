#ifndef engine_h
#define engine_h

#include <string>

using namespace std;

class Engine {
    int n_features;
    int n_epochs;
    float lrate;
    float lambda; // regularization constant
    float avg_rating;

    float *user_feature;
    float *artist_feature;
    float *bias_user;
    float *bias_artist;

public:
    Engine();
    ~Engine();
    double predict_playcount(int user, string artist);
    void train(int user, string artist, int playcount); 

};





#endif
