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
    double sfd_scale, bmd_scale;
    std::cout << "sfd scale: ";
    std::cin >> sfd_scale;
    std::cout << "bmd scale: ";
    std::cin >> bmd_scale;
    std::ostringstream ss;
    ss << "python src/diagram.py " << "outputs/" << input_frame <<
        " " << sfd_scale << " " << bmd_scale;
    system_call(ss.str());

    return 0;
}
