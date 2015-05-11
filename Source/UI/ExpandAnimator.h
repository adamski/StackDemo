/*
  ==============================================================================

    ExpandAnimator.h
    Created: 4 May 2015 10:40:25pm
    Author:  Adam Elemental

  ==============================================================================
*/

#ifndef EXPANDANIMATOR_H_INCLUDED
#define EXPANDANIMATOR_H_INCLUDED


#include "../../JuceLibraryCode/JuceHeader.h"

class ExpandAnimator : public StackAnimator 
{
public:

    ExpandAnimator (Rectangle<int> focusArea)
        :   StackAnimator(),
            focusArea (focusArea),
            slideDuration (150),
            startSpeed (1.0),
            endSpeed (1.0)
    {}

    ExpandAnimator (Rectangle<int> focusArea, int slideDuration, float startSpeed, float endSpeed)
        :   StackAnimator(),
            focusArea (focusArea),
            slideDuration (slideDuration),
            startSpeed (startSpeed),
            endSpeed (endSpeed)
    {}

    void refreshLayout ()
    {
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
        //Desktop::getInstance().getAnimator().fadeIn (newContent, 200);
    }

    void animateContentComponentRemoved (Component* contentRemoved, int index)
    {
        //Desktop::getInstance().getAnimator().fadeOut (contentRemoved, 200);
    }

    void animateStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex)
    {
        jassert (stackComponent != nullptr);
        Component* panel;

        int w = stackComponent->getWidth ();
        int h = stackComponent->getHeight ();

        Rectangle<int> bounds (0, 0, w, h);

        // Split the old panel in two (one above, one below the focusArea)
        // Take a snapshot of each 
        // Put the resulting Images into an ImageComponent
        // Animate the ImageComponents to move up and down
        // ?? Fade out the focus/row component
        panel = stackComponent->getContentComponentAtIndex (oldIndex);
        if (panel != 0)
        {
            // panel->setVisible (true);

            // Make sure the panel is currently on-screen to exit...
            // bounds.setX (0);
            // panel->setBounds (bounds);

            Rectangle<int> topBounds (0, 0, w, focusArea.getY()+focusArea.getHeight());
            topSnapshot = new ImageComponent("Top Snapshot");
            topSnapshot->setImage (panel->createComponentSnapshot (topBounds));

            Rectangle<int> bottomBounds (0, focusArea.getY()+(focusArea.getHeight()*2), w, h );
            bottomSnapshot = new ImageComponent("Bottom Snapshot");
            bottomSnapshot->setImage (panel->createComponentSnapshot (bottomBounds));

            focusSnapshot = new ImageComponent("Focus Snapshot");
            DBG (focusArea.toString());
            Rectangle<int> actualFocusArea (0, focusArea.getY()+focusArea.getHeight(), w, focusArea.getHeight());
            //actualFocusArea.setY (focusArea.getY()+focusArea.getHeight());
            focusSnapshot->setImage (panel->createComponentSnapshot (actualFocusArea));

            stackComponent->addAndMakeVisible (topSnapshot);
            topSnapshot->setBounds (topBounds);

            stackComponent->addAndMakeVisible (bottomSnapshot);
            bottomBounds.setTop (focusArea.getHeight());
            bottomSnapshot->setBounds (bottomBounds);

            stackComponent->addAndMakeVisible (focusSnapshot);
            focusSnapshot->setBounds (actualFocusArea);
            Desktop::getInstance().getAnimator().fadeOut (focusSnapshot, slideDuration);

            topBounds.setY (0-topBounds.getHeight());
            Desktop::getInstance().getAnimator().animateComponent(topSnapshot, topBounds, 1.0f, slideDuration, true, startSpeed, endSpeed);

            bottomBounds.setY (bottomBounds.getHeight());
            Desktop::getInstance().getAnimator().animateComponent(bottomSnapshot, bottomBounds, 1.0f, slideDuration, true, startSpeed, endSpeed);


            // // Animate it to exit
            // bounds.setX ((oldIndex < newIndex) ? -w : w);
            // Desktop::getInstance().getAnimator().animateComponent(panel, bounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
        }

        // Expand the new component into the space between 
        panel = stackComponent->getContentComponentAtIndex (newIndex);
        if (panel != 0)
        {
            panel->setVisible (true);
            //Desktop::getInstance().getAnimator().fadeIn (panel, 200);

            // // Place the panel in a suitable position for entry...
            // bounds.setX ((newIndex < oldIndex) ? -w : w);
            panel->setBounds (bounds);
            //
            // // Animate it to enter and fill the bounds.
            // bounds.setX (0);
            // Desktop::getInstance().getAnimator().animateComponent(panel, bounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
        }
    }

    void setExpandDuration (int durationMs, double newStartSpeed, double newEndSpeed)
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

    ScopedPointer<ImageComponent> topSnapshot;
    ScopedPointer<ImageComponent> bottomSnapshot;
    ScopedPointer<ImageComponent> focusSnapshot;

    Rectangle<int> focusArea;

};



#endif  // EXPANDANIMATOR_H_INCLUDED
