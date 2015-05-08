/*
  ==============================================================================

    StackAnimator.h
    Created: 4 May 2015 8:52:18pm
    Author:  Adam Wilson

  ==============================================================================
*/

#ifndef STACKANIMATOR_H_INCLUDED
#define STACKANIMATOR_H_INCLUDED

#include "StackComponent.h"

class StackAnimator : public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<StackAnimator> Ptr;

    StackAnimator () {};
    //virtual ~StackAnimator() {};
    
    virtual void animateContentComponentAdded (Component* newContent, int index) = 0;
    virtual void animateContentComponentRemoved (Component* contentRemoved, int index) = 0;
    virtual void animateStackFocusChange(Component* newFocusContent, int newIndex, int oldIndex) = 0;
    virtual void refreshLayout() = 0;

    void setStackComponent(StackComponent* stack)
    {
        stackComponent = stack;
    }

protected:

    ScopedPointer<StackComponent> stackComponent;
};

#endif  // STACKANIMATOR_H_INCLUDED
