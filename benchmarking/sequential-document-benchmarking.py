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

running_times = {"On-The-Fly": [], "Pre-Computed": [], "Documents": []}
doc_count = 1000

for num_run in range(10):
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
    subprocess.run([folder_dir + "/bin/benchmarking/OnTheFlyDocumentSearch/OnTheFlyDocumentSearch_benchmarking", run_dir])

    times = []

    with open(folder_dir + "/OnTheFlyDocumentSearch_benchmarking.csv", 'r') as fil:
        tmp = fil.read().split('\n')
        tmp = tmp[1:-1]
        times = [float(i) for i in tmp]
    times_mean = pd.Series(times).mean()
    print("OTF mean: " + str(times_mean))
    running_times["On-The-Fly"] += [times_mean]
    # otf_times[str(num_run + 1)] = times

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

    doc_count += 1000

    # pc_times[str(num_run + 1)] = times

# df = pd.DataFrame(otf_times)
# fig = df.plot(kind='box', figsize=(20, 16), fontsize=20).get_figure()
# fig.savefig("otf_" + str(num_run + 1) + ".pdf")

# df = pd.DataFrame(pc_times)
# fig = df.plot(kind='box', figsize=(20, 16), fontsize=20).get_figure()
# fig.savefig("pc_" + str(num_run + 1) + ".pdf")




df = pd.DataFrame(running_times)
df.set_index("Documents", inplace=True)
fig = df.plot.line(xticks=df.index, subplots=True, xlabel="Documents", ylabel="Running Times")[0].get_figure()
fig.savefig("running_times.pdf")
