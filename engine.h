#ifndef engine_h
#define engine_h

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Engine {
    int n_features;
    vector<int> n_epochs;
    float lrate;
    float lambda; // regularization constant
    float avg_rating;
    string train_filename;
    string test_filename;

    int n_users;
    int n_movies;
    int max_id_movie;

    vector<int> user;
    vector<int> movie;
    vector<float> rating;

    vector<float> bias_user;
    unordered_map<int,float> movie_average;

    vector< vector<float> > user_feature;
    vector< unordered_map<int,float> > movie_feature;

    vector<float> predicted_rating_without_feature;

    vector<float> real_rat;
    vector<int> x_axis;
    vector<float> y_axis;

public:
    Engine();
    ~Engine();
    void load_sets(string train_name, string test_name);
    void calculate_bias();
    float predict_rating(int user_id, int movie_id, int version = 3);
    void train_sample(int sample, vector<float> &user_vec, unordered_map<int, float> &movie_vec); 
    void train();
    void test(string test_filename, string output_filename);
    void validate(int epoch, vector<float> &u, unordered_map<int, float> &m);
    float predict_rating_temp(int user_id, int movie_id, vector<float> &u, unordered_map<int, float> &m);
    void get_real_rat();
    void plot();
};





#endif
