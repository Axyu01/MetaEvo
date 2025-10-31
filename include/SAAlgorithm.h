#ifndef SAALGORITHM_H
#define SAALGORITHM_H

#include "Solution.h"
#include "CVRProblem.h"
#include "SolutionsLogger.h"
#include <random>
#include <string>

class SAAlgorithm
{
public:
    // === Konstruktor i destruktor ===
    SAAlgorithm(CVRProblem& problem);
    virtual ~SAAlgorithm();

    // === Główne pola ===
    CVRProblem* problem;       // problem CVRP/TSP
    Solution* solution;        // aktualne rozwiązanie
    Solution* bestSolution;    // najlepsze znalezione rozwiązanie

    // === Parametry SA ===
    double startTemp = 1000.0;     // temperatura początkowa
    double minTemp = 1e-3;         // minimalna temperatura
    double alpha = 0.995;          // współczynnik chłodzenia (0 < alpha < 1)
    int iterationsPerTemp = 1000;  // liczba prób dla jednej temperatury
    int maxNoImprove = 2000;       // opcjonalny limit braku poprawy

    // === RNG ===
    std::mt19937 rng;
    std::uniform_real_distribution<double> uniform;

    // === Główne metody ===
    void Init();                   // inicjalizacja losowego rozwiązania
    void Iterate();                // klasyczna iteracja SA (bez logowania)
    void IterateWithLogging(SolutionsLogger& logger, int logInterval = 1000); // wersja z logowaniem
    void LoopUntil(double minTemperature);  // pętla aż do osiągnięcia T < minTemperature

private:
    // === Pomocnicze metody ===
    Solution* Neighbor(Solution* s);       // generuje sąsiedztwo (np. swap)
    double AcceptanceProbability(double oldVal, double newVal, double T);
    void CopySolution(Solution*& dest, const Solution* src);
    void PrintStatus(double T, int iter, double bestVal); // log w konsoli
};

#endif // SAALGORITHM_H
