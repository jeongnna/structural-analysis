#include "structure.h"


int main() {
    Frame frame = FrameUtil::construct("data/test_frame.txt");
    frame.compute_displacement();
    frame.compute_reaction();

    return 0;
}
