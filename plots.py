import os
import csv
import re
import statistics
from collections import defaultdict
import matplotlib.pyplot as plt
import analyze_csv

INPUT_DIR = "out/CVRP_files"
OUTPUT_DIR = "report_output"

def save_plot(name, notes, values, title):
    """Tworzy wykres postępu (dla EA lub SA)."""
    iter_groups = defaultdict(list)
    for val, note in zip(values, notes):
        if not note:
            continue
        try:
            iter_groups[int(note)].append(val)
        except ValueError:
            continue

    if not iter_groups:
        return

    plt.figure()
    plt.plot(notes, values, label="Best", linewidth=2)
    plt.xlabel("Iteration")
    plt.ylabel("Value")
    plt.title(title)
    plt.legend()
    plt.grid(True, linestyle=":")
    outpath = os.path.join(OUTPUT_DIR, f"{name}_progress.png")
    plt.savefig(outpath)
    plt.close()


def save_evolution_plot(name, notes, values, title):
    """Tworzy wykres postępu (dla EA lub SA)."""
    iter_groups = defaultdict(list)
    for val, note in zip(values, notes):
        if not note:
            continue
        try:
            iter_groups[int(note)].append(val)
        except ValueError:
            continue

    if not iter_groups:
        return

    iterations = sorted(iter_groups.keys())
    avg_vals = [statistics.mean(iter_groups[i]) for i in iterations]
    best_vals = [min(iter_groups[i]) for i in iterations]
    worst_vals = [max(iter_groups[i]) for i in iterations]

    plt.figure()
    plt.plot(iterations, best_vals, label="Best", linewidth=2)
    plt.plot(iterations, avg_vals, label="Average", linestyle="--")
    plt.plot(iterations, worst_vals, label="Worst", linestyle=":")
    plt.xlabel("Iteration")
    plt.ylabel("Value")
    plt.title(title)
    plt.legend()
    plt.grid(True, linestyle=":")
    outpath = os.path.join(OUTPUT_DIR, f"{name}_progress.png")
    plt.savefig(outpath)
    plt.close()