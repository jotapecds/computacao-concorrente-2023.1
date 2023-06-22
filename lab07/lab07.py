from threading import Thread
import math
import time
import sys

class ThreadCalculaPi(Thread):
    def __init__(self, inicio, series):
        super().__init__()
        self.inicio = inicio
        self.series = series
        self.somatorio = 0.0

    # Função da thread: Calcular n séries de pi {self.series} a partir de um ponto determinado {self.inicio}
    def run(self):
        for n in range(self.inicio, self.inicio + self.series):
            self.somatorio += math.pow(-1, n) / (2*n + 1)

def calcular_pi_concorrente(num_series, num_threads):
    # Reparte igualmente a quantidade de séries para cada thread
    series_por_thread = num_series // num_threads
    # Cria as instâncias das threads passando o ponto de partida relativo e quantas séries cada deve calcular 
    threads = [ThreadCalculaPi(i * series_por_thread, series_por_thread) for i in range(num_threads)]
    # Inicia os fluxos
    for thread in threads:
        thread.start()
    # Espera o término e guarda o resultado de cada fluxo 
    resultados = []
    for thread in threads:
        thread.join()
        resultados.append(thread.somatorio)
    # Retorna o somatório multiplicado por quatro para encontar a aproximação de pi
    return sum(resultados) * 4

# Valida os argumentos de entrada do programa
if sys.argv.__len__() != 3:
    print("> Argumentos inválidos! Por favor, entre com <número de séries> e <quantidade de threads>")
    exit()

num_series = int(sys.argv[1])
num_threads = int(sys.argv[2])

# Calcula a aproximação de pi e registra o tempo
inicio = time.time()
pi = calcular_pi_concorrente(num_series, num_threads)
pi = 3.14
tempo_decorrido = time.time() - inicio

# Resultados
print("> math.pi:    ", math.pi)
print("> Aproximação:", pi)
print("Séries:", num_series, "| Threads:", num_threads)
print("Cálculo executado em", tempo_decorrido, "segundos.")

# def calcula_medias_de_tempo(series: list, threads: list):
#     medias = []

#     for t in threads:
#         tempos = []
#         for s in series:
#             inicio = time.time()
#             pi = calcular_pi_concorrente(s, t)
#             tempo_decorrido = time.time() - inicio
#             tempos.append(tempo_decorrido)
#         medias.append(sum(tempos) / 3)

#     for i in range(threads.__len__()):
#         print(f"{threads[i]} Threads => Média: {medias[i]}")
