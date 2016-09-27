/**

NOTE: DEPRECATED

*/


#if !defined(__Heph_Hand_h__)
#define __Heph_Hand_h__

#include "Leap.h"
using namespace Leap;

class HephHand : public Hand {
public:
	HephHand();
	bool IsInSphere(const Vector&finger, const bool isLeft);
	int32_t GetTouching();
	int32_t GetPalmPosition(const Vector&palmNormal);
	void AssignAnatomy(const bool isLeft);
private:
	// following vectors are all positions of fingertips/joints
	Vector tipThumbL, tipIndexL, tipMiddleL, tipRingL, tipPinkyL;
	Vector tipThumbR, tipIndexR, tipMiddleR, tipRingR, tipPinkyR;

	// dip stands for distal interphalangeal (first joint after fingertip)
	Vector dipIndexL, dipMiddleL, dipRingL, dipPinkyL;
	Vector dipIndexR, dipMiddleR, dipRingR, dipPinkyR;

	// pip stands for proximal interphalangeal (second joint after fingertip)
	Vector pipIndexL, pipMiddleL, pipRingL, pipPinkyL;
	Vector pipIndexR, pipMiddleR, pipRingR, pipPinkyR;

	// sensitivity radius for touching position
	const float FINGER_SPHERE_RADIUS = 50.0; // in mm
};

#endif // __Heph_Hand_h__