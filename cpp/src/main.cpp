// Your visualisation entry point. Edit freely.
//
// Build and run (from the repository root):
//     cmake -S cpp -B cpp/build && cmake --build cpp/build
//     ./cpp/build/visualise
#include <iostream>

#include "challenge.hpp"
#include "challenge_io.hpp"
#include <filesystem>


int main() {
    std::vector<Cone> cones = parse_cones(challenge_io::kDataDir + "/track.csv");
    std::vector<Point2> line = plan_racing_line(cones);

    double minX = cones[0].x, maxX = cones[0].x;
    double minY = cones[0].y, maxY = cones[0].y;

    for (const Cone& cone : cones) {
        minX = std::min(minX, cone.x);
        maxX = std::max(maxX, cone.x);
        minY = std::min(minY, cone.y);
        maxY = std::max(maxY, cone.y);
    }

    double scale = std::min(900.0 / (maxX - minX),
                            700.0 / (maxY - minY));

    auto x = [&](double value) {
        return (value - minX) * scale + 50;
    };

    auto y = [&](double value) {
        return (maxY - value) * scale + 50;
    };


    std::filesystem::create_directories(challenge_io::kOutputDir);
    std::ofstream svg(challenge_io::kOutputDir + "/track.svg");

    svg << "<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='800'>\n";
    svg << "<rect width='100%' height='100%' fill='white'/>\n";

    svg << "<polyline fill='none' stroke='red' stroke-width='3' points='";

    for (const Point2& p : line) {
        svg << x(p.x) << "," << y(p.y) << " ";
    }

    svg << x(line[0].x) << "," << y(line[0].y) << "'/>\n";

    for (const Cone& cone : cones) {
        std::string colour = "orange";

        if (cone.side == "blue") {
            colour = "blue";
        }
        else if (cone.side == "yellow") {
            colour = "yellow";
        }

        svg << "<circle cx='" << x(cone.x)
            << "' cy='" << y(cone.y)
            << "' r='5' fill='" << colour << "'/>\n";
    }

    svg << "<circle cx='" << x(line[0].x)
        << "' cy='" << y(line[0].y)
        << "' r='6' fill='green'/>\n";

    svg << "</svg>";

    return 0;
}