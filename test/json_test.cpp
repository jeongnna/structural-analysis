#include <vector>
#include <fstream>
#include <iomanip>  // std::setw
#include "../lib/json.hpp"

using json = nlohmann::json;


int main() {
    std::ofstream outfile;
    outfile.open("json_test.json");

    json j1, j2;
    j1["x"] = 1;
    j1["y"] = 2;
    j2["x"] = 3;
    j2["y"] = 4;

    json jsn;
    jsn["list"] = json::array();
    jsn["list"].push_back(j1);
    jsn["list"].push_back(j2);

    std::vector<int> vec({1, 2, 3});
    jsn["vec"] = vec;

    outfile << std::setw(4) << jsn << std::endl;

    return 0;
}
