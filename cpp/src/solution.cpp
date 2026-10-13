// Your solution goes here.
//
// Implement Task 1 and the Task 2 option you chose. Leave the other function
// throwing NotImplemented. You may add other source files (list them in
// CMakeLists.txt), but keep these three functions exactly as declared in
// include/challenge.hpp: the marking harness calls them through the runner.
//
// Check your work (from the repository root):
//     cmake -S cpp -B cpp/build && cmake --build cpp/build
//     ./cpp/build/runner all
#include "challenge.hpp"
#include <sstream>
#include <fstream>
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// ---------------------------------------------------------------- Task 1

std::string remove_all_whitespace(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            result += c;
        }
    }
    return result;
}

std::string to_lowercase(const std::string& s){
    std:: string result = s;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}

// split on commas
std::vector<std::string> split(const std::string& line){
    std::string new_line = remove_all_whitespace(line);
    new_line = to_lowercase(new_line);
    std::stringstream ss(new_line);
    std::vector<std::string> result;
    std::string item; // std::getline() reads characters from an input stream and stores them in the string item until it reaches a delimiter ','
    while (std::getline(ss, item, ',')) {
        result.push_back(item); //push_back() adds a new element to the end of the vector
    }
    return result;
}

double string_to_double(const std::string& s) {
    try {
        return std::stod(s); // Convert string to double
    } catch (const std::invalid_argument& e) { // if the string does not contain a valid number
        throw std::runtime_error("Invalid argument: " + s);
    }
}

std::vector<Cone> parse_cones(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    std::vector<Cone> cones;

    std::size_t s = 0, x = 0, y = 0;
    bool found_side = false, found_x = false, found_y = false;

    if (std::getline(file, line)) {
        const std::vector<std::string> header = split(line);
        for (std::size_t i = 0; i < header.size(); ++i) {
            if (header[i] == "side") {
                    s = i;
                    found_side = true;
            } else if (header[i] == "x") {
                    x = i;
                    found_x = true;
            } else if (header[i] == "y") {
                    y = i;
                    found_y = true;
            }
        }
    }

    if (!found_side || !found_x || !found_y) {
        throw std::runtime_error("Did not have 3 things in the headers");
    }   

    while (std::getline(file, line)) {
        if (remove_all_whitespace(line).empty()) {
            continue; // Skip empty lines
        }
        const std::vector<std::string> row = split(line);

        
        if (row.size() <= s || row.size() <= x || row.size() <= y) { // checks if row has 3 things
            throw std::runtime_error("CSV row is missing required columns");
        }

        Cone cone;
        cone.side = row[s];
        cone.x = string_to_double(row[x]);
        cone.y = string_to_double(row[y]);
        cones.push_back(cone);
    }
    return cones;
}

// ---------------------------------------------------------------- Task 2a

const double safety_margin = 0.9; // looked up - 0.7 m is half the car's width,  0.12 m is cone radius and then rounded up

double distance(Point2 a, Point2 b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

Point2 nearest_point(Point2 p, const std::vector<Point2>& edge) {
    Point2 nearest = edge[0];
    double min_distance = 1e9; // 1 * 10^9, basically a big number

    for (std::size_t i = 0; i < edge.size(); i++) {
        Point2 a = edge[i];
        Point2 b = edge[
            (i + 1) % edge.size()
        ]; // the next point

        double dx = b.x - a.x;
        double dy = b.y - a.y;

        double length = dx * dx + dy * dy;

        double t = 0;
        if (length > 0) {
            t = ((p.x - a.x) * dx + (p.y - a.y) * dy) / length;
        }

        t = std::clamp(t, 0.0, 1.0); // only want between a and b

        Point2 closest = {a.x + t * dx,
            a.y + t * dy
        };

        double d = distance(p, closest);

        if (d < min_distance) {
            min_distance = d;
            nearest = closest;
        }
    }

    return nearest;
}


std::vector<Point2> add_points(const std::vector<Point2>& edge) { // add more points to make it smoother
    std::vector<Point2> result;

    for (std::size_t i = 0; i < edge.size(); i++) {
        Point2 a = edge[i];
        Point2 b = edge[(i + 1) % edge.size()];

        int steps = std::max(1, (int)std::ceil(distance(a, b)));

        for (int j = 0; j < steps; j++) {
            double t = (double)j / steps;

            result.push_back({
                a.x + t * (b.x - a.x),
                a.y + t * (b.y - a.y)
            });
        }
    }

    return result;
}

std::vector<Point2> plan_racing_line(const std::vector<Cone>& cones) {

    std::vector<Point2> blue, yellow;

    for (const Cone& cone : cones) {
        if (cone.side == "blue") {
            blue.push_back({cone.x, cone.y});
        }
        if (cone.side == "yellow") {
            yellow.push_back({cone.x, cone.y});
        }
    }

    if (blue.size() < 2 || yellow.size() < 2) { // check there are enough cones to make a racing line
        throw std::runtime_error("Not enough cones");
    }


    std::vector<Point2> left = add_points(blue);
    std::vector<Point2> midpoints;

    for (Point2 p : left) { // finding the nearest point on the yellow edge for each point on the blue edge
        Point2 q = nearest_point(p, yellow);

        midpoints.push_back({
            (p.x + q.x) / 2,
            (p.y + q.y) / 2
        });
    }


    double direction = 0;

    for (std::size_t i = 0; i < midpoints.size(); i++) { 
        Point2 a = midpoints[i];
        Point2 b = midpoints[(i + 1) % midpoints.size()];

        direction += (b.x - a.x) * (left[i].y - a.y) // cross product
                   - (b.y - a.y) * (left[i].x - a.x);
    }

    if (direction < 0) { // if the direction is negative, reverse the midpoints
        std::reverse(midpoints.begin(), midpoints.end());
    }


    std::vector<double> radius;

    for (Point2 p : midpoints) { // get a safe radius for each point on the midpoints
        double blue_dist = distance(p, nearest_point(p, blue));
        double yellow_dist = distance(p, nearest_point(p, yellow));

        double safe_radius = std::max(0.0,
                                    std::min(blue_dist, yellow_dist) - safety_margin); // we want the minimum distance to the nearest cone minus the safety margin

        radius.push_back(safe_radius);
    }


    std::vector<Point2> line = midpoints;
    int n = line.size();

    for (std::size_t iteration = 0; iteration < 5000; iteration++) { // repeatedly move the points towards the average of the ones next to it, but not more than the safe radius

        for (std::size_t i = 0; i < n; i++) {

            Point2 prev = line[(i + n - 1) % n];
            Point2 next = line[(i + 1) % n];

            Point2 p = {
                (prev.x + next.x) / 2,
                (prev.y + next.y) / 2
            };

            double dx = p.x - midpoints[i].x;
            double dy = p.y - midpoints[i].y;
            double d = std::hypot(dx, dy);


            if (d > radius[i]) {
                p.x = midpoints[i].x + dx / d * radius[i];
                p.y = midpoints[i].y + dy / d * radius[i];
            }

            line[i] = p;
        }
    }
    return line;
}

// ---------------------------------------------------------------- Task 2b
std::vector<Projection> project_cones(const std::vector<Cone>& cones,
                                      const Pose& pose,
                                      const Camera& camera) {
    (void)cones;
    (void)pose;
    (void)camera;
    throw NotImplemented();
}
