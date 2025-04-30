# Big Data Search Project

## How To?

#### Run Python file to create dataset:
1. `python3 -m venv venv`
2. `source ./venv/bin/activate`
3. `pip install -r requirements.txt`

#### Compile and run single thread file:
1. `g++ -o single_thread single_thread.cpp`
2. `./single_thread <TARGET_WORD>`
   
   for examaple: `./single_thread brandy`


#### Compile and run multi thread file
1. `g++ -fopenmp -o multi_thread multi_thread.cpp`
2. `./multi_thread <TARGET_WORD> <NUM_THREADS>`

    for example: `./multi_thread brandy 10`