from lorem_text import lorem
import sys
import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import csv
from scipy import stats
import numpy as np

folder_dir = os.getcwd()
otf_times = {}

for num_runs in range(10):
    dirName = "run_" + str(num_runs + 1)
    try:
        os.mkdir(dirName)
    except FileExistsError:
        print("Directory " , dirName ,  " already exists")
    run_dir = folder_dir + "/" + dirName + "/"
    # for num_docs in range(num_runs + 1):
    #     file_name = run_dir + "doc_" + str(num_docs+1) + ".txt"
    #     print(file_name)
    #     file = open(file_name, "w+")
    #     file.write(lorem.words(500))
    #     file.close()
    # for i in range(4):
    subprocess.run([folder_dir + "/bin/benchmarking/OnTheFlyDocumentSearch/OnTheFlyDocumentSearch_benchmarking", run_dir])

    times = []

    with open(folder_dir + "/OnTheFlyDocumentSearch_benchmarking.csv", 'r') as fil:
        tmp = fil.read().split('\n')
        tmp = tmp[1:-1]
        times = [float(i) for i in tmp]
    otf_times[str(num_runs + 1)] = times


df = pd.DataFrame(otf_times)
test = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)]
fig = test.plot(kind='box', figsize=(20, 16), fontsize=20).get_figure()
fig.savefig('test.pdf')