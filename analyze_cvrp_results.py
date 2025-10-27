import os
import csv
import re
import statistics
import matplotlib.pyplot as plt
from collections import defaultdict

INPUT_DIR = "out/CVRP_files"
OUTPUT_DIR = "report_output"

os.makedirs(OUTPUT_DIR, exist_ok=True)

def read_csv(filepath):
    """Wczytuje dane z pliku CSV zapisanych przez SolutionsLogger."""
    values = []
    notes = []
    with open(filepath, encoding="utf-8") as f:
        reader = csv.reader(f, delimiter=";")
        next(reader, None)  # pomiń nagłówek
        for row in reader:
            if len(row) >= 3 and row[0].strip():
                try:
                    val = float(row[0])
                    note = row[2].strip()
                    values.append(val)
                    notes.append(note)
                except ValueError:
                    continue
    return values, notes

def analyze_file(filepath):
    """Analizuje pojedynczy plik CSV."""
    values, notes = read_csv(filepath)
    if not values:
        return None

    avg_val = statistics.mean(values)
    best_val = min(values)
    worst_val = max(values)
    return {
        "file": filepath,
        "count": len(values),
        "avg": avg_val,
        "best": best_val,
        "worst": worst_val,
        "values": values,
        "notes": notes
    }

def find_csv_files(base_dir):
    """Rekurencyjnie znajduje wszystkie pliki CSV."""
    csv_files = []
    for root, _, files in os.walk(base_dir):
        for f in files:
            if f.endswith(".csv"):
                csv_files.append(os.path.join(root, f))
    return csv_files

def save_histogram(name, values):
    """Rysuje histogram rozkładu wartości."""
    plt.figure()
    plt.hist(values, bins=30)
    plt.xlabel("Value")
    plt.ylabel("Frequency")
    plt.title(name)
    outpath = os.path.join(OUTPUT_DIR, f"{name}_hist.png")
    plt.savefig(outpath)
    plt.close()

def save_evolution_plot(name, notes, values):
    """Dla algorytmu ewolucyjnego: wykres best/avg/worst po iteracjach."""
    iter_groups = defaultdict(list)
    for val, note in zip(values, notes):
        if not note:
            continue
        try:
            iter_groups[int(note)].append(val)
        except ValueError:
            continue

    if not iter_groups:
        return  # brak iteracji (nie evo)

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
    plt.title(f"Evolution Progress: {name}")
    plt.legend()
    plt.grid(True, linestyle=":")
    outpath = os.path.join(OUTPUT_DIR, f"{name}_evo.png")
    plt.savefig(outpath)
    plt.close()

def main():
    print(f"[INFO] Analiza wyników w katalogu: {INPUT_DIR}")
    csv_files = find_csv_files(INPUT_DIR)
    if not csv_files:
        print("[WARN] Nie znaleziono plików CSV!")
        return

    summary_lines = []
    latex_lines = [
        "\\section{Wyniki eksperymentów}",
        "Poniżej przedstawiono analizę wyników z plików CSV generowanych przez algorytmy."
    ]

    for path in csv_files:
        rel_name = os.path.relpath(path, INPUT_DIR)
        name = re.sub(r"[\\/]", "_", rel_name.replace(".csv", ""))
        data = analyze_file(path)
        if not data:
            continue

        summary_lines.append(
            f"{rel_name}: count={data['count']}, avg={data['avg']:.2f}, best={data['best']:.2f}, worst={data['worst']:.2f}"
        )

        # Histogram
        save_histogram(name, data["values"])

        # Wykres ewolucyjny (jeśli to plik z evo)
        if "evo" in rel_name.lower():
            save_evolution_plot(name, data["notes"], data["values"])

        # LaTeX
        latex_lines.append(f"\\subsection{{{rel_name}}}")
        latex_lines.append(
            f"Średnia wartość: {data['avg']:.2f}, najlepsza: {data['best']:.2f}, najgorsza: {data['worst']:.2f}."
        )

        latex_lines.append(f"\\includegraphics[width=0.8\\linewidth]{{report_output/{name}_hist.png}}")
        if "evo" in rel_name.lower():
            latex_lines.append(f"\\includegraphics[width=0.8\\linewidth]{{report_output/{name}_evo.png}}")

    # Zapis tekstowego podsumowania
    with open(os.path.join(OUTPUT_DIR, "summary.txt"), "w", encoding="utf-8") as f:
        f.write("\n".join(summary_lines))

    # Zapis LaTeX
    with open(os.path.join(OUTPUT_DIR, "summary_auto.tex"), "w", encoding="utf-8") as f:
        f.write("\n\n".join(latex_lines))

    print(f"[OK] Analiza zakończona. Wyniki zapisano w folderze: {OUTPUT_DIR}")

if __name__ == "__main__":
    main()
