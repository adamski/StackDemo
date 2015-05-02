/*
  ==============================================================================

    SlidingStackComponent.h
    Created: 12 Mar 2011 12:13:09pm
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __SLIDINGSTACKCOMPONENT_H_3EB614FF__
#define __SLIDINGSTACKCOMPONENT_H_3EB614FF__

#include "StackComponent.h"

class SlidingStackComponent	:	public StackComponent
{
public:

	SlidingStackComponent ();
	virtual ~SlidingStackComponent ();

protected:

	void setSlideDuration (int durationMs, double startSpeed = 1.0, double endSpeed = 1.0);

	virtual void refreshLayout ();
	virtual void handleContentComponentAdded (Component* newContent, int index, bool animate);
	virtual void handleContentComponentRemoved (Component* contentRemoved, int index, bool animate);
	virtual void handleStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex, bool animate);

	Rectangle<int> getBoundsForContent (int index);

private:

	int slideDuration;
	double startSpeed;
	double endSpeed;

};



#endif  // __SLIDINGSTACKCOMPONENT_H_3EB614FF__
