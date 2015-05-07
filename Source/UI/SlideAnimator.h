/*
  ==============================================================================

    SlideAnimator.h
    Created: 4 May 2015 10:40:25pm
    Author:  Adam Elemental

  ==============================================================================
*/

#ifndef SLIDEANIMATOR_H_INCLUDED
#define SLIDEANIMATOR_H_INCLUDED


#include "../../JuceLibraryCode/JuceHeader.h"

class SlideAnimator : public StackAnimator 
{
public:

    SlideAnimator ()
        :   StackAnimator(),
            slideDuration (150),
            startSpeed (1.0),
            endSpeed (1.0)
    {}

    SlideAnimator (int slideDuration, float startSpeed, float endSpeed)
        :   StackAnimator(),
            slideDuration (slideDuration),
            startSpeed (startSpeed),
            endSpeed (endSpeed)
    {}

    void refreshLayout ()
    {
        jassert (stackComponent != nullptr);

        for (int i = 0; i < stackComponent->getStackSize (); i++)
        {
            Component* panel = stackComponent->getContentComponentAtIndex (i);
            if (i == stackComponent->getStackFocusIndex())
            {
                panel->setVisible (true);
                panel->setBounds (0, 0, stackComponent->getWidth(), stackComponent->getHeight());
            }
            else
            {
                panel->setVisible (false);
            }
        }
    }

    void animateContentComponentAdded (Component *newContent, int index)
    {
        newContent->setBounds (getBoundsForContent(index));
    }

    void animateContentComponentRemoved (Component* contentRemoved, int index)
    {
        Desktop::getInstance().getAnimator().fadeOut (contentRemoved, 200);
    }

    void animateStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex)
    {
        jassert (stackComponent != nullptr);
        Component* panel;

        int w = stackComponent->getWidth ();
        int h = stackComponent->getHeight ();

        Rectangle<int> bounds (0 ,0, w, h);

        // Move the old panel...
        panel = stackComponent->getContentComponentAtIndex (oldIndex);
        if (panel != 0)
        {
            panel->setVisible (true);

            // Make sure the panel is currently on-screen to exit...
            bounds.setX (0);
            panel->setBounds (bounds);

            // Animate it to exit
            bounds.setX ((oldIndex < newIndex) ? -w : w);
            Desktop::getInstance().getAnimator().animateComponent(panel, bounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
        }

        // Move the new panel...
        panel = stackComponent->getContentComponentAtIndex (newIndex);
        if (panel != 0)
        {
            panel->setVisible (true);

            // Place the panel in a suitable position for entry...
            bounds.setX ((newIndex < oldIndex) ? -w : w);
            panel->setBounds (bounds);

            // Animate it to enter and fill the bounds.
            bounds.setX (0);
            Desktop::getInstance().getAnimator().animateComponent(panel, bounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
        }
    }

    void setSlideDuration (int durationMs, double newStartSpeed, double newEndSpeed)
    {
        slideDuration = durationMs;
        startSpeed = newStartSpeed;
        endSpeed = newEndSpeed;
    }

private:

    Rectangle<int> getBoundsForContent (int index)
    {
        jassert (stackComponent != nullptr);

        int w = stackComponent->getWidth ();
        int h = stackComponent->getHeight ();
        int x = (index - stackComponent->getStackFocusIndex()) * w;
        Rectangle<int> rect (x,0,w,h);
        return rect;
    }

    int slideDuration;
    double startSpeed;
    double endSpeed;


};



#endif  // SLIDEANIMATOR_H_INCLUDED
