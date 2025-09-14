# K-Means Clustering – Sequential vs Parallel

This project implements the **K-Means clustering algorithm** in C++ and evaluates the performance difference between **sequential** and **parallelized** versions using OpenMP. The work explores concurrency trade-offs, scalability, and performance analysis on multi-core systems.

---

## Objectives
- Implement K-Means clustering from scratch in C++.  
- Parallelize clustering steps with **OpenMP** (static scheduling & reductions).  
- Benchmark sequential vs parallel implementations on datasets of different sizes.  
- Analyze performance trade-offs and threading overhead.  

---

## Approach

### 1. Dataset Handling
- Input data read from CSV files (points in multi-dimensional space).  
- Configurable parameters: number of clusters `k`, maximum iterations, convergence threshold.  

### 2. Sequential Implementation
- Assign each point to its nearest centroid.  
- Recalculate centroids as the mean of assigned points.  
- Repeat until convergence or iteration limit.  

### 3. Parallel Implementation (OpenMP)
- Parallelized **point-to-centroid assignment** using OpenMP `for` directives.  
- Parallelized **centroid updates** using OpenMP reductions for summations.  
- Used static scheduling to balance workload across threads.  

### 4. Performance Benchmarking
- Tested on datasets of varying sizes (small to large).  
- Evaluated on a **10-core system**.  
- Measured runtime for sequential and parallel runs.  

---

## Results
- **Small datasets**: Parallel version provided minimal or negative gains due to threading overhead.  
- **Medium to large datasets**: Parallel implementation achieved noticeable speedups, especially during centroid reassignment.  
- Showed importance of **dataset size vs number of cores** in parallel computing efficiency.  

---

## Tech Stack
- **Language:** C++  
- **Libraries:** OpenMP, Standard Template Library (STL)  
- **Tools:** g++, GNU Make, CSV I/O utilities  

---

## Run Locally
1. Clone this repo:  
   ```bash
   git clone https://github.com/<your-username>/Personal-Projects.git
   cd Personal-Projects/KMeans-Clustering
