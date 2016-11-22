from os import listdir
import os
import string

import numpy as np
from pybrain.datasets            import ClassificationDataSet
from pybrain.utilities           import percentError
from pybrain.tools.shortcuts     import buildNetwork
from pybrain.supervised.trainers import BackpropTrainer
from pybrain.structure.modules   import SoftmaxLayer
from pybrain.structure import TanhLayer
from random import shuffle


from pylab import ion, ioff, figure, draw, contourf, clf, show, hold, plot
from scipy import diag, arange, meshgrid, where
from numpy.random import multivariate_normal


X = []
Y = []
# load train data
trainDataDir = '/home/jayd/Desktop/NUS/UncertaintyModel/project/Training_Dataset/'
for i in range(16, 31):
    for j in range(1, 11):
        file_name = (
            trainDataDir + "subject-" + str(i) + "/" + "subject-" + str(i) + "_" + str(j) + ".skdepth.cropping")
        if (os.path.isfile(file_name)):
            name = str(j)
            lines = open(file_name).readlines()
            image = []
            for line in lines:
                temps = line.strip("\n").split(" ")[:-1]
                image.extend(temps)
            X.append([float(p) for p in image])
            Y.append(name)

    for j in string.uppercase:
        file_name = (
            trainDataDir + "subject-" + str(i) + "/" + "subject-" + str(i) + "_" + str(j) + ".skdepth.cropping")
        if (os.path.isfile(file_name)):
            name = str(j)
            lines = open(file_name).readlines()
            image = []
            for line in lines:
                temps = line.strip("\n\r").split(" ")[:-1]
                image.extend(temps)
            X.append([float(p) for p in image])
            Y.append(name)
# load test data
test_X = []
test_Y = []
testDataDir = '/home/jayd/Desktop/NUS/UncertaintyModel/project/Test_Dataset/'
for i in range(1, 16):
    for j in range(1, 11):
        file_name = (testDataDir + "subject-" + str(i) + "/" + "subject-" + str(i) + "_" + str(j) + ".skdepth.cropping")
        if (os.path.isfile(file_name)):
            name = str(j)
            lines = open(file_name).readlines()
            image = []
            for line in lines:
                temps = line.strip("\n\r").split(" ")[:-1]
                image.extend(temps)
            test_X.append([float(p) for p in image])
            test_Y.append(name)

    for j in string.uppercase:
        file_name = (testDataDir + "subject-" + str(i) + "/" + "subject-" + str(i) + "_" + str(j) + ".skdepth.cropping")
        if (os.path.isfile(file_name)):
            name = str(j)
            lines = open(file_name).readlines()
            image = []
            for line in lines:
                temps = line.strip("\n\r").split(" ")[:-1]
                image.extend(temps)
            test_X.append([float(p) for p in image])
            test_Y.append(name)

#nural network
print "training data loaded. ", len(X), len(Y)
print "test data loaded. ", len(test_X), len(test_Y)

# change 29 char labels to continues 0....28 int.
labels = list(set(Y))
Y_n = [labels.index(e) for e in Y]
test_Y_n = [labels.index(e) for e in test_Y]

# normalize the feature to [-1, 1]
X_mean = np.mean(X, axis=0)
norm_X = [line - X_mean for line in np.array(X)]
norm_test_X = [line - X_mean for line in np.array(test_X)]

examples = []
for i in range(0, len(norm_X)):
    examples.append((norm_X[i], Y_n[i]))
shuffle(examples)


alldata = ClassificationDataSet(5400, 1, nb_classes=29)
for i in range(0, len(examples)): 
    alldata.addSample(examples[i][0], [examples[i][1]])

tstdata, trndata = alldata.splitWithProportion( 0.25 )
trndata._convertToOneOfMany()
tstdata._convertToOneOfMany()

print "Number of training patterns: ", len(trndata)
print "Input and output dimensions: ", trndata.indim, trndata.outdim
print "First sample (input, target, class):"
print trndata['input'][0], trndata['target'][0], trndata['class'][0]
fnn = buildNetwork( trndata.indim, 10, trndata.outdim, hiddenclass=TanhLayer, outclass=SoftmaxLayer )

trainer = BackpropTrainer( fnn, dataset=trndata, momentum=0.05, verbose=True, weightdecay=0.01)

predictdata = ClassificationDataSet(5400,1, nb_classes=29)
for i in range(0, len(norm_test_X)): 
    predictdata.addSample(norm_test_X[i], [0])

predictdata._convertToOneOfMany()  # this is still needed to make the fnn feel comfy

for i in range(2000):
    trainer.trainEpochs( 1 )
    trnresult = percentError( trainer.testOnClassData(),
                              trndata['class'] )
    tstresult = percentError( trainer.testOnClassData(
           dataset=tstdata ), tstdata['class'] )

    print "epoch: %4d" % trainer.totalepochs, \
          "  train error: %5.2f%%" % trnresult, \
          "  test error: %5.2f%%" % tstresult

    out = fnn.activateOnDataset(predictdata)
    out = out.argmax(axis=1)  # the highest output activation gives the class
    result = [labels[e] for e in out]
print result 
with open('results.txt', 'w') as outfile:
    outfile.writelines(["%s\n" % item  for item in out])
