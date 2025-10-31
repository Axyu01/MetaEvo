import os
import csv
import re
import statistics
from collections import defaultdict

INPUT_DIR = "out/CVRP_files"
OUTPUT_DIR = "report_output"
os.makedirs(OUTPUT_DIR, exist_ok=True)

# -----------------------------
# Pomocnicze funkcje
# -----------------------------

def read_csv(filepath):
    """Wczytuje dane z pliku CSV wygenerowanego przez SolutionsLogger."""
    values = []
    with open(filepath, encoding="utf-8") as f:
        reader = csv.reader(f, delimiter=";")
        next(reader, None)  # pomiń nagłówek
        for row in reader:
            if len(row) >= 1 and row[0].strip():
                try:
                    val = float(row[0])
                    values.append(val)
                except ValueError:
                    continue
    return values


def analyze_file(filepath):
    """Analizuje pojedynczy plik CSV."""
    values = read_csv(filepath)
    if not values:
        return None
    return {
        "file": filepath,
        "count": len(values),
        "avg": statistics.mean(values),
        "best": min(values),
        "worst": max(values),
        "std": statistics.stdev(values) if len(values) > 1 else 0.0
    }


def find_csv_files(base_dir):
    """Zwraca listę wszystkich plików CSV z podkatalogów random, gready, evo."""
    csv_files = []
    for root, _, files in os.walk(base_dir):
        for f in files:
            if f.endswith(".csv"):
                csv_files.append(os.path.join(root, f))
    return csv_files


def group_by_instance(data_list):
    """Grupuje dane po instancjach i algorytmach."""
    grouped = defaultdict(lambda: {"random": None, "gready": None, "evo": []})
    for d in data_list:
        path = d["file"].lower()
        # Wydobycie nazwy instancji np. A-n32-k5
        m = re.search(r"a-n\d+-k\d+", path)
        if not m:
            continue
        instance = m.group(0)

        if "random" in path:
            grouped[instance]["random"] = d
        elif "gready" in path:
            grouped[instance]["gready"] = d
        elif "evo" in path:
            grouped[instance]["evo"].append(d)
    return grouped


# -----------------------------
# Tworzenie tabeli LaTeX
# -----------------------------

def save_latex_table(grouped):
    lines = []
    lines.append("\\begin{table}[h!]")
    lines.append("\\centering")
    lines.append("\\caption{Porównanie wyników algorytmów CVRP}")
    lines.append("\\begin{tabular}{lccccccccc}")
    lines.append("\\hline")
    lines.append("Instancja & \\multicolumn{3}{c}{Random [10k]} & \\multicolumn{3}{c}{Greedy [n]} & \\multicolumn{3}{c}{EA [10x]}\\\\")
    lines.append("& best & avg & std & best & avg & std & best & avg & std\\\\")
    lines.append("\\hline")

    def fmt(x):
        return f"{x:.2f}" if isinstance(x, (int, float)) else "-"

    for inst, algs in grouped.items():
        rand = algs["random"]
        gre = algs["gready"]
        evo_list = algs["evo"]

        # EA: zbieramy najlepsze wyniki z 10 uruchomień
        if evo_list:
            bests = [e["best"] for e in evo_list]
            evobest = min(bests)
            evoavg = statistics.mean(bests)
            evostd = statistics.stdev(bests) if len(bests) > 1 else 0
        else:
            evobest = evoavg = evostd = "-"

        line = (
            f"{inst} & "
            f"{fmt(rand['best'] if rand else '-')} & {fmt(rand['avg'] if rand else '-')} & {fmt(rand['std'] if rand else '-')} & "
            f"{fmt(gre['best'] if gre else '-')} & {fmt(gre['avg'] if gre else '-')} & {fmt(gre['std'] if gre else '-')} & "
            f"{fmt(evobest)} & {fmt(evoavg)} & {fmt(evostd)} \\\\"
        )
        lines.append(line)

    lines.append("\\hline")
    lines.append("\\end{tabular}")
    lines.append("\\end{table}")

    with open(os.path.join(OUTPUT_DIR, "table_results.tex"), "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
    print("[OK] Zapisano tabelę LaTeX: report_output/table_results.tex")


# -----------------------------
# Główna funkcja
# -----------------------------

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

    grouped = group_by_instance(data_all)
    save_latex_table(grouped)

    # Tworzymy prosty summary.txt
    with open(os.path.join(OUTPUT_DIR, "summary.txt"), "w", encoding="utf-8") as f:
        for inst, algs in grouped.items():
            f.write(f"=== {inst} ===\n")
            for name, data in algs.items():
                if not data:
                    continue
                if isinstance(data, list):  # evo
                    f.write(f"{name} ({len(data)} trials)\n")
                    for e in data:
                        f.write(f"  {os.path.basename(e['file'])}: best={e['best']:.2f}, avg={e['avg']:.2f}, std={e['std']:.2f}\n")
                else:
                    f.write(f"{name}: best={data['best']:.2f}, avg={data['avg']:.2f}, std={data['std']:.2f}\n")
            f.write("\n")

    print("[OK] Analiza zakończona. Wyniki w folderze:", OUTPUT_DIR)


if __name__ == "__main__":
    main()
