#include "engine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

Engine::Engine(){
    lrate = .1;
    max_id_movie = 0;
}

Engine::~Engine(){
}


void Engine::load_train_set(string filename) {
    train_filename = filename;
    
    ifstream train_file(filename);
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

void Engine::calculate_bias(){
    int n_ratings = 0;
    double total_rating = 0;

    unordered_map<int,int> n_ratings_user;
    unordered_map<int,float> total_rating_user;
    unordered_map<int, int> n_ratings_movie;
    unordered_map<int,float> total_rating_movie;

    // sum the ratings
    for (int i = 0; i < user.size(); i++) {
        n_ratings++;
        total_rating += rating[i];

        n_ratings_user[user[i]]++;
        total_rating_user[user[i]] += rating[i];

        n_ratings_movie[movie[i]]++;
        total_rating_movie[movie[i]] += rating[i];
    }
   
    //divide by number of ratings to get averages
    avg_rating = total_rating / (float)n_ratings;

    bias_user.resize(n_users);

    for (auto it = total_rating_user.begin(); it != total_rating_user.end(); it++) {
        bias_user[it->first] = it->second / (float)n_ratings_user[it->first] - avg_rating;
    }

    for (auto it2 = total_rating_movie.begin(); it2 != total_rating_movie.end(); it2++) {
        bias_movie[it2->first] = it2->second / (float)n_ratings_movie[it2->first] - avg_rating;
    }
}

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
        predicted_rating = avg_rating + bias_user[user_id] + bias_movie[movie_id];
        //predicted_rating = 3.5;

        // write the predicted rating
        predicted_rating_file << predicted_rating << '\n';
    }
    test_file.close();
    predicted_rating_file.close();
}
