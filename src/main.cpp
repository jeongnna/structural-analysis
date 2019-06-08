#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>
#include "structure.h"


std::string get_input_frame() {
    std::string s;
    std::regex e("^data/|.txt$");
    std::cout << "Input frame: ";
    std::cin >> s;
    return std::regex_replace(s, e, "");
}


void system_call(std::string command) {
    char cmd[command.size() + 1];
    strcpy(cmd, &command[0]);
    std::system(cmd);
}


int main() {
    // get input file and build the frame
    std::string input_frame = get_input_frame();
    system_call("mkdir outputs/" + input_frame);
    Frame frame = FrameUtil::construct("data/" + input_frame + ".txt");

    // compute
    frame.compute_displacement();
    frame.compute_reaction("outputs/" + input_frame + "/reaction.json");

    // draw structure diagram (running python script)
    std::ostringstream ss;
    ss << "python src/diagram.py " <<
        "outputs/" << input_frame << "/reaction.json " <<
        "outputs/" << input_frame << "/diagramdata.txt " <<
        "outputs/" << input_frame << "/diagram.png";
    system_call(ss.str());

    return 0;
}
