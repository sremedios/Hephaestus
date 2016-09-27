#define WINVER 0x0500
#include <windows.h>
#include <iostream>
#include <string.h>
#include "Leap.h"

using namespace Leap;
//using namespace std;

// adding SampleListener class to main program
class SampleListener : public Listener {
public:
	virtual void onConnect(const Controller&);
	virtual void onFrame(const Controller&);
	std::string GetPalmPosition(const Vector&);
	int TestType(const Vector&);
	bool IsInSphere(const Vector&, const Vector&);
	int GetTouching(const Hand&LHand, const Hand&RHand);

private:
	Vector indexTipL;
	Vector middleTipL;
	Vector ringTipL;
	Vector pinkyTipL;
	Vector thumbTipL;
	Vector indexTipR;
	Vector middleTipR;
	Vector ringTipR;
	Vector pinkyTipR;
	Vector thumbTipR;
};

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const float FINGER_SPHERE_RADIUS = 50.0; // in mm
const float SLEEP_AMOUNT = 350.0; // in ms

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
	const Frame frame = controller.frame();
	

	HandList hands = frame.hands();

	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		// store finger IDs
		int fingerIterator = 0;
		int32_t finger_IDs[5];

		// Get the fingers
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;

			// set finger_IDs
			finger_IDs[fingerIterator] = finger.id();
			// ensure iterator doesn't exceed 4
			fingerIterator++;
			if (fingerIterator > 4)
				fingerIterator = 0;

			if (hand.isLeft()) {
				indexTipL = hand.finger(finger_IDs[1]).tipPosition();
				middleTipL = hand.finger(finger_IDs[2]).tipPosition();
				ringTipL = hand.finger(finger_IDs[3]).tipPosition();
				pinkyTipL = hand.finger(finger_IDs[4]).tipPosition();
				thumbTipL = hand.finger(finger_IDs[0]).tipPosition();
			}

			else if (hand.isRight()) {
				indexTipR = hand.finger(finger_IDs[1]).tipPosition();
				middleTipR = hand.finger(finger_IDs[2]).tipPosition();
				ringTipR = hand.finger(finger_IDs[3]).tipPosition();
				pinkyTipR = hand.finger(finger_IDs[4]).tipPosition();
				thumbTipR = hand.finger(finger_IDs[0]).tipPosition();
			}

			// get bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
			}

		}
		//
		// inside hand loop, after finger loop
		//





		const Vector indexDistVect = hand.finger(finger_IDs[1]).bone(static_cast<Bone::Type>(3)).direction();
		const Vector thumbDistVect = hand.finger(finger_IDs[0]).bone(static_cast<Bone::Type>(3)).direction();




		// non-hand specific
		const Vector indexTip = hand.finger(finger_IDs[1]).tipPosition();
		const Vector middleTip = hand.finger(finger_IDs[2]).tipPosition();
		const Vector ringTip = hand.finger(finger_IDs[3]).tipPosition();
		const Vector pinkyTip = hand.finger(finger_IDs[4]).tipPosition();
		const Vector thumbTip = hand.finger(finger_IDs[0]).tipPosition();

		/**
			THE FOLLOWING ARE NUMBER CODES TO TELL WHICH FINGERS ARE BEING TOUCHED
			LEFT HAND:	INDEX = 1
						MIDDLE = 2
						RING = 3
						PINKY = 4
			RIGHT HAND:	INDEX = 11
						MIDDLE = 12
						RING = 13
						PINKY = 14

			EXAMPLE COMBINATIONS:
				Index Left && Middle Right	=	112
				nothing Left && Pinky Right	=	14
				Ring Left && Ring Right		=	113

			IE: right hand is 10 more than left hand

			ABOVE IS USED WITH SWITCH STATEMENT AND VARIABLE "touchCombo"
		*/

		// used to simulate keyboard
		INPUT ip;

		// Set up a generic keyboard event.
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0; // hardware scan code for key
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

	/**
	
				FIXED LEFT VS RIGHT HAND ISSUE BY MOVING
				FINGERTIP VECTORS INTO THE PRIVATE DATA
				FOR LISTENER CLASS.

				MAY WANT TO IMPLEMENT AN EXTENDED HAND CLASS FOR IMPORTANT VARIABLES

				NOW NEED TO FIND ISSUE OF WHY TYPED CHARACTERS ARE BEING REPEATED
				MAYBE RELATED TO HAVING TWO SETS OF FINGERTIPS ALLOCATED?

				ALSO NEED TO JUDGE WHETHER SWITCH IS BETTER/WORSE THAN IF/ELSE
				I THINK THEY'RE PROBABLY THE SAME BECAUSE I HAVE TO ANYWAYS RUN THROUGH
				A BUNCH OF IF/ELSE TO SET UP THE SWITCH'S CONTROL STATEMENT.

				MAY WANT TO IMPROVE MY ISINSPHERE METHOD TO DETERMINE WHICH FINGERS ARE TOUCHING
				RATHER THAN PASSING THEM IN AS PARAMETERS EACH TIME

	*/

		// Switch method.  May be slower than an if/else multi-way?? may be causing duplicate letters? See below for if/else block
		{
			int touchCombo = 9999; // value to make below statements false

			if (IsInSphere(indexTipL, thumbTipL)
				&& IsInSphere(indexTipR, thumbTipR))
				touchCombo = 111;

			else if (IsInSphere(indexTipL, thumbTipL)
				&& !IsInSphere(indexTipR, thumbTipR))
				touchCombo = 1;

			else if (IsInSphere(indexTipR, thumbTipR))
				touchCombo = 11;

			else
				touchCombo = 9999;


			switch (touchCombo)
			{
			case 1:		// Press the "H" key
				ip.ki.wVk = 0x48; // virtual-key code f
				break;
			case 11:	// Press the "J" key
				ip.ki.wVk = 0x4A;
				break;
			case 111:	// Press the "I" key
				ip.ki.wVk = 0x49; // virtual-key code f
				break;
			default:
				break;
			}
		}

		// if/else method. might be faster than switch? still getting duplicates
		{
			if (IsInSphere(indexTipL, thumbTipL)
				&& IsInSphere(indexTipR, thumbTipR)) {
				ip.ki.wVk = 0x4A;
			}
			else if (IsInSphere(indexTipL, thumbTipL)
				&& !IsInSphere(indexTipR, thumbTipR)) {
				ip.ki.wVk = 0x4B;
			}
			else if (IsInSphere(indexTipR, thumbTipR)) {
				ip.ki.wVk = 0x4C;
			}

			else if (IsInSphere(middleTipL, thumbTipL)
				&& IsInSphere(middleTipR, thumbTipR)) {
				ip.ki.wVk = 0x45;
			}
			else if (IsInSphere(middleTipL, thumbTipL)
				&& !IsInSphere(middleTipR, thumbTipR)) {
				ip.ki.wVk = 0x46;
			}
			else if (IsInSphere(middleTipR, thumbTipR)) {
				ip.ki.wVk = 0x47;
			}
		}



		// PROBLEM: cannot differentiate when specific hand's fingers are touching
		//  
		//	if (IsInSphere(indexTip, thumbTip, FINGER_SPHERE_RADIUS)) {
		//		// Press the "H" key
		//		ip.ki.wVk = 0x48; // virtual-key code for the "I" key
		//	}
		//	
		//	
		//	if (IsInSphere(middleTip, thumbTip, FINGER_SPHERE_RADIUS)) {
		//		// Press the "E" key
		//		ip.ki.wVk = 0x45; // virtual-key code for the "E" key
		//	
		//	}
		//	if (IsInSphere(ringTip, thumbTip, FINGER_SPHERE_RADIUS)) {
		//		// Press the "L" key
		//		ip.ki.wVk = 0x4C; // virtual-key code for the "L" key
		//	
		//	}
		//	if (IsInSphere(pinkyTip, thumbTip, FINGER_SPHERE_RADIUS)) {
		//		/// press the enter key
		//		ip.ki.wVk = 0x0D; // virtual-key code for the "L" key
		//	
		//		//// Press the "O" key
		//		//ip.ki.wVk = 0x4F; // virtual-key code for the "O" key
		//	}


		// finishing keystroke code
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
		// Release key at end of frame
		ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		SendInput(1, &ip, sizeof(INPUT));
		

		// if (TestType(normal) == 0) {
		// 	// Press the "A" key
		// 	ip.ki.wVk = 0x41; // virtual-key code for the "a" key
		// 	ip.ki.dwFlags = 0; // 0 for key press
		// 	SendInput(1, &ip, sizeof(INPUT));
		// 
		// 	// Release the "A" key
		// 	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		// 	SendInput(1, &ip, sizeof(INPUT));
		// }
		// else if (TestType(normal) > 0) {
		// 	// Press the "S" key
		// 	ip.ki.wVk = 0x53; // virtual-key code for the "s" key
		// 	ip.ki.dwFlags = 0; // 0 for key press
		// 	SendInput(1, &ip, sizeof(INPUT));
		// 
		// 	// Release the "S" key
		// 	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		// 	SendInput(1, &ip, sizeof(INPUT));
		// }
		// else {
		// 	// Press the "M" key
		// 	ip.ki.wVk = 0x4D; // virtual-key code for the "M" key
		// 	ip.ki.dwFlags = 0; // 0 for key press
		// 	SendInput(1, &ip, sizeof(INPUT));
		// 
		// 	// Release the "M" key
		// 	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		// 	SendInput(1, &ip, sizeof(INPUT));
		// }

		Sleep(SLEEP_AMOUNT);

		//const Vector indexCrossThumbDistVect = indexDistVect.cross(thumbDistVect);

		// execute every 10 frames
		// if (frame.id() % 10 == 0) {
		// 	//std::cout << hand.finger(finger_IDs[1]).tipPosition()
		// 	//	<< std::string(4, ' ')
		// 	//	<< hand.finger(finger_IDs[0]).tipPosition()
		// 	//	<< std::endl;
		// 
		// 	//std::cout << indexCrossThumbDistVect
		// 	//	<< -thumbDistVect.cross(indexCrossThumbDistVect)
		// 	//	<< std::endl;
		// 
		// 	std::cout << indexDistVect.angleTo(thumbDistVect) * RAD_TO_DEG << std::endl;
		// }

		
		// if (hand.finger(finger_IDs[1]).tipPosition() == hand.finger(finger_IDs[0]).tipPosition()) {
		// 	std::cout << "touching" << std::endl;
		// }

	
		// NOTE: following commented code moved to GetPalmPosition
		// test palm up or down?
		//if (normal.roll() * RAD_TO_DEG > -30 && normal.roll() * RAD_TO_DEG < 30)
		//	std::cout << "Palm down";
		//else if ((normal.roll() * RAD_TO_DEG > 160 && normal.roll() * RAD_TO_DEG < 180)
		//				|| 
		//		(normal.roll() * RAD_TO_DEG < -140 && normal.roll() * RAD_TO_DEG > -180))
		//	std::cout << "Palm up";
		//

		//std::cout << GetPalmPosition(normal);
	
	
	// Calculate the hand's pitch, roll, and yaw angles
	// std::cout << std::string(2, ' ') << "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
	// 	<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
	// 	<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
	//
	//
	//	// Get the Arm bone
	//	Arm arm = hand.arm();
	//	std::cout << std::string(2, ' ') << "Arm direction: " << arm.direction()
	//		<< " wrist position: " << arm.wristPosition()
	//		<< " elbow position: " << arm.elbowPosition() << std::endl;
	//
	//	// Get fingers
	//	const FingerList fingers = hand.fingers();
	//	for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
	//		const Finger finger = *fl;
	//		std::cout << std::string(4, ' ') << fingerNames[finger.type()]
	//			<< " finger, id: " << finger.id()
	//			<< ", length: " << finger.length()
	//			<< "mm, width: " << finger.width() << std::endl;
	//
	//		// Get finger bones
	//		for (int b = 0; b < 4; ++b) {
	//			Bone::Type boneType = static_cast<Bone::Type>(b);
	//			Bone bone = finger.bone(boneType);
	//			std::cout << std::string(6, ' ') << boneNames[boneType]
	//				<< " bone, start: " << bone.prevJoint()
	//				<< ", end: " << bone.nextJoint()
	//				<< ", direction: " << bone.direction() << std::endl;
	//		}
	//	}
	}

	//if (!frame.hands().isEmpty()) {
	//	std::cout << std::endl;
	//}


	// output frame info
	// std::cout << "Frame id: " << frame.id()
	// 	<< ", timestamp: " << frame.timestamp()
	// 	<< ", hands: " << frame.hands().count()
	// 	<< ", fingers: " << frame.fingers().count()
	// 	<< std::endl;
}

