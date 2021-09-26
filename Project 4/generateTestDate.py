import pandas as pd
import random

df = pd.read_csv("cities.csv")
city_names = df["City"].tolist()

name = input("Enter Name: ")
nameList = []
outputList = []
while name != "q":
    outputList.append([name, random.randrange(10000,99999), random.randrange(1,100), random.choice(city_names[:20])])
    name = input("Enter Name: ")

f = open(r"output.txt","w+")
for i in range(len(outputList)): 
    f.write("string i{} = \"'{}' '{}' '{}' '{}' \";".format(i, outputList[i][0], outputList[i][1], outputList[i][2], outputList[i][3]))
    f.write("\n")
    f.write("t.insert(i{});".format(i))
    f.write("\n")

f.close()