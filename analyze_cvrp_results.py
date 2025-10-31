import os
import csv
import re
import statistics
from collections import defaultdict
import matplotlib.pyplot as plt

# ======= KONFIGURACJA =======
INPUT_DIR = "out/CVRP_files"
OUTPUT_DIR = "report_output"
os.makedirs(OUTPUT_DIR, exist_ok=True)
# =============================

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
    std_val = statistics.stdev(values) if len(values) > 1 else 0.0

    return {
        "file": filepath,
        "count": len(values),
        "avg": avg_val,
        "best": best_val,
        "worst": worst_val,
        "std": std_val,
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


def compute_stats(data_list):
    """Zwraca statystyki dla danej grupy algorytmu."""
    if not data_list:
        return {"best": "-", "avg": "-", "std": "-"}
    best = min(d["best"] for d in data_list)
    avg = statistics.mean(d["best"] for d in data_list)
    std = statistics.stdev(d["best"] for d in data_list) if len(data_list) > 1 else 0
    return {"best": best, "avg": avg, "std": std}


def group_by_instance(data_list):
    """Grupuje wyniki wg instancji i algorytmu."""
    grouped = defaultdict(lambda: defaultdict(list))
    for d in data_list:
        rel = os.path.relpath(d["file"], INPUT_DIR)
        name = rel.lower()

        instance = re.split(r"[/\\]", rel)[0]
        if "random" in name:
            grouped[instance]["random"].append(d)
        elif "greedy" in name:
            grouped[instance]["greedy"].append(d)
        elif "evo" in name:
            grouped[instance]["evo"].append(d)
    return grouped


def save_latex_table(grouped):
    """Tworzy tabelę LaTeX z wynikami (bez SA)."""
    latex = []
    latex.append("\\begin{table}[h!]")
    latex.append("\\centering")
    latex.append("\\caption{Porównanie wyników algorytmów}")
    latex.append("\\begin{tabular}{lccccccccc}")
    latex.append("\\hline")
    latex.append("Instancja & \\multicolumn{3}{c}{Random [10k]} & \\multicolumn{3}{c}{Greedy [n]} & "
                 "\\multicolumn{3}{c}{EA [10x]}\\\\")
    latex.append("& best & avg & std & best & avg & std & best & avg & std\\\\")
    latex.append("\\hline")

    def fmt(v):
        return f"{v:.2f}" if isinstance(v, (int, float)) else str(v)

    for instance, alg_data in grouped.items():
        def get_alg(name):
            for key in alg_data.keys():
                if name.lower() in key.lower():
                    return alg_data[key]
            return []

        rand = compute_stats(get_alg("random"))
        greedy = compute_stats(get_alg("greedy"))
        evo = compute_stats(get_alg("evo"))

        latex.append(
            f"{instance} & {fmt(rand['best'])} & {fmt(rand['avg'])} & {fmt(rand['std'])} & "
            f"{fmt(greedy['best'])} & {fmt(greedy['avg'])} & {fmt(greedy['std'])} & "
            f"{fmt(evo['best'])} & {fmt(evo['avg'])} & {fmt(evo['std'])} \\\\"
        )

    latex.append("\\hline")
    latex.append("\\end{tabular}")
    latex.append("\\end{table}")

    with open(os.path.join(OUTPUT_DIR, "table_results.tex"), "w", encoding="utf-8") as f:
        f.write("\n".join(latex))

    print("[OK] Zapisano tabelę: report_output/table_results.tex")


def main():
    print(f"[INFO] Analiza wyników w katalogu: {INPUT_DIR}")
    csv_files = find_csv_files(INPUT_DIR)
    if not csv_files:
        print("[WARN] Nie znaleziono plików CSV!")
        return

    data_all = []
    for path in csv_files:
        name = re.sub(r"[\\/]", "_", os.path.relpath(path, INPUT_DIR).replace(".csv", ""))
        d = analyze_file(path)
        if d:
            data_all.append(d)
            if "evo" in name:
                save_evolution_plot(name, d["notes"], d["values"])

    grouped = group_by_instance(data_all)
    save_latex_table(grouped)

    # zapis tekstowy
    with open(os.path.join(OUTPUT_DIR, "summary.txt"), "w", encoding="utf-8") as f:
        for d in data_all:
            f.write(f"{d['file']}: best={d['best']:.2f}, avg={d['avg']:.2f}, worst={d['worst']:.2f}\n")

    print("[OK] Analiza zakończona. Wyniki w folderze:", OUTPUT_DIR)


if __name__ == "__main__":
    main()
