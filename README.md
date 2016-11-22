## Random Forest for Hand Pose Estimation from Single Depth Image

### Introduction 
This program has a Random Forest containing number of Trees who can be trained 
by training data and make prediction on test data. The Random Forest will then 
collect the results of all the trees and make the final prediction.

This program is a implementation of paper: 
     Chi Xu, Ashwin Nanjappa, Xiaowei Zhang, Li Cheng. [Estimate Hand Poses Efficientlyfrom Single Depth Images](http://web.bii.a-star.edu.sg/~xuchi/dhand.htm). In In International Journal of Computer Vision (IJCV), 2015. 

We have implemented the 'baseline-M' version and the 'dynamical weight at leaf nodes' version.

The baseline-M is a normal random forest that can be change and adapt for other general problem sovling.

### Platform
* Ubuntu14.04 or other common Linux version.
* Mac and Windows is possible, but may need some configuration by yourself.
* Note you also need to have the `Prerequests` on your platform.

### Directory

```console
.    
CMakeList.txt  *For Cmake
Makefile    *For compile time use, sepcifing the compile rules.
README.md   *This file.
common.h
...         *The source files and the makefile.
main.cpp
InputData/  *Stores input data `.csv`.
OutputData/  *Stores output data from program `.csv`.
ExampleInput/  *Stores some small examples of input data file.
```

### Prerequest
* g++  * The C++ compiler, or you can use anything equvalent.
* Armadillo-7.400.2   * A linear algebra C++ open library.

**How to install the prerequests?**
First download the `armdillo-7.400.2.zip` from [this link](http://pan.baidu.com/s/1o7Um4Sa), then uppack it. 
Alternatively, you could also refers to [official website of Armdillo](http://arma.sourceforge.net/)
install openCV at [openCV official user guide](http://docs.opencv.org/2.4.13/doc/user_guide/user_guide.html)

```console 
$ sudo apt-get update
$ sudo apt-get install g++
$ sudo apt-get install cmake
$ sudo apt-get install libblas-dev liblapack-dev
$ cd /YourDirectory/armdillo-7.400.2
$ cmake .
$ make
$ sudo make install
```

Finish. Now you have the compiler and the linear algebra library.
Notice above command lines are not tested, so not ensure work.
For detail information about `Armdillo`, please refer to its RAEDME.txt, in line 125.

We need `Armadillo` to do some matrix calculation such as COV() and DET(),
Armadillo can also be installed on Windows if you like.

### Compile
```console
$ cd /YourDirectory/HandPoseEstimation/
$ cmake .
$ make
```
After the make, you should see some files like `.o`, `.gch` and excutable `run.out`.
If failed during in make, please check you if your prerequests are successfully installed.

### Run
```console
$ ./HandPoseEstimation
$ d
```
for debug mode. (without test on test data)

or

```console
$ ./HandPoseEstimation
$ r 
```
for real time mode. (with output of test data result)


When you are running the program, it will ask you which mode to choose,
for now, just choose `debug mode`.

**Notice:** If you experience 'segmentation falt‘ during your running, please first
 check the 'feaNum','labelNum' and 'trainExampleNum' at 'common.h'.

### Program Input and Output 
This program does not input from the raw files like .shand, .skdepth or skdepth.cropping.
Instead this program takes a single '.csv' file as its input, which stores all the training examples.

The training Data in .csv format can be downloaded at: [this link](https://pan.baidu.com/s/1dFNui1j)
where `value0, value1, ...` is the pixel value of our depth image, so there should be `90*60` values,and the `label0, label1, ...` is the hand joints postion value, so there should be `20*3` labels.

In both `debug` and `real-time` mode of this program at running time, the program will output two files
under folder `HandPoseEstimation/OutputData`:


* resultRaw.csv
* result.csv


And with postfix of `Debug` or `Real`, imples which mode has been chosen.
In debug mode where we have only training data input, we can also output prediction results because
we split the training data into two part in the debug mode, 70% for training and 30% for testing.
    
### Parameter Settings
There are some parameters like number of threads, number of features, number of labels are in the source file `common.h`, specifing by `#define`.  Just edit them and save if you need.

**Please set the NUMTHREAD equals to the cores of your computer.**

Parameters that can be set in 'main.cpp':
1. Threasholds for the trees at training time. See `a`, `b`, `c` in the code.
2. Number of trees in the forest. See `treeNum` in the code.
3. The ratio of training data and test data at debug mode. See code `trainData.split()`.
4. Input and output file names can be found in main.cpp.

Other parameters:
If you want to change the percentage of random data assigned to each tree, refer to 
`RandomForest.cpp`->`RandomForest::run()`->`generateRandomData(0.6)`.

### Known Bugs
This is just a prototype, below are a list that the way I used is not same with the paper.

1. I randomly select 60% examples for a tree rather than randomly select them at each spliting node time.
2. In the RandomForest, the vote is done by just calculting the average value of all the results.
3. At prediction time, just goes down along the tree by seeing if the corresponing feature value of 
    the example is bigger then the split value at the node. When reach leaf, return the average value
    of labels in the leaf. 
4. For randomly select features at a node, I randomly select sqrt(features.size()).

### About CS5340
This Hand Pose Estimation is a course project of NUS School of Computing CS5340 
[Uncertainty modelling in AI](http://web.bii.a-star.edu.sg/~chengli/courses/CS5340-F16/) Fall 2016.

**Useful books:**
* **Probabilistic Graphical Models : Principles and Techniques**  Daphne Koller and Nir Friedman
* **An Introduction to Probabilistic Graphical Models** M. Jordan
* [Deep Learning Online Book](http://www.deeplearningbook.org/)

* [Graphical models, exponential families, and variational inference](http://www.eecs.berkeley.edu/~wainwrig/Papers/WaiJor08_FTML.pdf)
* [Probabilistic Reasoning in Intelligent Systems: Networks of Plausible Inference](http://www.amazon.com/Probabilistic-Reasoning-Intelligent-Systems-Plausible/dp/1558604790)

### Team member
* Liu Zhendong - Tony
* Yang Xuan - Yancy
* Huang Mengying - Molly

### Reference
1. Chi Xu, Ashwin Nanjappa, Xiaowei Zhang, Li Cheng. "Estimate Hand Poses Efficiently
         from Single Depth Images". In In International Journal of Computer Vision (IJCV), 2015. 
2. Conrad Sanderson and Ryan Curtin. Armadillo: a template-based C++ library for linear algebra.
        Journal of Open Source Software, Vol. 1, pp. 26, 2016.
