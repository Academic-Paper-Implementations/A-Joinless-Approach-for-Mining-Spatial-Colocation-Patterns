# A Joinless Approach for Mining Spatial Colocation Patterns

## 📋 Tổng quan
Dự án này cài đặt thuật toán từ bài báo **"A Joinless Approach for Mining Spatial Colocation Patterns"**.
Mục tiêu là khai phá các mẫu co-location trong dữ liệu không gian mà không cần thực hiện phép nối (join) đắt đỏ, thay vào đó sử dụng cấu trúc **Star Neighborhood**.

## 📄 Paper
* **Tên bài báo:** A Joinless Approach for Mining Spatial Colocation Patterns
* **Link:** [PDF](./joinless_algorithm.pdf) hoặc [Online](https://sci-hub.hlgczx.com/10.1109/tkde.2006.150)

## 📂 Cấu trúc Dự án
* `build_dataset.py`: Chứa các cấu trúc dữ liệu chính (SpatialInstance, StarNeighborhood, Clique, ...) và logic xây dựng dataset.
* `test.ipynb`: Notebook kiểm thử và minh họa cách sử dụng.
* `README_BUILD_DATASET.md`: Tài liệu chi tiết về module `build_dataset.py`.
* `data/`: Thư mục chứa dữ liệu (ví dụ: LasVegas dataset).

## 🚀 Cách sử dụng

### 1. Cài đặt môi trường
Đảm bảo bạn đã cài đặt các thư viện cần thiết (xem `requirements.txt` nếu có, hoặc cài `pandas`, `numpy`).

### 2. Xây dựng Dataset
Sử dụng `build_dataset.py` để chuyển đổi dữ liệu CSV thành cấu trúc Star Neighborhood.

```python
from build_dataset import load_or_build_dataset

csv_path = "data/LasVegas_x_y_alphabet_version_03_2.csv"
cache_path = "LasVegas_cache.pkl"
distance_threshold = 160.0

dataset = load_or_build_dataset(csv_path, cache_path, distance_threshold)
print(f"Loaded {len(dataset.instances)} instances.")
```

### 3. Chạy thuật toán (Đang cập nhật)
Hiện tại code tập trung vào việc xây dựng cấu trúc dữ liệu nền tảng (Star Neighborhoods). Phần thuật toán khai phá (mining) sẽ được cập nhật tiếp theo.

## 🤝 Đóng góp
Tham khảo `CONTRIBUTING.md` để biết cách đóng góp vào dự án.