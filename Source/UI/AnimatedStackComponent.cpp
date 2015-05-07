/*
    ==============================================================================

    AnimatedStackComponent.cpp
    Created: 12 Mar 2011 12:13:09pm
    Author:  Haydxn

    ==============================================================================
*/

#include "AnimatedStackComponent.h"



AnimatedStackComponent::AnimatedStackComponent (const StackAnimator::Ptr & defaultStackAnimator) 
    :   StackComponent(), 
        stackAnimator (defaultStackAnimator)
{
    stackAnimator->setStackComponent (this);
}

AnimatedStackComponent::~AnimatedStackComponent ()
{
}

void AnimatedStackComponent::setDefaultStackAnimator(StackAnimator* defaultStackAnimator)
{
    stackAnimator = defaultStackAnimator;
}

void AnimatedStackComponent::refreshLayout ()
{
    
    for (int i = 0; i < getStackSize (); ++i)
    {
        Component* panel = getContentComponentAtIndex (i);
        if (i == getStackFocusIndex())
        {
            panel->setVisible (true);
            panel->setBounds (0, 0, getWidth(), getHeight());
        }
        else
        {
            panel->setVisible (false);
        }
    }
}


void AnimatedStackComponent::handleContentComponentAdded (Component* newContent, int index, bool animate)
{
    addAndMakeVisible (newContent);

    if (animate) 
    {
        stackAnimator->animateContentComponentAdded (newContent, index);
    }

}

void AnimatedStackComponent::handleContentComponentRemoved (Component* contentRemoved, int index, bool animate)
{
    if (animate)
    {
        getStackAnimatorForComponent (contentRemoved)->animateContentComponentRemoved (contentRemoved, index);
    }
    
    removeChildComponent (contentRemoved);
    if (shouldContentComponentBeDeleted(contentRemoved))
        delete contentRemoved;
}

void AnimatedStackComponent::handleStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex, bool animate)
{
    if (!animate)
    {
        getStackAnimatorForComponent (newFocusContent)->refreshLayout();
    }
    else 
    {
        getStackAnimatorForComponent (newFocusContent)->animateStackFocusChange (newFocusContent, newIndex, oldIndex);
    }
}

StackAnimator::Ptr AnimatedStackComponent::getStackAnimatorForComponent (Component *component) 
{
    NamedValueSet & properties = component->getProperties();
    if (properties.contains (AnimatedStackHelpers::stackAnimatorId)) // also check for valid stackAnimator object
    {
        var *animator = properties.getVarPointer (AnimatedStackHelpers::stackAnimatorId); 
        StackAnimator::Ptr stackAnimator = dynamic_cast <StackAnimator *> (animator->getObject());
        return stackAnimator;
    }
    else
    {
        return getStackAnimator();  
    }
}

void AnimatedStackComponent::setStackAnimatorForComponent (StackAnimator::Ptr stackAnimator, Component *component) 
{
   component->getProperties().set (AnimatedStackHelpers::stackAnimatorId, var(stackAnimator));
}

StackAnimator::Ptr AnimatedStackComponent::getStackAnimator() 
{
    return stackAnimator;
}

