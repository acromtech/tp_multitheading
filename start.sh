#!/bin/bash

uv sync

# Activer le mode d'arrêt en cas d'erreur
set -e

# Fonction pour tuer tous les processus en cours en cas d'interruption
cleanup() {
    echo "Arrêt de tous les processus en cours..."
    kill $BOSS_PID $MINION_PID $BOSS_CPP_PID $QUEUE_PID $PROXY_PID $LOW_LEVEL_PID 2>/dev/null || true
    wait $BOSS_PID $MINION_PID $BOSS_CPP_PID $QUEUE_PID $PROXY_PID $LOW_LEVEL_PID 2>/dev/null || true
    echo "Tous les processus ont été arrêtés."
}

# Détecter un Ctrl+C (SIGINT) ou une erreur (EXIT) et appeler cleanup()
trap cleanup EXIT SIGINT

# Nettoyer l'écran
clear

echo "Compilation du programme C++..."
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build
echo "Compilation terminée."
echo ""
echo ""
echo "Lancement de QueueManager.py et Proxy.py..."
echo ""

uv run QueueManager.py > /dev/null 2>&1 &
QUEUE_PID=$!

uv run proxy.py > /dev/null 2>&1 &
PROXY_PID=$!

sleep 2

echo ""
echo "Lancement de Boss.py et Minion.py..."
echo ""
uv run Boss.py &
BOSS_PID=$!

uv run Minion.py > /dev/null 2>&1 &
MINION_PID=$!

wait $BOSS_PID
wait $MINION_PID

echo ""
echo "Boss.py et Minion.py sont terminés."
echo ""

sleep 5

echo ""
echo ""
echo "Lancement de Boss.py et low_level.cpp..."
echo ""
uv run Boss.py &
BOSS_CPP_PID=$!

./build/low_level > /dev/null 2>&1 &
LOW_LEVEL_PID=$!

wait $BOSS_CPP_PID

cleanup

echo "Tous les processus sont terminés."
