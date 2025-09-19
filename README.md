# Maximum Common Induced Subgraph (MCIS) Algorithm Testing Framework

<div align="center">

_A Senior Thesis Project in Electrical Engineering and Computer Science_\
_Yale University • CPSC 4900_

[![C++](https://img.shields.io/badge/C++-20-blue.svg?style=flat&logo=cplusplus)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.22+-green.svg?style=flat&logo=cmake)](https://cmake.org/)
[![GTest](https://img.shields.io/badge/Testing-Google%20Test-red.svg?style=flat)](https://github.com/google/googletest)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat)](LICENSE)

</div>

---

## Overview

This library provides a comprehensive C++ testing framework for evaluating Maximum Common Induced
Subgraph (MCIS) algorithms specifically designed for Brain-Computer Interface (BCI) accelerator
workloads.

### Why This Matters

Brain-Computer Interfaces represent the cutting edge of medical technology, offering hope for
restoring sensory and motor functions to individuals with severe neurological disorders. However,
the practical implementation of fully implanted BCIs is severely constrained by ultra-low power
requirements, since implanted devices must minimize heat dissipation to prevent damage to surrounding
neural tissue.

Many BCI workloads (neural signal processing, data interpretation) can be modeled as Computational
Directed Acyclic Graphs (CDAGs). Rather than designing separate hardware accelerators for each
algorithm, this research aims to validate the most efficient algorithms to identify the largest computational structures shared among different BCI
algorithms, enabling the design of a single, minimal, canonical hardware accelerator that
maximizes hardware reuse while minimizing power consumption.

### The Challenge

The Maximum Common Induced Subgraph (MCIS) problem is NP-hard, making brute-force approaches computationally infeasible for complex CDAGs representing realistic BCI workloads. However, CDAGs derived from signal processing algorithms often possess specific structural properties that may be exploitable for developing efficient algorithms.

---

## Architecture

This framework provides:

- **Modular Graph Representation**: Robust data structures for representing and manipulating CDAGs
- **Algorithm Interface**: Clean interfaces for integrating various MCIS-finding algorithms
- **Comprehensive Benchmarking**: Performance measurement tools for runtime, memory usage, and subgraph quality
- **Testing Infrastructure**: Google Test-based validation framework
- **Dataset Generation**: Tools for creating BCI-relevant CDAG datasets (DWT, FFT, MVM algorithms)

---

## Quick Start

### Prerequisites

Ensure you have the following dependencies installed:

- **C++ Compiler**: Supporting C++20 standard
  - GCC 10+ / Clang 12+ / MSVC 2019+
- **CMake**: Version 3.22.1 or higher
- **(optional) Graphviz (dot)**: For graph visualization

```bash
  # macOS
  brew install cmake graphviz

  # Ubuntu/Debian
  sudo apt install cmake graphviz

  # Windows (using Chocolatey)
  choco install cmake graphviz
```

### Building the Project

```bash
# Clone the repository
git clone https://github.com/sebaraj/MCIS-BCI.git
cd MCIS-BCI

# Create build directory
mkdir build && cd build

# Install dependencies with Conan
conan profile detect
conan install .. --build=missing -of . -v -g CMakeToolchain -g CMakeDeps

# Configure with CMake
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build .

# Run tests
ctest --verbose
```

---
