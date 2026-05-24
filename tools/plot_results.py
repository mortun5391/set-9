from pathlib import Path
import sys

import matplotlib.pyplot as plt
import pandas as pd


def nice_name(name: str) -> str:
    return {
        "QUICKSORT": "Quicksort",
        "MERGESORT": "Mergesort",
        "STRING_QUICKSORT": "String quicksort",
        "STRING_MERGESORT_LCP": "String mergesort (LCP)",
        "MSD_RADIX_SORT": "MSD radix",
        "MSD_RADIX_SORT_WITH_STRING_QS": "MSD + string QS",
    }[name]


def dataset_name(name: str) -> str:
    return {
        "random": "Случайный массив",
        "reverse": "Обратно отсортированный массив",
        "nearly_sorted": "Почти отсортированный массив",
        "common_prefix": "Строки с общим префиксом",
    }[name]


def draw_lines(frame: pd.DataFrame, column: str, ylabel: str, title: str, out_path: Path) -> None:
    fig, ax = plt.subplots(figsize=(10, 6))
    for algorithm in frame["algorithm"].unique():
        part = frame[frame["algorithm"] == algorithm].sort_values("n")
        ax.plot(part["n"], part[column], marker="o", markersize=3, linewidth=1.5,
                label=nice_name(algorithm))
    ax.set_xlabel("Количество строк n")
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=8)
    fig.tight_layout()
    fig.savefig(out_path, dpi=180)
    plt.close(fig)


def draw_final_bar(frame: pd.DataFrame, column: str, ylabel: str, title: str, out_path: Path) -> None:
    end = frame[frame["n"] == 3000].copy()
    end["label"] = end["algorithm"].map(nice_name)
    end = end.sort_values(column)
    fig, ax = plt.subplots(figsize=(11, 6))
    ax.bar(end["label"], end[column])
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.tick_params(axis="x", rotation=28)
    ax.grid(True, axis="y", alpha=0.3)
    fig.tight_layout()
    fig.savefig(out_path, dpi=180)
    plt.close(fig)


def main() -> None:
    csv_path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("data/results.csv")
    output = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("figures")
    output.mkdir(parents=True, exist_ok=True)

    df = pd.read_csv(csv_path)
    for dataset, frame in df.groupby("dataset"):
        draw_lines(frame, "avg_time_us", "Среднее время, мкс",
                   f"Время работы: {dataset_name(dataset)}", output / f"time_{dataset}.png")
        draw_lines(frame, "char_comparisons", "Посимвольные сравнения",
                   f"Количество сравнений: {dataset_name(dataset)}", output / f"comparisons_{dataset}.png")
        draw_lines(frame, "char_inspections", "Обращения к символам",
                   f"Обращения к символам: {dataset_name(dataset)}", output / f"inspections_{dataset}.png")
        draw_final_bar(frame, "avg_time_us", "Среднее время, мкс",
                       f"Сравнение на n = 3000: {dataset_name(dataset)}", output / f"time_3000_{dataset}.png")

    at_3000 = df[df["n"] == 3000].copy()
    at_3000.to_csv(csv_path.parent / "summary_at_3000.csv", index=False)


if __name__ == "__main__":
    main()
