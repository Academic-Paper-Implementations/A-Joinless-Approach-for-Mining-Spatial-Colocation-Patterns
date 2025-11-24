import sys
import os

# Add the current directory to sys.path to allow imports
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from joinless import load_or_build_dataset, mine_colocation_patterns

def main():
    # Define paths relative to this script
    base_dir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    data_dir = os.path.join(base_dir, "data")
    
    csv_path = os.path.join(data_dir, "LasVegas_x_y_alphabet_version_03_2.csv")
    cache_path = os.path.join(data_dir, "LasVegas_cache.pkl")
    distance_threshold = 160.0

    print(f"Data directory: {data_dir}")
    
    # Load or build dataset
    try:
        dataset = load_or_build_dataset(csv_path, cache_path, distance_threshold)
        print(f"Successfully loaded dataset with {len(dataset.instances)} instances.")
        
        # Run mining algorithm (Placeholder)
        print("Starting mining process...")
        patterns = mine_colocation_patterns(dataset, min_prevalence=0.5)
        print("Mining complete.")
        
    except FileNotFoundError:
        print(f"Error: Data file not found at {csv_path}")
        print("Please ensure the data directory contains the LasVegas dataset.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
