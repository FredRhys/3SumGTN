# 3SumGTN

A reimplementation of [my master's thesis code](https://github.com/FredRhys/4th-year-diss-code), made without deadline stress.

This program seeks integer solutions to the equation:

![equation](https://latex.codecogs.com/svg.image?\binom{x}{3}&plus;\binom{y}{3}&plus;\binom{z}{3}=k)

for given $k$. From now on the binomial symbol will be written ($x$)C3

For a full explanation, see [my thesis](https://drive.google.com/file/d/1LHM9BBNr0isWwWY-byo3zMEb0KBxi436/view?usp=sharing). I intend to update this over time, so please offer any feedback in the form of comments on the document :)

Currently, only the `basic` algorithm is implemented. The program tries to find a solution for every $k$ from 1 to 1,000,000, and outputs how many integers it fails on.

## Credits
Credit to AR Booker for the original [```factor64```](https://github.com/arbooker/factor64) and to all the contributors of RidiculousFish's [```libdivide```](https://github.com/ridiculousfish/libdivide). All other code is my own.

## Technical Aspects
This program has only been desinged to run on Linux. To run, clone this repo with ```git clone --recurse-submodules https://github.com/FredRhys/3SumGTN```, then run ```make``` to compile.

You are suggested to take advantage of ```run.sh``` and call it using ```make run```. A default version will be created by running ```make run.sh```. Configure it by adjusting:

    ```build/main RANGE_MIN RANGE_MAX NUM_THREADS DIVISOR_MIN DIVISOR_MAX```.

The program will iterate over every *k* from ```RANGE_MIN``` to ```RANGE_MAX```, using the specified number of threads, and call the basic algorithm to try and solve for it. If this fails, then it repeatedly calls the advanced algorithm, doubling ```DIVISOR_MIN``` until it exceeds ```DIVISOR_MAX```. Large values for ```DIVISOR_MIN``` and ```DIVISOR_MAX``` will significantly increase runtime but may be necessary to find solutions for some values of ```k```.

[CodeCogs](https://editor.codecogs.com/) was used for Latex rendering in ```README.md```.

## Fun Facts
- The smallest integer the basic algorithm can't find a solution to is 107 = (&minus;393)C3 + (328)C3 + (298)C3.

- The basic algorithm can find solutions to all but 12,993 integers below 1,000,000.
- The advanced algorithm with a maximum divbound of 739 (equivalent to the original search) can find solutions to all but 284 integers below 1,000,000.
    