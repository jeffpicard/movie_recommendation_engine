#ifndef engine_h
#define engine_h

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Engine {
    int n_features;
    int n_epochs;
    float lrate;
    float lambda; // regularization constant
    float avg_rating;
    string train_filename;

    int n_users;
    int n_movies;
    int max_id_movie;

    vector<int> user;
    vector<int> movie;
    vector<float> rating;

//    vector< vector<float> > user_feature;
//    vector< vector<float> > artist_feature;
    vector<float> bias_user;
    unordered_map<int,float> bias_movie;


public:
    Engine();
    ~Engine();
    void load_train_set(string filename);
    void calculate_bias();
    double predict_rating(int user, int artist);
    void train_sample(int user, int artist, float rating); 
    void train();
    void test(string test_filename, string output_filename);

};





#endif
