import numpy as np

mini = 0
suffix = ''
if mini == 1:
    suffix = '_mini'

true_ratings_file = open('./data/20m/true_rating'+suffix+'.tsv')
true_ratings_list = []
for line in true_ratings_file:
    true_ratings_list.append(float(line))
true_ratings = np.array(true_ratings_list)

predicted_ratings_file = open('./data/20m/predicted_rating'+suffix+'.tsv')
#predicted_ratings_file = open('temp.txt')
predicted_ratings_list = []
for line in predicted_ratings_file:
    predicted_ratings_list.append(float(line))
predicted_ratings = np.array(predicted_ratings_list)

individual_error = np.abs(predicted_ratings - true_ratings)
avg_error = np.mean(individual_error)

rmse = np.sqrt(np.mean((predicted_ratings - true_ratings) ** 2))

print 'rmse:',rmse, '    mean_mag:', avg_error

