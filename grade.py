import numpy as np

true_ratings_file = open('./data/true_playcount_mini.tsv')
true_ratings_list = []
for line in true_ratings_file:
    true_ratings_list.append(float(line))
true_ratings = np.array(true_ratings_list)

predicted_ratings_file = open('./data/predicted_playcount_mini.tsv')
predicted_ratings_list = []
for line in predicted_ratings_file:
    predicted_ratings_list.append(float(line))
predicted_ratings = np.array(predicted_ratings_list)

individual_error = np.divide(np.abs(predicted_ratings - true_ratings), true_ratings)
avg_error = np.mean(individual_error)

print avg_error

