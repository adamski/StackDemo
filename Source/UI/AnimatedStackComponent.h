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

namespace AnimatedStackHelpers
{
    static const Identifier stackAnimatorId ("StackAnimator");

    static void setStackAnimatorForComponent (StackAnimator::Ptr stackAnimator, Component *component) 
    {
        stackAnimator->incReferenceCount(); // this prevents a crash when using 'Back' button on this component
        component->getProperties().set (AnimatedStackHelpers::stackAnimatorId, var(stackAnimator));
    }

};

class AnimatedStackComponent	:	public StackComponent
{
public:

	AnimatedStackComponent (const StackAnimator::Ptr & defaultStackAnimator);
	~AnimatedStackComponent ();

        void setDefaultStackAnimator(StackAnimator* defaultStackAnimator);

protected:

	virtual void handleContentComponentAdded (Component* newContent, int index, bool animate);
	virtual void handleContentComponentRemoved (Component* contentRemoved, int index, bool animate);
	virtual void handleStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex, bool animate);

        StackAnimator::Ptr getStackAnimatorForComponent (Component* component);
        void setStackAnimatorForComponent (StackAnimator::Ptr stackAnimator, Component *component);
        StackAnimator::Ptr getStackAnimator();


private:
        StackAnimator::Ptr stackAnimator;

};



#endif  // __SLIDINGSTACKCOMPONENT_H_3EB614FF__
