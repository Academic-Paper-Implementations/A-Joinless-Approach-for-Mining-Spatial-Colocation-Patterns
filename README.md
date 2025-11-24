# A Joinless Approach for Mining Spatial Colocation Patterns

## 📋 Overview
This project implements the **Joinless Co-location Pattern Mining** algorithm.
It is structured to support multiple implementations (Python, C, Java) and keeps documentation and data separate.

## 📂 Project Structure

```
root/
├── data/                           # Shared datasets
├── docs/                           # Documentation & Papers
│   ├── joinless_algorithm.pdf      # Original Paper
│   └── README_BUILD_DATASET.md     # Data structure docs
├── notebooks/                      # Jupyter Notebooks
│   └── analysis.ipynb              # Analysis & Visualization
├── src/                            # Source Code
│   ├── python/                     # Python Implementation
│   │   ├── joinless/               # Main Package
│   │   │   ├── structures.py       # Data structures
│   │   │   ├── data_loader.py      # Loading logic
│   │   │   └── mining.py           # Algorithm implementation
│   │   └── main.py                 # Entry point
│   ├── c/                          # C Implementation (Future)
│   └── java/                       # Java Implementation (Future)
└── requirements.txt
```

## 🚀 How to Run (Python)

### 1. Setup
Ensure you have the required packages:
```bash
pip install -r requirements.txt
```

### 2. Run the Main Script
You can run the python implementation directly:
```bash
python src/python/main.py
```

### 3. Use Notebooks
Open `notebooks/analysis.ipynb` in Jupyter.
*Note: You may need to adjust the path imports in the notebook to point to `../src/python`.*

## 🤝 Contributing
* **Python**: Work in `src/python/joinless`.
* **C/Java**: Add your implementations in `src/c` or `src/java`.