[Link cài c++ trên vscode](https://code.visualstudio.com/docs/cpp/config-linux)
### Đoạn code config trong tasks.json
- Thay 
```
$file,
```
trong phần args của g++ bằng
```bash
"${workspaceFolder}/src/c++/src/*.cpp",
"-I${workspaceFolder}/src/c++/include",
```
[Link lấy file .hpp của src code đọc .csv file](https://github.com/vincentlaucsb/csv-parser)