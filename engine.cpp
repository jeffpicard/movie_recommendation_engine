#include "engine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <math.h>
#include <numeric>

Engine::Engine(){
    // for optimal results: lrate .01, 10 epochs, 20 features
    lrate = .01;
    n_features = 20;
    n_epochs.resize(n_features, 10);
    max_id_movie = 0;
}

Engine::~Engine(){
}


// load the data
void Engine::load_sets(string train_name, string test_name) {
    train_filename = train_name;
    test_filename = test_name;
    
    ifstream train_file(train_filename);
    string line, user_id, movie_id, rating_str, dud;
    set<int> count_users;
    set<int> count_movies;

    while (getline(train_file, line)) {
        stringstream line_stream(line);
        line_stream >> user_id >> movie_id >> rating_str;  

        user.push_back(stoi(user_id));
        movie.push_back(stoi(movie_id));
        rating.push_back(stof(rating_str));

        count_users.insert(stoi(user_id));
        count_movies.insert(stoi(movie_id));
        if (stoi(movie_id) > max_id_movie) {
            max_id_movie = stoi(movie_id);
        }
    }
    n_users = count_users.size();
    n_movies = count_movies.size();
    train_file.close();
}

// find the average for each movie, and each user's bias
void Engine::calculate_bias(){
    int n_ratings = 0;
    double total_rating = 0;

    unordered_map<int, int> n_ratings_movie;
    unordered_map<int,float> total_rating_movie;
    unordered_map<int,int> n_ratings_user;
    unordered_map<int,float> total_bias_user;

    // get movie averages
    for (int i = 0; i < user.size(); i++) {
        n_ratings_movie[movie[i]]++;
        total_rating_movie[movie[i]] += rating[i];
    }
    for (auto it = total_rating_movie.begin(); it != total_rating_movie.end(); it++) {
        movie_average[it->first] = it->second / (float)n_ratings_movie[it->first];
    }

    // get user biases
    bias_user.resize(n_users);
    for (int i = 0; i < user.size(); i++) {
        n_ratings_user[user[i]]++;
        total_bias_user[user[i]] += rating[i] - movie_average[movie[i]];
    }
    for (auto it = total_bias_user.begin(); it != total_bias_user.end(); it++) {
        bias_user[it->first] = it->second / (float)n_ratings_user[it->first];
    }
}


// train one {userID, movieID, rating} tuple
void Engine::train_sample(int sample, vector<float> &user_vec, unordered_map<int,float> &movie_vec) {
    float error = lrate * (rating[sample] - (predicted_rating_without_feature[sample] + user_vec[user[sample]] * movie_vec[movie[sample]]) );
    float temp = user_vec[user[sample]];
    user_vec[user[sample]] += error * movie_vec[movie[sample]];
    movie_vec[movie[sample]] += error * temp;
}


// run the whole training process
void Engine::train() {
    // initialize to avg + bias for feature 0
    predicted_rating_without_feature.resize(user.size());
    for (int i = 0; i < user.size(); i++) {
        predicted_rating_without_feature[i] = movie_average[movie[i]] + bias_user[user[i]];
    }

    cout << "starting training" << endl;
    for (int f = 0; f < n_features; f++) {
        cout << "Starting feature " << f << endl;

        // initialize new feature vector to .1
        vector<float> user_value;
        user_value.resize(n_users, .1);

        unordered_map<int,float> movie_value;
        for (int i = 0; i < movie.size(); i++) {
            movie_value[movie[i]] = .1;
        }

        // train!
        for (int e = 0; e < n_epochs[f]; e++) {
            for (int s = 0; s < user.size(); s++) {
                train_sample(s, user_value, movie_value);
            }

            /*************************/
            if (f == 0) { 
                validate(e, user_value, movie_value);
            }
            
        }

        // add this feature to the list
        for (int i = 0; i < user.size(); i++) {
            predicted_rating_without_feature[i] += user_value[user[i]] * movie_value[movie[i]];
        }
        user_feature.push_back(user_value);
        movie_feature.push_back(movie_value);

//        validate(f, user_value, movie_value);
    }
    cout << "Size is " << movie_feature.size() << endl;
}

// load the true ratings from file
void Engine::get_real_rate() {
    ifstream real_file("./data/20m/true_rating.tsv");
    string line, rate_str;
    float rate;
    while (getline(real_file, line)) {
        // read a test line
        stringstream line_stream(line);
        line_stream >> rate_str;
        rate = stof(rate_str);
        real_rate.push_back(rate);
    }
}

// find estimated out-of-sample error
void Engine::validate(int epoch, vector<float> &u, unordered_map<int, float> &m) {
    ifstream test_file(test_filename);
    string line, user_id_str, movie_id_str;
    int user_id, movie_id;
    float predicted_rating;
    long i = 0;
    vector<float> sq_err;

    while (getline(test_file, line)) {
        // read a test line
        stringstream line_stream(line);
        line_stream >> user_id_str >> movie_id_str;
        user_id = stoi(user_id_str);
        movie_id = stoi(movie_id_str);

        // predict the rating
        predicted_rating = predict_rating_temp(user_id, movie_id, u, m);

        // save
        sq_err.push_back( pow(real_rate[i] - predicted_rating, 2.));
        i++;
    }
    test_file.close();
    float rmse = 0;
    for (int i=0; i < sq_err.size(); i++) {
        rmse += sq_err[i];
    }
    rmse = pow(rmse / (float)sq_err.size(), .5);

    x_axis.push_back(epoch);
    y_axis.push_back(rmse);
}


// Export data for plotting in Python
void Engine::plot() {
    ofstream plot_file("plot_data.dat");
    for (int i = 0; i < x_axis.size(); i++) {
        plot_file << x_axis[i] << " " << y_axis[i] << endl;
    }
}



// Predict an unknown rating
float Engine::predict_rating(int user_id, int movie_id, int version) {
    if (version != 3) {
        return 3.;
    }
    float avg = movie_average[movie_id];
    float bias = bias_user[user_id];
    float learned = 0;
    for (int f = 0; f < user_feature.size(); f++) {
        learned += user_feature[f][user_id] * movie_feature[f][movie_id];
    }
    return avg + bias + learned;
}


// Slightly modified version. Needs to get incorporated into original function
float Engine::predict_rating_temp(int user_id, int movie_id, vector<float> &u, unordered_map<int, float> &m) {
    float avg = movie_average[movie_id];
    float bias = bias_user[user_id];
    float learned = 0;
    for (int i = 0; i < user_feature.size(); i++) {
        learned += user_feature[i][user_id] * movie_feature[i][movie_id];
    }
    learned += u[user_id] * m[movie_id];
    return avg + bias + learned;
}



// Run the trained result on unseen samples
void Engine::test(string test_filename, string output_filename) {
    ifstream test_file(test_filename);
    ofstream predicted_rating_file(output_filename);

    string line, user_id_str, movie_id_str;
    int user_id, movie_id;
    float predicted_rating;
    while (getline(test_file, line)) {
        // read a test line
        stringstream line_stream(line);
        line_stream >> user_id_str >> movie_id_str;
        user_id = stoi(user_id_str);
        movie_id = stoi(movie_id_str);

        // predict the rating
        predicted_rating = predict_rating(user_id, movie_id);

        // write the predicted rating
        predicted_rating_file << predicted_rating << '\n';
    }
    test_file.close();
    predicted_rating_file.close();
}