std::string SampleListener::GetPalmPosition(const Vector& v) {

	// test palm up or down?
	if (v.roll() * RAD_TO_DEG > -30 && v.roll() * RAD_TO_DEG < 30)
		return "Palm down";
	else if ((v.roll() * RAD_TO_DEG > 160 && v.roll() * RAD_TO_DEG < 180)
		||
		(v.roll() * RAD_TO_DEG < -140 && v.roll() * RAD_TO_DEG > -180))
		return "Palm up";
	else
		return "Palm Sideways";
}

int SampleListener::TestType(const Vector& v) {

	// test palm up or down?
	if (v.roll() * RAD_TO_DEG > -30 && v.roll() * RAD_TO_DEG < 30)
		return -1;
	else if ((v.roll() * RAD_TO_DEG > 160 && v.roll() * RAD_TO_DEG < 180)
		||
		(v.roll() * RAD_TO_DEG < -140 && v.roll() * RAD_TO_DEG > -180))
		return 1;
	else
		return 0;
}

bool SampleListener::IsInSphere(const Vector&finger, const Vector&thumb) {
	float xF = finger.x;
	float yF = finger.y;
	float zF = finger.z;
	float xT = thumb.x;
	float yT = thumb.y;
	float zT = thumb.z;

	return((pow(xT - xF, 2) + pow(yT - yF, 2) + pow(zT - zF, 2)) <= pow(FINGER_SPHERE_RADIUS, 2));
}

