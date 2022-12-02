# Raman Spectroscopy analyze

# Basic information
GitHub repository: https://github.com/haashiagh10/propsal

# Problem to Solve
Raman Spectroscopy , and High-Performance Liquid Chromatography(HPLC) are the way to get Raman spectroscpy data.To deal with the data , We want to detect the feature in the liqid by checking the Raman data.
However , the machine to get the spectral data and the model background are not quite clear.
After remove the noise and background in the data , we can implement some algorithm to analyze the data.

# System Architecture
The module should be compatible with NumPy arrays
![image](https://github.com/haashiagh10/propsal/blob/main/flowchart.png)

# API Description
I wish that one can install it by using pip.

However, an alternative means of installation would be git cloning the repo and installing using make.
- **Read/Save the data**
  - Read .txt format files
  - Read .csv format files
 
- **Data Preprocess**
  - Remove zeros and nans
  - Remove cosmic rays 
 
- **Data Process**
  - PCA
  - NMF
  - Peaks fitting(TBD)

- **algorithm**
  - k-means
  
# Engineering Infrastructure
- **Build System**
  - make
- **Testing Tool**
  - Google Test for C++
  - Pytest for Python
- **Version Control**
  - Git

# Schedule

| Week  | What-to-do |
| :---- |:---------- |
| 1  | Implement read module|
| 2  | Work on preprocessing part|
| 3  | Work on preprocessing part|
| 4  | Work on processing part|
| 5  | Work on processing part|
| 6  | Binding to python, unittesting, debugging, writing basic documentation|
| 7  | Work on algorithm part|
| 8  | Work on presentation|
