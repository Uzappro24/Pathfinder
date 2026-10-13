# FS-AI 2026 Individual Challenge: &lt;your name&gt;

> Replace everything in this file with your own write-up. See `INTERFACE.md` for how the code is run.

**Task chosen:** 2a (path planning)
**Language:** C++.

## How to run my visualisation

```bash
cmake -S cpp -B cpp/build
cmake --build cpp/build
./cpp/build/visualise
```

## Task 1

Firstly, I made two functions that removed all whitespaces from the text in the csv file and made it all lowercase in case it was different formats. Then, since I had to read the file line by line, the whole line would be a single string, so I separated the string into a vector separated by commas, so it would be easy to manipulate later. Then I read the file line by line, and for each line, I passed it through those 3 functions, then realised I had to convert 2 of them into doubles since they were strings, so I made a function to do that. Also I had to make sure I was passing the numbers, not the colour, so I checked which ones were the x and y coordinates, then passed those. Then, for each vector, I made a cone, assigning which value was what, then added each vector to the tuple.


## Task 2

I used a midpoint path then I smoothed it out a few times. I first made a path through the middle of the track and the smoothed the route but made sure it didn't come close to the cones. I first separated the cones into blue cones and yellow cones. I also made sure there are at least two cones of each. 
Then I added some more points on the blue boundary and for each point I then found the nearest position on the yellow boundary and calculated the midpoint between them. The midpoints made a path that was in the middle of the track. 
I also had to make sure the racing line was in the right direction of the track (blue on the left). I used 2D cross product and if it was, the blue line is on the left. I added these results together and ff the total was negative, I reversed the order of the points
During smoothing, the racing line could've moved closer to the cones so to avoid this, I calculated a safe radius for every midpoint and for each point, and during smoothing, I made sure that each point was inside its own region.
Then  I applied smoothed the line repeatedly: for each point, I calculated the average position of the two points next to it and moved it towards that position. This made the path less sharp and also shortened it. If the position was outside the safe radius, I moved it back to the edge of its safe radius.

Assumptions: It's a closed loop, the cones are already ordered and the blue cones are on the left boundary and yellow cones on the right and ignored other cones. If I had more time, I wouldn't have made these assumptions and try to find a solution for each of these.