/** what if to determine which finger parts are touching others, 
	i pass in the HandList as a parameter, iterate through the hands, 
	and return a string? int? based on which are touching

	maybe need to have algorithm for sum of ints depending on which fingers are touching
	eg: 100 for left index, 1000 for left middle, etc
		and 1 for right index, 2 for right middle, etc

		so what happens is left index touching thumb + right index touching thumb == 101
		Lmiddle + Rright == 1003
		0 == no fingers touching so only Rmiddle == 2

	if I'm gonna do this for 3 points per hand, will need more numbers
	starting at 100 for left fingertips so right can be 1-12

	pass one of hand:number assignments:
	LEFT HAND:
				No touch					: 0
				Fingertips (index - pinky)	: 100, 1 000, 10 000, 100 000
				Middle phalanx				: 200, 2 000, 20 000, 200 000
				Proximal Phalanx			: 300, 3 000, 30 000, 300 000
	RIGHT HAND:
				No touch					: 0
				Fingertips (index - pinky)	: 1, 2, 3, 4
				Middle phalanx				: 5, 6, 7, 8
				Proximal Phalanx			: 9, 10, 11, 12

	therefore, all possible combinations of "one touch per hand" give unique values to parse.

	** NOTE** if I want to have multiple touches per hand, will probably need a different system, but for now
	i think just 1 touch per hand will be enough
*/

