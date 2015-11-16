import csv
import math
import operator
import random
import sys

def calculateCentriods(dataset, numClasses):
    '''
    RETURN A LIST OF ALL CENTRIODS

    PARAM dataset:      The dataset to calculate the centriods for.  Note that it must 
        be organized as [classID, attribute1Value, attribute2Value, ..., attributeNValue]
        with each row as a sample of data
    PARAM numClasses:   The number of classes in the dataset
    RETURN:             A list containing the centriods for each class, indexed as
        centriods[classID]
    '''
    centroids = []
    for classNumber in range(numClasses):
            # Reset list each time
            currentCentroid = []

            for row in dataset:
                if row[0] == classNumber:
                    currentCentroid.append(row[1:])
            
            # Get the columns with zip, sum, average, round to 1 decimal point
            currentCentroid = [(sum(x)/len(currentCentroid)) for x in zip(*currentCentroid)]

            # Store class number in first entry of list
            currentCentroid.insert(0, classNumber)

            centroids.append(currentCentroid)
    return centroids

def euclideanDistance(p, q, dim):
    '''
    RETURN THE EUCLIDAN DISTANCE BETWEEN TWO N-TUPLES

    PARAM p:   A tuple contaning a point, formatted (x-coord, y-coord, z-coord, etc.)
    PARAM q:   A tuple formatted (x-coord, y-coord, z-coord, etc.)
    PARAM dim: The dimension of both tuples
    RETURN:    The euclidian distance (vector) as a floating point value
    '''
    # Minimum distance is 0
    d = 0

    for x in range(1,dim):
        d += (p[x] - q[x]) ** 2 # Sum the Squares
    return math.sqrt(d) # Square Root of the sum of the squares

def getClosestClass(centroids, sample):
    '''
    RETURN THE CLOSEST CENTRIOD TO THE GIVEN SAMPLE

    PARAM centroids:    A list of centroids
    PARAM numClasses:   The sample to classify
    RETURN:             The centriod which sample is closest to.

    NOTE: We return the whole row, not just the classID
    '''
    distancesList = []

    # Find the distance to each centriod, then sort based on it
    for classID in range(len(centroids)):
        currentDistance = euclideanDistance(sample, centroids[classID], len(sample))
        distancesList.append((centroids[classID], currentDistance))
    distancesList.sort(key=operator.itemgetter(1))

    # Closest centriod's class
    return distancesList[0][0]

def getMinMax(dataset):
    '''
    RETURN THE MINIMUM AND MAXIMUM OF EACH ATTRIBUTE

    PARAM dataset:      The dataset to parse
    RETURN:             A list of attribute minimums and maximums

    NOTE: Return list format is [(attribute1min, attribute1max), (attribute2min, attribute2max), ...]
    '''
    maxList = []
    minList = []

    # Grab each column
    for column in zip(*dataset):
        maxList.append(column)
        minList.append(column)

    # Put maximums and minimums into list
    for i in range(len(maxList)):
        maxList[i] = max(maxList[i])
    for i in range(len(minList)):
        minList[i] = min(minList[i])

    # Assign tuples and return
    attributeMinMax = []
    for i in range(len(maxList)):
        pair = (minList[i], maxList[i])
        attributeMinMax.append(pair)
    return attributeMinMax

def normalizeFunciton(value, minimum, maximum):
    '''
    NORMALZE FUNCTION

    PARAM value:        The value to be noramlized
    PARAM minimum:      Minimum value to use in the funciton
    PARAM maximum:      Maximum value to use in the funciton
    RETURN:             (value - minimum) / (maximum - minimum)
    '''
    return (value - minimum) / (maximum - minimum)

def outputToConsole(classList, dataset, classMembershipCount, numberCorrect):
    print(classList[0])

    classNames = []
    numClasses = 0
    for i in classList[1:]:
        accuracy = round((100 * numberCorrect[int(i[0])]) / classMembershipCount[int(i[0])], 1)
        print("Class {0} ({1}): {2} samples, {3}% accuracy".format(
            i[0], 
            i[(i.index('=')+1):], 
            classMembershipCount[numClasses], 
            accuracy), end='')
        print()
        numClasses += 1

    totalAccuracy = round((100 * sum(numberCorrect)) / len(dataset), 1)
    print("overall: {0} samples, {1}% accuracy".format(len(dataset), totalAccuracy))

