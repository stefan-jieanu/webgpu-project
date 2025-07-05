# Build project files using wgpu or dawn
```
cmake -B build -DWEBGPU_BACKEND=WGPU
```
or
```
cmake -B build -DWEBGPU_BACKEND=DAWN
```

# Build project
cmake --build build

# Run App
./build/App