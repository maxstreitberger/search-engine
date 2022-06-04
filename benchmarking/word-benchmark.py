from lorem_text import lorem
import sys
import os
import shutil
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import csv
from scipy import stats
import numpy as np

folder_dir = os.getcwd()
otf_times = {}

running_times = {"On-The-Fly": [], "Pre-Computed": [], "Words": []}
word_count = 1000

for num_run in range(100):
    running_times["Words"] += [word_count]
    dirName = "run_" + str(num_run + 1)
    try:
        os.mkdir(dirName)
    except FileExistsError:
        print("Directory " , dirName ,  " already exists")
    run_dir = folder_dir + "/" + dirName + "/"
    for num_docs in range(1):
        file_name = run_dir + "doc_" + str(num_docs+1) + ".txt"
        file = open(file_name, "w+")
        file.write(lorem.words(word_count))          # 100 words ~750bytes
        file.close()
    subprocess.run([folder_dir + "/bin/benchmarking/OnTheFlyDocumentSearch/OnTheFlyDocumentSearch_benchmarking", run_dir])

    times = []

    with open(folder_dir + "/OnTheFlyDocumentSearch_benchmarking.csv", 'r') as fil:
        tmp = fil.read().split('\n')
        tmp = tmp[1:-1]
        times = [float(i) for i in tmp]
    times_mean = pd.Series(times).mean()
    print("OTF mean: " + str(times_mean))
    running_times["On-The-Fly"] += [times_mean]

    times = []
    
    subprocess.run([folder_dir + "/bin/benchmarking/PreComputedDocumentSearch/PreComputedDocumentSearch_benchmarking", run_dir])
    with open(folder_dir + "/PreComputedDocumentSearch_benchmarking.csv", 'r') as fil:
        tmp = fil.read().split('\n')
        tmp = tmp[1:-1]
        times = [float(i) for i in tmp]
    times_mean = pd.Series(times).mean()
    print("PC mean: " + str(times_mean))
    running_times["Pre-Computed"] += [times_mean]


    try:
        shutil.rmtree(dirName)
    except OSError as e:
        print("Error: %s : %s" % (dirName, e.strerror))

    word_count += 1000

df = pd.DataFrame(running_times)
df.set_index("Words", inplace=True)
fig = df.plot(kind="line", xticks=[1000, 100000], ylabel="Running Times", subplots=True)[0].get_figure()
fig.savefig("words.pdf")