def outputToFile(filename, classList, dataset, classMembershipCount, numberCorrect, guesses):
    nameOfFile = filename.split(".")[0]
    nameOfFile += ".cv"

    cvFile = open(nameOfFile, "w+")

    cvFile.write(classList[0] + "\n")

    classNames = []
    numClasses = 0
    for i in classList[1:]:
        accuracy = round((100 * numberCorrect[int(i[0])]) / classMembershipCount[int(i[0])], 1)
        outputString = "Class {0} ({1}): {2} samples, {3}% accuracy".format(
            i[0], 
            i[(i.index('=')+1):], 
            classMembershipCount[numClasses], 
            accuracy)
        cvFile.write(outputString)
        cvFile.write("\n")
        numClasses += 1

    totalAccuracy = round((100 * sum(numberCorrect)) / len(dataset), 1)
    cvFile.write("overall: {0} samples, {1}% accuracy\n".format(len(dataset), totalAccuracy))

    cvFile.write("\nSample, Class, Prediction\n")
    for i in range(len(dataset)):
        outputString = "{0}, {1}, {2}".format(i+1, dataset[i][0], guesses[i])
        if dataset[i][0] != guesses[i]:
            outputString += "*"
        cvFile.write(outputString + "\n") 
        outputString = ""

    cvFile.close()

def SampleClassifier(dataset, filename):
    '''
    CLASSIFY EACH ROW IN THE DATASET BY MINIMUM DISTANCE TO ANY CLASS' CENTRIOD

    PARAM dataset:      The dataset to classify
    RETURN:             none
    '''

    # Organize our class list and attribute names into their own lists
    classList = dataset.pop(0)
    attributeNames = dataset.pop(0)

    # We remove sample number from the first column
    for row in dataset:
        row.pop(0)

    # At the point, the dataset only contains values in the following format
    # [ classID, attribute1Value, attribute2Value, ..., attributeNValue ]

    # We go ahead and count the total number of members in each class right now
    classMembershipCount = countClassMembershpip(dataset)
    for row in dataset:
        classMembershipCount[row[0]] += 1
    numberCorrect = classMembershipCount[:]
    guesses = []

    # Now we can start looking at one sample at a time
    i = 0 # Index for current sample
    while i < len(dataset):
        unknownSample = dataset.pop(i) # Current test sample (row)

        # This converts the dataset into a normalized array using the normalizer function x_normal = (x - min)/(max - min)
        minMaxSet = getMinMax(dataset)
        normalizedData = []
        for row in dataset:
            temp = []
            for column in range(1,len(row)):
                temp.append(normalizeFunciton(row[column], minMaxSet[column][0], minMaxSet[column][1]))
            temp.insert(0, row[0])
            normalizedData.append(temp)

        normalizedSample = [unknownSample[0]] # The first column of the current test row doesn't change (It's the class number, you can't normalize that shit)
        for column in range(1,len(unknownSample)): # This loop normalizes the rest of the test sample
            normalizedSample.append(normalizeFunciton(unknownSample[column], minMaxSet[column][0], minMaxSet[column][1]))

        # Compute the centriod for each class, save as a list of centroids
        centriods = calculateCentriods(normalizedData, len(classMembershipCount))

        # This function finds the distance to each centriod, sorts the list, and returns it.  We save it here as closestNeighbor[]
        closestNeighbor = getClosestClass(centriods, normalizedSample)
        guessClass = closestNeighbor[0] # Index 0 is the closest
        guesses.append(guessClass) # guesses is an empty list now
        if unknownSample[0] != guessClass:
            numberCorrect[unknownSample[0]] -= 1

        # Insert it back into the list so we can classify the next sample
        dataset.insert(i, unknownSample)
        i+=1

    # You are good to output at this point.
    # Guesses contains the guess class for each sample
    # numberCorrect contains the number of correct guesses for each class
    # classMembershipCount contains the number of members in each class (index = class)

def countClassMembershpip(dataset):
    '''
    COUNT THE NUMBER OF MEMBERS IN EACH DATA CLASS

    PARAM dataset:      The dataset to parse
    RETURN:             A list with indexed as follows
        numberOfMembersInClass = classMembers[classID]
    '''
    classMembers = []
    for row in dataset:
        if row[0] >= len(classMembers):
            classMembers.append(0)

    return classMembers

def read_csv(filename):
    '''
        RETURN A LIST OF ROWS WHERE EACH ROW IS A ROW IN THE CSV FILE

        PARAM filename: A .csv file to parse
        RETURN:         A list of rows where each row is a row in the csv file

        NOTE: All numerical values in the returned dataset are floating point values
        NOTE: Ignores strings
    '''
    with open(filename) as f:
        obj = csv.reader(f)
        dataset = list(obj)

    for i in range(0, len(dataset)):
        for j in range(0, len(dataset[i])):
            try:
                dataset[i][j] = float(dataset[i][j])
            except:
                pass

    # Sample Number and Class Number are integers
    for row in dataset[2:]:
        row[0] = int(row[0])
        row[1] = int(row[1])

    return dataset

if __name__ == "__main__":
    filename = sys.argv[1]
    dataset = read_csv(sys.argv[1])

    SampleClassifier(dataset, sys.argv[1])
