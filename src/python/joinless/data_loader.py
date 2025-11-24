import pandas as pd
import os
from .structures import SpatialDataset, SpatialInstance

# 8. HELPER FUNCTION: Load Dataset from CSV
def load_spatial_dataset(csv_path: str) -> SpatialDataset:
    """
    Load the LasVegas dataset from CSV into SpatialDataset structure.
    """
    df = pd.read_csv(csv_path)
    dataset = SpatialDataset()

    for _, row in df.iterrows():
        instance = SpatialInstance(
            feature=str(row["Feature"]),
            instance_id=int(row["Instance"]),
            x=float(row["LocX"]),
            y=float(row["LocY"]),
            checkin=int(row["Checkin"]),
        )
        dataset.add_instance(instance)

    return dataset


def load_or_build_dataset(
    csv_path: str,
    cache_path: str,
    distance_threshold: float,
    force_rebuild: bool = False,
) -> SpatialDataset:
    """
    Load dataset from cache if available, otherwise build from CSV.

    Args:
        csv_path: Path to CSV file
        cache_path: Path to pickle cache file
        distance_threshold: Distance threshold for neighbor relations
        force_rebuild: If True, rebuild even if cache exists

    Returns:
        SpatialDataset with precomputed star neighborhoods
    """
    if not force_rebuild and os.path.exists(cache_path):
        print("Loading from cache...")
        dataset = SpatialDataset.load_from_file(cache_path)

        # Check if threshold matches
        if dataset.distance_threshold == distance_threshold:
            print(
                f"Loaded {len(dataset.instances)} instances, {len(dataset.star_neighborhoods)} star neighborhoods"
            )
            return dataset
        else:
            print(
                f"Threshold mismatch. Rebuilding with threshold={distance_threshold}..."
            )

    print("Building dataset from CSV...")
    dataset = load_spatial_dataset(csv_path)
    dataset.build_neighbor_relations(threshold=distance_threshold)
    dataset.build_star_neighborhoods()
    dataset.save_to_file(cache_path)
    print("Dataset cached successfully.")
    return dataset
