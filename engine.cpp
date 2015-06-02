#include "engine.h"

Engine::Engine(){
    lrate = .1;
}

Engine::~Engine(){
    delete[] user_feature;
    delete[] artist_feature;
    delete[] bias_user;
    delete[] bias_artist;
}
