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
    """Wczytuje dane z pliku CSV zapisanych przez SolutionsLogger (Value;Genome;Note)."""
    values, genomes, notes = [], [], []
    with open(filepath, encoding="utf-8") as f:
        reader = csv.reader(f, delimiter=";")
        next(reader, None)  # pomijamy nagłówek
        for row in reader:
            if len(row) >= 3 and row[0].strip():
                try:
                    val = float(row[0])
                    genome = row[1].strip()
                    note = row[2].strip()
                    values.append(val)
                    genomes.append(genome)
                    notes.append(note)
                except ValueError:
                    continue
    return values, genomes, notes


def analyze_file(filepath):
    """Analizuje pojedynczy plik CSV."""
    values, genomes, notes = read_csv(filepath)
    if not values:
        return None

    avg_val = statistics.mean(values)
    best_val = min(values)
    worst_val = max(values)
    std_val = statistics.stdev(values) if len(values) > 1 else 0.0

    # znajdź genom najlepszego rozwiązania
    idx_best = values.index(best_val)
    best_genome = genomes[idx_best] if idx_best < len(genomes) else "-"

    return {
        "file": filepath,
        "count": len(values),
        "avg": avg_val,
        "best": best_val,
        "best_genome": best_genome,
        "worst": worst_val,
        "std": std_val,
        "values": values,
        "genomes": genomes,
        "notes": notes
    }


def find_csv_files(base_dir):
    """Znajduje wszystkie pliki CSV w katalogu bazowym."""
    csv_files = []
    for root, _, files in os.walk(base_dir):
        for f in files:
            if f.endswith(".csv"):
                csv_files.append(os.path.join(root, f))
    return csv_files


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


def compute_stats(data_list, alg_name):
    """Zwraca statystyki z uwzględnieniem różnic między algorytmami."""
    if not data_list:
        return {"best": "-", "worst": "-", "avg": "-", "std": "-"}

    # Dla Random i Greedy – tylko jeden plik
    if alg_name in ("random", "greedy"):
        d = data_list[0]
        return {
            "best": d["best"],
            "worst": d["worst"],
            "avg": d["avg"],
            "std": d["std"]
        }

    # Dla Evo i SA – wiele plików (np. 10 triali)
    bests = [d["best"] for d in data_list]
    worst = max(bests)
    best = min(bests)
    avg = statistics.mean(bests)
    std = statistics.stdev(bests) if len(bests) > 1 else 0
    return {"best": best, "worst": worst, "avg": avg, "std": std}


def group_by_instance(data_list):
    """Grupuje dane wg instancji i algorytmu."""
    grouped = defaultdict(lambda: defaultdict(list))
    for d in data_list:
        rel = os.path.relpath(d["file"], INPUT_DIR)
        name = rel.lower()
        match = re.search(r"(a-n\d+-k\d+)", name)
        instance = match.group(1) if match else rel

        if "random" in name:
            grouped[instance]["random"].append(d)
        elif "gready" in name or "greedy" in name:
            grouped[instance]["greedy"].append(d)
        elif "evo" in name:
            grouped[instance]["evo"].append(d)
        elif "sa" in name:
            grouped[instance]["sa"].append(d)
    return grouped


def save_latex_table(grouped):
    """Tworzy tabelę LaTeX z kolumnami best/worst/avg/std."""
    latex = []
    latex.append("\\begin{table}[h!]")
    latex.append("\\centering")
    latex.append("\\caption{Porównanie wyników algorytmów dla CVRP}")
    latex.append("\\begin{tabular}{lcccccccccccccccc}")
    latex.append("\\hline")
    latex.append(
        "Instancja & \\multicolumn{4}{c}{Random [10k]} & "
        "\\multicolumn{4}{c}{Greedy [n]} & "
        "\\multicolumn{4}{c}{EA [10x]} & "
        "\\multicolumn{4}{c}{SA [10x]} \\\\"
    )
    latex.append(
        "& best & worst & avg & std "
        "& best & worst & avg & std "
        "& best & worst & avg & std "
        "& best & worst & avg & std \\\\"
    )
    latex.append("\\hline")

    def fmt(v):
        if isinstance(v, (int, float)):
            return f"{int(round(v))}"
        return str(v)

    for instance, alg_data in sorted(grouped.items()):
        rand = compute_stats(alg_data.get("random", []), "random")
        greedy = compute_stats(alg_data.get("greedy", []), "greedy")
        evo = compute_stats(alg_data.get("evo", []), "evo")
        sa = compute_stats(alg_data.get("sa", []), "sa")

        latex.append(
            f"{instance} & "
            f"{fmt(rand['best'])} & {fmt(rand['worst'])} & {fmt(rand['avg'])} & {fmt(rand['std'])} & "
            f"{fmt(greedy['best'])} & {fmt(greedy['worst'])} & {fmt(greedy['avg'])} & {fmt(greedy['std'])} & "
            f"{fmt(evo['best'])} & {fmt(evo['worst'])} & {fmt(evo['avg'])} & {fmt(evo['std'])} & "
            f"{fmt(sa['best'])} & {fmt(sa['worst'])} & {fmt(sa['avg'])} & {fmt(sa['std'])} \\\\"
        )

    latex.append("\\hline")
    latex.append("\\end{tabular}")
    latex.append("\\end{table}")

    with open(os.path.join(OUTPUT_DIR, "table_results.tex"), "w", encoding="utf-8") as f:
        f.write("\n".join(latex))

    print("[OK] Zapisano tabelę LaTeX: report_output/table_results.tex")


def main():
    print(f"[INFO] Analiza wyników w katalogu: {INPUT_DIR}")
    csv_files = find_csv_files(INPUT_DIR)
    if not csv_files:
        print("[WARN] Nie znaleziono plików CSV!")
        return

    data_all = []
    for path in csv_files:
        d = analyze_file(path)
        if d:
            data_all.append(d)
            name = re.sub(r"[\\/]", "_", os.path.relpath(path, INPUT_DIR).replace(".csv", ""))

            # Rysowanie wykresów
            if "evo" in name.lower():
                save_evolution_plot(name, d["notes"], d["values"], f"Evolutionary Progress: {name}")
            elif "sa" in name.lower():
                save_evolution_plot(name, d["notes"], d["values"], f"SA Progress: {name}")

    grouped = group_by_instance(data_all)
    save_latex_table(grouped)

    # summary.txt z genomami
    with open(os.path.join(OUTPUT_DIR, "summary.txt"), "w", encoding="utf-8") as f:
        for d in data_all:
            alg = "Unknown"
            name = d["file"].lower()
            if "evo" in name:
                alg = "Evolutionary"
            elif "sa" in name:
                alg = "Simulated Annealing"
            elif "greedy" in name or "gready" in name:
                alg = "Greedy"
            elif "random" in name:
                alg = "Random"

            f.write(
                f"{alg} | {d['file']}:\n"
                f"  best   = {d['best']:.2f}\n"
                f"  avg    = {d['avg']:.2f}\n"
                f"  worst  = {d['worst']:.2f}\n"
                f"  genome = {d['best_genome']}\n\n"
            )

    print("[OK] Analiza zakończona. Wyniki w folderze:", OUTPUT_DIR)


if __name__ == "__main__":
    main()
