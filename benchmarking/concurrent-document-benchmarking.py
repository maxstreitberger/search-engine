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
pc_times = {}

running_times = {"On-The-Fly": [], "Documents": []}
doc_count = 10

for num_run in range(1):
    print("Benchmark nr. " + str(num_run+1))
    print("Document count: " + str(doc_count))
    running_times["Documents"] += [doc_count];
    dirName = "run_" + str(num_run + 1)
    try:
        os.mkdir(dirName)
    except FileExistsError:
        print("Directory " , dirName ,  " already exists")
    run_dir = folder_dir + "/" + dirName + "/"
    for num_docs in range(doc_count):
        file_name = run_dir + "doc_" + str(num_docs+1) + ".txt"
        file = open(file_name, "w+")
        file.write(lorem.words(100))          # 100 words ~750bytes
        file.close()
    subprocess.run([folder_dir + "/bin/benchmarking/ConcurrentOnTheFlyDocumentSearch/ConcurrentOnTheFlyDocumentSearch_benchmarking", run_dir])

    times = []

    with open(folder_dir + "/ConcurrentOnTheFlyDocumentSearch_benchmarking.csv", 'r') as fil:
        tmp = fil.read().split('\n')
        tmp = tmp[1:-1]
        times = [float(i) for i in tmp]
    times_mean = pd.Series(times).mean()
    print("OTF mean: " + str(times_mean))
    running_times["On-The-Fly"] += [times_mean]
    
    try:
        shutil.rmtree(dirName)
    except OSError as e:
        print("Error: %s : %s" % (dirName, e.strerror))

    doc_count += 1000

df = pd.DataFrame(running_times)
df.set_index("Documents", inplace=True)
fig = df.plot.line(xticks=df.index, xlabel="Documents", ylabel="Running Times").get_figure()
fig.savefig("running_times.pdf")