int SampleListener::GetTouching(const Hand&LHand, const Hand&RHand) 
// this function returns an appropriate int value relating to the above code of which fingers are touching per hand
// this function assumes all fingers and bones have been assigned
{
	int sum = 0;

	if (IsInSphere(indexTipL, thumbTipL)) {
		sum = 100;

		/**
				GETTING A BUG HERE REGARDING SCOPE OF FUNCTIONS
				IT'S TIME TO EXTEND THE HAND CLASS TO INCLUDE ALL RELEVANT 
				VARIABLES AND FUNCTIONS, SINCE WHAT I WANT TO BE DOING IS
				MOSTLY RELATED TO MEASURING THE HANDS, NOT EXTENDING THE LISTENER

				THE LISTENER SHOULD MOSTLY JUST ASSIGN THE HANDS THROUGH THE ONFRAME() CALL

				THINGS I NEED FOR THE HAND CLASS:
				ISINSPHERE
				GETTOUCHING
				ALL VARIABLES FOR RELEVANT PHALANGES
				MORE FUNCTIONS??

				NEED TO DECIDE WHERE THE ACTUAL KEYBOARD TYPING FUNCTION WILL GO
				EITHER IN LISTENER, MAIN, HANDS, OR ITS OWN
				UNSURE
		*/
	}
	
	return sum;
}

/**
int main(int argc, char** argv) {
	SampleListener listener;
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
*/
