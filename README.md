# movie_recommendation_engine
A quick attempt at predicting what rating a user will give to a selected movie.
Inspired by the Netflix Prize.


What this code does:
-Learn from a training set of millions of {userID, movieID, rating} rows
-Run over a (never before seen) test set of {userID, movieID} rows and predict
 a rating for each one
-Return the average error between predicted value and true value of the test set
-Make plots such as error vs. number of features and error vs. number of epochs



Methods
Collaborative filtering...
I do some quick corrections for movie and user biases and then essentially do a
Principal Component Analysis / Singular Value Decomposition. I use a greedy
algorithm to find the top most descriptive features. The features are abstract,
but if I were to print movies that have a high component of one particular
feature, a human might look at them and say "oh, this feature is something like
movies about exploring space". To predict a rating, the engine looks at what
features the given user likes, and if the given movie is high in those features.



To run it:
Download the data, randomly cut out, say, 1% of the lines and save to
./data/20m/train.tsv     For the lines you cut out, put the {userID, movieID}
columns into ./data/20m/test.tsv and the rating into ./data/20m/true_rating.tsv

Then:
make
./run
grade.py



References
Data: http://grouplens.org/datasets/movielens/
The basic idea of how to do this: http://sifter.org/simon/journal/20061211.html