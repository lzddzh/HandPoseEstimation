#
# Generate a fake input data file.
# By Tony. Sep 20.
#
import random
featureNum = 10
labelNum = 5
exampleNum = 100

print 'id',
for i in range(0, featureNum):
    print ',value' + str(i),
for i in range(0, labelNum):
    print ',label' + str(i),
print '\n',

for i in range(0,exampleNum):
    print i,
    for j in range(0, featureNum):
        print "," + str(random.random()),
    for j in range(0, labelNum):
        print "," + str(random.random() * 150),
    print '\n',

