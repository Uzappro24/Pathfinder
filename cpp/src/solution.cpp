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
    std::string line;
    std::vector<Cone> cones;

    std::size_t s = 0, x = 0, y = 0;

    if (std::getline(file, line)) {
        const std::vector<std::string> header = split(line);
        for (std::size_t i = 0; i < header.size(); ++i) {
            if (header[i] == "side") {
                    s = i;
            } else if (header[i] == "x") {
                    x = i;
            } else if (header[i] == "y") {
                    y = i;
            }
        }
    }

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        const std::vector<std::string> row = split(line);

        Cone cone;
        cone.side = row[s];
        cone.x = string_to_double(row[x]);
        cone.y = string_to_double(row[y]);
        cones.push_back(cone);
    }
    return cones;
}

// ---------------------------------------------------------------- Task 2a
std::vector<Point2> plan_racing_line(const std::vector<Cone>& cones) {
    (void)cones;
    throw NotImplemented();
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
