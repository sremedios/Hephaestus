/**

			NOTES: 
			ACCURACY OF LEAP MOTION CONTROLLER APPEARS TO BE TOO LOW ATM TO TRACK TIP DIP AND PIP ACCURATELY.
			EITHER FIND A WAY TO IMPROVE ACCURACY OR FIND NEW MAPPINGS FOR TYPING.

			ALL OTHER CODE APPEARS TO BE WORKING PROPERLY

			MAIN IS NOW IN TEST_002.  TEST_001 IS DEPRECATED BUT USED FOR REFERENCE.

			ANYTHING THAT SHOULD BE CHECKED EVERY FRAME (IE: FINGER POSITIONS) SHOULD BE CALLED IN THE
			ONFRAME() METHOD 


*/


#define WINVER 0x0500
#include "HephListener.h"
#include <windows.h>
#include <iostream>

using namespace Leap;

void HephListener::onConnect(const Controller& controller) 
/** DESCRIPTION:	This function declares when the listener connects
*/
{
	std::cout << "Connected" << std::endl;
}

void HephListener::onFrame(const Controller& controller) 
/** DESCRIPTION:	This function handles all logic to be executed in each frame
					including assigning all hand objects and handling all system output
					This function is essentially the "main" function
*/
{
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

		// Get the fingers
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;

			// set finger_IDs
			finger_IDs[fingerIterator] = finger.id();
			
			// get bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
			}

			AssignAnatomy(hand, hand.isLeft(), fingerIterator); // track all tip and joint positions
			
			fingerIterator++;
			if (fingerIterator > 4) // ensure iterator doesn't exceed 4
				fingerIterator = 0;
		}

		// inside hand loop, after finger loop

		HephType(GetTouching());

		// per frame tests
		std::cout << tipIndexL << " " << GetTouching() << std::endl;

		// pause for a moment
		Sleep(SLEEP_AMOUNT);
	}
}

bool HephListener::IsInSphere(const Vector&finger, const bool isLeft)
/** DESCRIPTION:	This function determines if a finger is touching the thumb
	PARAMTERS:		Vector finger is the finger to be checked
					bool isLeft states whether the finger to be checked is on the left hand or not
	RETURNS:		true if touching, false if not
*/
{
	float xF = finger.x;
	float yF = finger.y;
	float zF = finger.z;
	float xT, yT, zT;

	if (isLeft) {
		xT = tipThumbL.x;
		yT = tipThumbL.y;
		zT = tipThumbL.z;
	}
	else {
		xT = tipThumbR.x;
		yT = tipThumbR.y;
		zT = tipThumbR.z;
	}

	return((pow(xT - xF, 2) + pow(yT - yF, 2) + pow(zT - zF, 2)) < pow(FINGER_SPHERE_RADIUS, 2));
}

int32_t HephListener::GetPalmPosition(const Vector&palmNormal)
/** DESCRIPTION:	This function returns -1, 0, or 1 depending on the palm's normal vector
	RETURNS:		-1 is facing downwards, 1 is upwards, and 0 is anything else.
*/
{
	// test palm up or down?
	if (palmNormal.roll() * RAD_TO_DEG > -30 && palmNormal.roll() * RAD_TO_DEG < 30)
		return -1;
	else if ((palmNormal.roll() * RAD_TO_DEG > 160 && palmNormal.roll() * RAD_TO_DEG < 180) ||
		(palmNormal.roll() * RAD_TO_DEG < -140 && palmNormal.roll() * RAD_TO_DEG > -180))
		return 1;
	else
		return 0;
}

int32_t HephListener::GetTouching()
/** DESCRIPTION:	This function determines which fingers are touching.
	RETURNS:		LEFT HAND:
						No touch					: 0
						Fingertips (index - pinky)	: 100, 1 000, 10 000, 100 000
						Middle phalanx				: 200, 2 000, 20 000, 200 000
						Proximal Phalanx			: 300, 3 000, 30 000, 300 000
					RIGHT HAND:
						No touch					: 0
						Fingertips (index - pinky)	: 1, 2, 3, 4
						Middle phalanx				: 5, 6, 7, 8
						Proximal Phalanx			: 9, 10, 11, 12

					Only one touch per hand is allowed.
					Returned value is the sum of left and right hands.
*/
{
	int sum = 0;
	// left tips
	if (IsInSphere(tipIndexL, true))
		sum = 100;
	if (IsInSphere(tipMiddleL, true))
		sum += 1000;
	if (IsInSphere(tipRingL, true))
		sum += 10000;
	if (IsInSphere(tipPinkyL, true))
		sum += 100000;
	
	// left dips
	if (IsInSphere(dipIndexL, true))
		sum += 200;
	if (IsInSphere(dipMiddleL, true))
		sum += 2000;
	if (IsInSphere(dipRingL, true))
		sum += 20000;
	if (IsInSphere(dipPinkyL, true))
		sum += 200000;
	
	// left pips
	if (IsInSphere(pipIndexL, true))
		sum += 300;
	if (IsInSphere(pipMiddleL, true))
		sum += 3000;
	if (IsInSphere(pipRingL, true))
		sum += 30000;
	if (IsInSphere(pipPinkyL, true))
		sum += 300000;
	
	// right tips
	if (IsInSphere(tipIndexR, true))
		sum += 1;
	if (IsInSphere(tipMiddleR, true))
		sum += 2;
	if (IsInSphere(tipRingR, true))
		sum += 3;
	if (IsInSphere(tipPinkyR, true))
		sum += 4;
	
	// right dips
	if (IsInSphere(dipIndexR, true))
		sum += 5;
	if (IsInSphere(dipMiddleR, true))
		sum += 6;
	if (IsInSphere(dipRingR, true))
		sum += 7;
	if (IsInSphere(dipPinkyR, true))
		sum += 8;
	
	// right pips
	if (IsInSphere(pipIndexR, true))
		sum += 9;
	if (IsInSphere(pipMiddleR, true))
		sum += 10;
	if (IsInSphere(pipRingR, true))
		sum += 11;
	if (IsInSphere(pipPinkyR, true))
		sum += 12;

	return sum;
}

