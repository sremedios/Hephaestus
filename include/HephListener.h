#if !defined(__Heph_Listener_h__)
#define __Heph_Listener_h__

#include "Leap.h"
#include <string.h>
using namespace Leap;

class HephListener : public Listener {
public:
	virtual void onConnect(const Controller&);
	virtual void onFrame(const Controller&);
	bool IsInSphere(const Vector&, const bool);
	int32_t GetTouching();
	int32_t GetPalmPosition(const Vector&);
	void AssignAnatomy(const Hand&,const bool,const int);
	void HephType(int32_t touchingCode);
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

	// used to iterate through fingers
	int32_t finger_IDs[5];

	// sensitivity radius for touching position
	const float FINGER_SPHERE_RADIUS = 20.0; // in mm
	const float SLEEP_AMOUNT = 100.0; // in ms
};

#endif // __HephListener_h__
