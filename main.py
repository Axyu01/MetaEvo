import os
import csv
import re
import statistics
from collections import defaultdict
import matplotlib.pyplot as plt
import analyze_csv

# ======= KONFIGURACJA =======
INPUT_DIR = "out/CVRP_files"
OUTPUT_DIR = "report_output"
os.makedirs(OUTPUT_DIR, exist_ok=True)
# =============================

INSTANCE_NAMES = ["a-n32-k5","a-n37-k6","a-n39-k5","a-n45-k6","a-n48-k7","a-n54-k7","a-n60-k9"]
OPT_SOLUTIONS = [784,949,822,944,1073,1167,1354]
INSTANCE_INFO = {}
def save_best_genomes(INSTANCE_INFO, output_dir="report_output"):
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, "summary_genomes.txt")
    
    with open(output_path, "w", encoding="utf-8") as f:
        for inst_name, data in INSTANCE_INFO.items():
            f.write(f"=== Instancja: {inst_name} (opt = {data['opt']}) ===\n\n")
            
            for alg in ["random", "gready", "evo", "sa"]:
                alg_data = data[alg]
                
                f.write(f"[{alg.upper()}] Najlepszy genom ogólny:\n")
                f.write(f"  best = {alg_data['best']}, genome = {alg_data['bestGenome']}\n")
                
                # Jeśli algorytm ma triale, wypisz każdy trial
                if alg in ["evo", "sa"] and alg_data["trials"]:
                    f.write("  Wszystkie triale:\n")
                    for trial_name, trial in sorted(alg_data["trials"].items()):
                        genome = trial.get("bestGenome", "--")
                        value = trial.get("best", "--")
                        f.write(f"    {trial_name}: best = {value}, genome = {genome}\n")
            
            f.write("\n")

    print(f"[OK] Podsumowanie genomów zapisane do: {output_path}")

def generate_latex_table(INSTANCE_INFO, output_dir="report_output"):
    os.makedirs(output_dir, exist_ok=True)
    
    lines = []
    lines.append("\\begin{table}[h!]")
    lines.append("\\centering")
    lines.append("\\caption{Porównanie wyników algorytmów dla CVRP}")
    lines.append("\\resizebox{\\textwidth}{!}{%")
    lines.append("\\begin{tabular}{lccccccccccccccccc}")
    lines.append("\\hline")
    lines.append("Instancja & opt & \\multicolumn{4}{c}{Random [10k]} & \\multicolumn{4}{c}{Greedy [n]} & \\multicolumn{4}{c}{EA [10x]} & \\multicolumn{4}{c}{SA [10x]} \\\\")
    lines.append("&  & best & worst & avg & std & best & worst & avg & std & best & worst & avg & std & best & worst & avg & std \\\\")
    lines.append("\\hline")

    for name, data in INSTANCE_INFO.items():
        opt = data["opt"]

        random = data["random"]
        gready = data["gready"]
        evo = data["evo"]
        sa = data["sa"]

        def fmt(x): 
            return f"{x:.0f}" if isinstance(x, (int, float)) else "--"

        row = (
            f"{name} & {opt} & "
            f"{fmt(random['best'])} & {fmt(random['worst'])} & {fmt(random['avg'])} & {fmt(random['std'])} & "
            f"{fmt(gready['best'])} & {fmt(gready['worst'])} & {fmt(gready['avg'])} & {fmt(gready['std'])} & "
            f"{fmt(evo['best'])} & {fmt(evo['worst'])} & {fmt(evo['avg'])} & {fmt(evo['std'])} & "
            f"{fmt(sa['best'])} & {fmt(sa['worst'])} & {fmt(sa['avg'])} & {fmt(sa['std'])} \\\\"
        )
        lines.append(row)

    lines.append("\\hline")
    lines.append("\\end{tabular}")
    lines.append("}")
    lines.append("\\end{table}")

    latex_table = "\n".join(lines)

    # zapis do pliku
    output_path = os.path.join(output_dir, "table_results.tex")
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(latex_table)

    print(f"[OK] Tabela LaTeX zapisana do: {output_path}")

