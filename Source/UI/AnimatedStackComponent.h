/*
  ==============================================================================

    AnimatedStackComponent.h
    Created: 12 Mar 2011 12:13:09pm
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __SLIDINGSTACKCOMPONENT_H_3EB614FF__
#define __SLIDINGSTACKCOMPONENT_H_3EB614FF__

#include "StackComponent.h"
#include "StackAnimator.h"

class AnimatedStackComponent	:	public StackComponent
{
public:

	AnimatedStackComponent (StackAnimator* defaultStackAnimator);
	~AnimatedStackComponent ();

        void setDefaultStackAnimator(StackAnimator* defaultStackAnimator);

protected:

	virtual void refreshLayout ();
	virtual void handleContentComponentAdded (Component* newContent, int index, bool animate);
	virtual void handleContentComponentRemoved (Component* contentRemoved, int index, bool animate);
	virtual void handleStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex, bool animate);

        StackAnimator::Ptr getStackAnimatorForComponent (Component* component);
        StackAnimator::Ptr getStackAnimator();


private:
        StackAnimator::Ptr stackAnimator;

};



#endif  // __SLIDINGSTACKCOMPONENT_H_3EB614FF__
