import itertools
import sys
from pathlib import Path

# --- Leitura do arquivo ---
def read_instance_file(filepath):
    lines = Path(filepath).read_text().splitlines()
    num_tasks, num_machines = map(int, lines[0].split())
    processing_times = [list(map(int, line.split())) for line in lines[1:num_tasks+1]]
    return processing_times

# --- Cálculo do makespan ---
def calculate_makespan(permutation, processing_times):
    num_tasks = len(permutation)
    num_machines = len(processing_times[0])
    completion_times = [[0]*num_machines for _ in range(num_tasks)]

    completion_times[0][0] = processing_times[permutation[0]][0]
    for j in range(1, num_tasks):
        completion_times[j][0] = completion_times[j-1][0] + processing_times[permutation[j]][0]
    for i in range(1, num_machines):
        completion_times[0][i] = completion_times[0][i-1] + processing_times[permutation[0]][i]
    for j in range(1, num_tasks):
        for i in range(1, num_machines):
            completion_times[j][i] = max(completion_times[j-1][i], completion_times[j][i-1]) + processing_times[permutation[j]][i]
    return completion_times[-1][-1]

# --- Cálculo de limite inferior ---
def calculate_lower_bound(processing_times):
    num_tasks = len(processing_times)
    num_machines = len(processing_times[0])
    machine_min_sums = sum(min(processing_times[i][j] for i in range(num_tasks)) for j in range(num_machines))
    task_sums = [sum(processing_times[i]) for i in range(num_tasks)]
    task_max_sum = max(task_sums)
    return max(machine_min_sums, task_max_sum)

# --- Verificação de makespan específico ---
def verify_makespan_target(processing_times, target, max_permutations=5_000_000):
    num_tasks = len(processing_times)
    count = 0
    best_found = sys.maxsize
    for perm in itertools.permutations(range(num_tasks)):
        count += 1
        makespan = calculate_makespan(perm, processing_times)
        best_found = min(best_found, makespan)
        if makespan == target:
            print(f"✅ Encontrado makespan exato {target} após {count:,} permutações.")
            return True
        if count >= max_permutations:
            break
    print(f"❌ Makespan {target} não encontrado nas primeiras {max_permutations:,} permutações.")
    print(f"Melhor makespan encontrado: {best_found}")
    return False

# --- Main ---
def main():
    filepath = "../inputs/in.txt"
    try:
        processing_times = read_instance_file(filepath)
    except Exception as e:
        print(f"Erro ao ler o arquivo: {e}")
        return

    lb = calculate_lower_bound(processing_times)
    print(f"\n🔍 Limite inferior teórico do makespan: {lb}")

    target = 909
    if lb > target:
        print(f"❌ Makespan {target} é IMPOSSÍVEL pois está abaixo do limite inferior ({lb}).")
        return

    possible = verify_makespan_target(processing_times, target, max_permutations=5_000_000)
    if possible:
        print(f"\n✅ O makespan {target} é POSSÍVEL para a instância.")
    else:
        print(f"\n❌ O makespan {target} NÃO FOI ENCONTRADO e provavelmente é impossível.")

    # Teste extra: maior valor impossível abaixo de 916
    print("\n🔁 Verificando maior makespan impossível abaixo de 916...")
    for test in range(target, 916):
        print(f"\nTestando makespan {test}...")
        if not verify_makespan_target(processing_times, test, max_permutations=3_000_000):
            print(f"🚫 {test} é (provavelmente) impossível.")
            break

if __name__ == "__main__":
    main()