def init_instance_info():
    global INSTANCE_INFO
    INSTANCE_INFO = {}

    for name, opt in zip(INSTANCE_NAMES, OPT_SOLUTIONS):
        INSTANCE_INFO[name] = {
            "opt": opt,
            "evo": {"trials": {}, "best": 0, "bestGenome": None, "worst": 0, "avg": 0, "std": 0},
            "sa":  {"trials": {}, "best": 0, "bestGenome": None, "worst": 0, "avg": 0, "std": 0},
            "gready": {"best": 0, "bestGenome": None, "worst": 0, "avg": 0, "std": 0},
            "random": {"best": 0, "bestGenome": None, "worst": 0, "avg": 0, "std": 0},
        }
def analyze_trials():
    global INSTANCE_INFO

    for inst, data in INSTANCE_INFO.items():
        # Evolutionary Algorithm
        evo_trials = data["evo"]["trials"]
        if evo_trials:
            best_values = [t["best"] for t in evo_trials.values()]
            best, worst, avg, std = analyze_csv.analyze_best(best_values)
            INSTANCE_INFO[inst]["evo"]["best"] = best
            INSTANCE_INFO[inst]["evo"]["worst"] = worst
            INSTANCE_INFO[inst]["evo"]["avg"] = avg
            INSTANCE_INFO[inst]["evo"]["std"] = std

        # Simulated Annealing
        sa_trials = data["sa"]["trials"]
        if sa_trials:
            best_values = [t["best"] for t in sa_trials.values()]
            best, worst, avg, std = analyze_csv.analyze_best(best_values)
            INSTANCE_INFO[inst]["sa"]["best"] = best
            INSTANCE_INFO[inst]["sa"]["worst"] = worst
            INSTANCE_INFO[inst]["sa"]["avg"] = avg
            INSTANCE_INFO[inst]["sa"]["std"] = std


def find_csv_files(base_dir):
    csv_files = []
    for root, _, files in os.walk(base_dir):
        for f in files:
            if f.endswith(".csv"):
                csv_files.append(os.path.join(root, f))
    return csv_files

def main():
    init_instance_info()
    print(f"[INFO] Analiza wyników w katalogu: {INPUT_DIR}")
    csv_files = find_csv_files(INPUT_DIR)
    if not csv_files:
        print("[WARN] Nie znaleziono plików CSV!")
        return


    for path in csv_files:
        file_name = os.path.basename(path)[:-4]#removing .csv
        lower_path = path.lower()

        instance_name = None
        for inst in INSTANCE_NAMES:
            if inst.strip().lower() in lower_path:
                instance_name = inst.strip()
                break

        if "evo" in path.lower():
            print(path)
            #print(file_name)
            bestVal,bestGenome,worstVal = analyze_csv.analyze_csv_evo(path,"evo",file_name,True)
            INSTANCE_INFO[instance_name]["evo"]["trials"][file_name] = {
                "best": bestVal,
                "bestGenome": bestGenome,
                "worst": worstVal
            }
        elif "sa" in path.lower():
            print(path)
            bestVal,bestGenome,worstVal,avg,std = analyze_csv.analyze_csv(path,"sa",file_name,True)
            INSTANCE_INFO[instance_name]["sa"]["trials"][file_name] = {
                "best": bestVal,
                "bestGenome": bestGenome,
                "worst": worstVal,
                "avg": avg,
                "std": std
            }
        elif "gready"  in path.lower():
            print(path)
            bestVal,bestGenome,worstVal,avg,std = analyze_csv.analyze_csv(path,"gready",file_name)
            INSTANCE_INFO[instance_name]["gready"]= {
                "best": bestVal,
                "bestGenome": bestGenome,
                "worst": worstVal,
                "avg": avg,
                "std": std
            }
        elif "random" in path.lower():
            print(path)
            bestVal,bestGenome,worstVal,avg,std = analyze_csv.analyze_csv(path,"random",file_name)
            INSTANCE_INFO[instance_name]["random"]= {
                "best": bestVal,
                "bestGenome": bestGenome,
                "worst": worstVal,
                "avg": avg,
                "std": std
            }
    analyze_trials()
    generate_latex_table(INSTANCE_INFO,OUTPUT_DIR)
    save_best_genomes(INSTANCE_INFO,OUTPUT_DIR)

    print("[OK] Analiza zakończona. Wyniki w folderze:", OUTPUT_DIR)


if __name__ == "__main__":
    main()
