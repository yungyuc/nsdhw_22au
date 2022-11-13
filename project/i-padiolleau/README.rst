##Basic Information
https://github.com/i-padiolleau/optimization-for-composite-type-data-structure

##Problem to Solve
When it comes to optimize a code that manipulate at signicant amount of data, it is important to consider how the data is access to adapt the data structure used in the code. 
Compostite types array can be used to represent individuals with specific data (str name, str surname, int age, str nationality, str job, boolean married, int sex, ..)
There is differents solutions to store composite types array and the data structure impact the efficiency of the cache loading it. 
- AoS : Arrays of composed data structure
- SoA : Composite structure composed of different array types
- Single array with maching index and different type 
I will try with this project to manipule significant amount of data on a specific population and compute usefull data as (mean age, most given name, percentage of married person, ...)
based on the three method to store this population data and see which is the more efficient and for what usage.

##Prospective Users
The user will be able to manipulate the test that demonstrate the performance of each approch considering optimization

##System Architecture
The input gived by the user will be parameter for the tests, to change size of arrays, types and test in order to produces graphics results to show to the user.44

##API Description
The test.py file wile provided some pre-programmed tests that the user could choose to runs and all the functions that could be used to write a script (creation of the variable, allocation of memory, function that used data in order to test data locality)

##Engineering Infrastructure
**Build** will be perform by **Makefile** with "make" command
**Git will** serve as **version control** : Each week will be marked as V+1.0 and modified features marked as VX.+1
**Testing** will be perform using **python3.pytest**
**Documentation** (comments and **README** file to explain the few steps to use the code)

##Schedule
Week 1 (10/31):
Surrend proposal and worked the problem on paper, to some research online about data locality and tests demonstrating this principle

Week 2 (11/7):
Make the structure of the C++ code, Makefile

Week 3 (11/14):
Implement the different functions and struct allowing the user to create data structures on demande for the test

Week 4 (11/21):
Wrap in python pytest file and try the first test trought command "make test"

Week 5 (11/28):
Implement all the tests (test execution time)

Week 6 (12/5):
(void) This week is empty now, but projects never go as planned so this free week will allow some mistake along the way

Week 7 (12/12):
Prepare the report and worked again on code comments and documentation

Week 8 (12/19):
Surrend work and prepare the presentation