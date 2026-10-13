# FS-AI 2026 Individual Challenge: &lt;your name&gt;

> Replace everything in this file with your own write-up. See `INTERFACE.md` for how the code is run.

**Task chosen:** 2a (path planning)
**Language:** C++.

## How to run my visualisation

```bash
cmake -S cpp -B cpp/build
cmake --build cpp/build
./cpp/build/runner 1
```

## Task 1

Firstly, I made two functions that removed all whitespaces from the text in the csv file and made it all lowercase in case it was different formats. Then, since I had to read the file line by line, the whole line would be a single string, so I separated the string into a vector separated by commas, so it would be easy to manipulate later. Then I read the file line by line, and for each line, I passed it through those 3 functions, then realised I had to convert 2 of them into doubles since they were strings, so I made a function to do that. Also I had to make sure I was passing the numbers, not the colour, so I checked which ones were the x and y coordinates, then passed those. Then, for each vector, I made a cone, assigning which value was what, then added each vector to the tuple. P.S. I severly underestimated how long this would take as this was my first time doing something like this in C++, so I didn't have much time to attempt task 2 lol. Hope you understand. 


## Task 2

Approach, design decisions, assumptions, and the algorithm you chose.

## Limitations and what I'd do with more time
