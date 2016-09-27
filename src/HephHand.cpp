/**

			NOTE: DEPRECATED

*/

#include "HephHand.h"

HephHand::HephHand():Hand()
/** DESCRIPTION:	Initializer; should only be a copy of the original hand.
*/
{}

bool HephHand::IsInSphere(const Vector&finger, const bool isLeft)
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
	
	return((pow(xT - xF, 2) + pow(yT - yF, 2) + pow(zT - zF, 2)) <= pow(FINGER_SPHERE_RADIUS, 2));
}

int32_t HephHand::GetPalmPosition(const Vector&palmNormal) 
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

int32_t HephHand::GetTouching()
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
		sum += 100;
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

void HephHand::AssignAnatomy(const bool isLeft)
/** DESCRIPTION:	This function assigns values to each of the joints and fingertips in the hand
	POST:			all joints and fingertip position vectors are assigned for the hand
*/
{
	if (isLeft) {
		tipIndexL = finger(1).tipPosition();
		tipMiddleL = finger(2).tipPosition();
		tipRingL = finger(3).tipPosition();
		tipPinkyL = finger(4).tipPosition();
		tipThumbL = finger(0).tipPosition();
	}

	else{
		tipIndexR = finger(1).tipPosition();
		tipMiddleR = finger(2).tipPosition();
		tipRingR = finger(3).tipPosition();
		tipPinkyR = finger(4).tipPosition();
		tipThumbR = finger(0).tipPosition();
	}
}