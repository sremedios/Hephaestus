#include <iostream>
#include <string.h>
#include "Leap.h"
#include "HephListener.h"

using namespace Leap;

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };

int main(int argc, char** argv) {
	HephListener listener;
	Controller controller;

	// run program even in background
	controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	controller.addListener(listener);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);

	return 0;
}