void HephListener::HephType(int32_t touchingCode)
/** DESCRIPTION:	This function determines which fingers are touching.
	PARAMETERS:		LEFT HAND:
						No touch					: 0
						Fingertips (index - pinky)	: 100, 1 000, 10 000, 100 000
						Middle phalanx				: 200, 2 000, 20 000, 200 000
						Proximal Phalanx			: 300, 3 000, 30 000, 300 000
					RIGHT HAND:
						No touch					: 0
						Fingertips (index - pinky)	: 1, 2, 3, 4
						Middle phalanx				: 5, 6, 7, 8
						Proximal Phalanx			: 9, 10, 11, 12

					Only one touch per hand is allowed.
					Returned value is the sum of left and right hands.
	POST:			A keystroke is emulated
*/
{

	// used to simulate keyboard
	INPUT ip;
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	switch (touchingCode) {
	case 100:
		ip.ki.wVk = 0x4A; // virtual-key code for the "J" key
		break;
	case 1000:
		ip.ki.wVk = 0x4B;  // virtual-key code for the "K" key
		break;
	};

	// finishing keystroke code
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	// Release key at end of frame
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

void HephListener::AssignAnatomy(const Hand&hand, const bool isLeft, const int fingerIterator)
/** DESCRIPTION:	This function assigns values to each of the joints and fingertips in the hand
	POST:			all joints and fingertip position vectors are assigned for the hand
*/
{
	// BUG: currently saving all info even if hand leaves zone.  Maybe reset to 0,0,0?
	if (isLeft) {
		switch (fingerIterator) {
		case 0:	tipThumbL = hand.finger(finger_IDs[0]).tipPosition();
			break;
		case 1:	tipIndexL = hand.finger(finger_IDs[1]).tipPosition();
			dipIndexL = hand.finger(finger_IDs[1]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipIndexL = hand.finger(finger_IDs[1]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		case 2:	tipMiddleL = hand.finger(finger_IDs[2]).tipPosition();
			dipMiddleL = hand.finger(finger_IDs[2]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipMiddleL = hand.finger(finger_IDs[2]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		case 3:	tipRingL = hand.finger(finger_IDs[3]).tipPosition();
			dipRingL = hand.finger(finger_IDs[3]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipRingL = hand.finger(finger_IDs[3]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		case 4:	tipPinkyL = hand.finger(finger_IDs[4]).tipPosition();
			dipPinkyL = hand.finger(finger_IDs[4]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipPinkyL = hand.finger(finger_IDs[4]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		};
	}
	else {
		switch (fingerIterator) {
		case 0:	tipThumbR = hand.finger(finger_IDs[0]).tipPosition();
			break;
		case 1:	tipIndexR = hand.finger(finger_IDs[1]).tipPosition();
			dipIndexR = hand.finger(finger_IDs[1]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipIndexR = hand.finger(finger_IDs[1]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		case 2:	tipMiddleR = hand.finger(finger_IDs[2]).tipPosition();
			dipMiddleR = hand.finger(finger_IDs[2]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipMiddleR = hand.finger(finger_IDs[2]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		case 3:	tipRingR = hand.finger(finger_IDs[3]).tipPosition();
			dipRingR = hand.finger(finger_IDs[3]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipRingR = hand.finger(finger_IDs[3]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		case 4:	tipPinkyR = hand.finger(finger_IDs[4]).tipPosition();
			dipPinkyR = hand.finger(finger_IDs[4]).bone(static_cast<Bone::Type>(3)).prevJoint();
			pipPinkyR = hand.finger(finger_IDs[4]).bone(static_cast<Bone::Type>(2)).prevJoint();
			break;
		};
	}
}