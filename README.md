# 3SumGTN

A reimplementation of [my master's thesis code](https://github.com/FredRhys/4th-year-diss-code), made without deadline stress.

This program seeks integer solutions to the equation:

![equation](https://latex.codecogs.com/svg.image?\binom{x}{3}&plus;\binom{y}{3}&plus;\binom{z}{3}=k)

for given $k$. From now on the binomial symbol will be written ($x$)C3

For a full explanation, see [my thesis](https://drive.google.com/file/d/1LHM9BBNr0isWwWY-byo3zMEb0KBxi436/view?usp=sharing). I intend to update this over time, so please offer any feedback in the form of comments on the document :)

Currently, only the `basic` algorithm is implemented. The program tries to find a solution for every $k$ from 1 to 1,000,000, and outputs how many integers it fails on.

## Credits
Credit to AR Booker for the original [```factor64```](https://github.com/arbooker/factor64). [My fork](https://github.com/FredRhys/factor64) was made only to provide a ```.gitignore```. Credit also to him for being my supervisor during the original project. All other code is my own.

## Technical Aspects
This program has only been desinged to run on Linux. To run, clone this repo with ```git clone --recurse-submodules https://github.com/FredRhys/3SumGTN```, then run ```make``` to compile. Running ```build/main``` will perform the search. [CodeCogs](https://editor.codecogs.com/) was used for Latex rendering in ```README.md```.

## Fun Facts
- The smallest integer the basic algorithm can't find a solution to is 107 = (&minus;393)C3 + (328)C3 + (298)C3.

- The basic algorithm can find solutions to all but 12,993 integers below 1,000,000.