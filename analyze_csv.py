import os
import csv
import re
import statistics
from collections import defaultdict
import matplotlib.pyplot as plt


INPUT_DIR = "out/CVRP_files"
OUTPUT_DIR = "report_output"

#dla gready/random/sa
def analyze_csv(filepath,alg_name,file_name,isPloted = False):
    values,notes = [], []
    bestVal = float('inf')
    worstVal = float('-inf')
    sum = 0
    count = 0
    bestGenome =""
    with open(filepath, encoding="utf-8") as f:
        reader = csv.reader(f, delimiter=";")
        next(reader, None)  # skip header
        for row in reader:
            if len(row) >= 3 and row[0].strip():
                try:
                    val = float(row[0])
                    genome = row[1].strip()
                    note = row[2].strip()

                    #store for avg and plot
                    values.append(val)
                    notes.append(note)

                    #find best
                    if(val<bestVal):
                        bestVal = val
                        bestGenome = genome

                    #find worst
                    if(val>worstVal):
                        worstVal = val

                    sum = sum + val
                    count = count + 1
                except ValueError:
                    continue
                
    avg = statistics.mean(values)
    std = statistics.stdev(values) if len(values) > 1 else 0.0

    if isPloted:
        name = name = alg_name+"_"+file_name
        title = name
        save_single_line_plot(name,notes,values,title)

    #print((bestVal,bestGenome,worstVal,avg,std))
    return bestVal,bestGenome,worstVal,avg,std

#dla evo
def analyze_csv_evo(filepath,alg_name,file_name,isPloted = False):
    bestValues,avarageValues,worstValues,notes = [], [], [], []
    bestVal = float('inf')
    worstVal = float('-inf')
    sum = 0
    count = 0
    bestGenome =""

    iterator = 0
    with open(filepath, encoding="utf-8") as f:
        reader = csv.reader(f, delimiter=";")
        next(reader, None)  # skip header
        for row in reader:
            if iterator == 0:#avarage
                val = float(row[1])
                avarageValues.append(val)

            elif iterator == 1:#best
                
                val = float(row[0])
                bestValues.append(val)
                genome = row[1].strip()
                note = row[2].strip()
                notes.append(note)#for later iteration

                #find best
                if(val<bestVal):
                    bestVal = val
                    bestGenome = genome
                pass
            elif iterator == 2:#worst
                
                val = float(row[0])
                worstValues.append(val)
                #genome = row[1].strip()
                #note = row[2].strip()
                
                 #find worst
                if(val>worstVal):
                    worstVal = val
                pass
            iterator = (iterator + 1)%3
    if isPloted:
        name = alg_name+"_"+file_name
        title = name
        save_evolution_plot(name,notes,bestValues,worstValues,avarageValues,title)

    return bestVal,bestGenome,worstVal

def analyze_best(values):
    best = min(values)
    worst = max(values)
    avg = statistics.mean(values)
    std = statistics.stdev(values) if len(values) > 1 else 0.0
    return best,worst,avg,std
    
def save_evolution_plot(name, notes, bestValues,worstValues,avarageValues, title):

    plt.figure()
    plt.plot(notes, bestValues, label="Best", linewidth=2)
    plt.plot(notes, worstValues, label="Worst", linewidth=2)
    plt.plot(notes, avarageValues, label="Average", linewidth=2)
    plt.xlabel("Iteration")
    plt.ylabel("Value")
    plt.title(title)
    plt.legend()
    plt.grid(True, linestyle=":")

    outpath = os.path.join(OUTPUT_DIR, f"{name}.png")
    plt.savefig(outpath)
    plt.close()

def save_single_line_plot(name, notes, values, title, ylabel="Value"):

    plt.figure()
    plt.plot(notes, values, label=ylabel, linewidth=2)
    plt.xlabel("Iteration")
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle=":")
    plt.legend()

    outpath = os.path.join(OUTPUT_DIR, f"{name}.png")
    plt.savefig(outpath)
    plt.close()
