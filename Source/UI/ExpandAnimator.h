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
#include "StackComponent.h"

class ExpandAnimator : public StackAnimator, public ChangeListener 
{
public:

    ExpandAnimator (Rectangle<int> focusArea)
        :   StackAnimator(),
            focusArea (focusArea),
            slideDuration (150),
            startSpeed (1.0),
            endSpeed (1.0)
    {
        Desktop::getInstance().getAnimator().addChangeListener (this);
    }

    ExpandAnimator (Rectangle<int> focusArea, int slideDuration, float startSpeed, float endSpeed)
        :   StackAnimator(),
            focusArea (focusArea),
            slideDuration (slideDuration),
            startSpeed (startSpeed),
            endSpeed (endSpeed)
    {
        Desktop::getInstance().getAnimator().addChangeListener (this);
    }

    void refreshLayout ()
    {
        for (int i = 0; i < stackComponent->getStackSize (); i++)
        {
            Component* panel = stackComponent->getContentComponentAtIndex (i);
            if (i == stackComponent->getStackFocusIndex())
            {
                DBG (">>>>> refreshLayout setVisible");
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
        
    }

    /**
     * save the image of the deleted component and display that
     * so that the animation can play on top of it
     */
    void animateContentComponentRemoved (Component* contentRemoved, int index)
    {
        Rectangle<int> bounds = getCurrentBounds(); 
        // save deleted content image 
        deletedPanel = new ImageComponent("Deleted Panel Snapshot");
        Image contentRemovedImage = contentRemoved->createComponentSnapshot (contentRemoved->getLocalBounds(), false);

        deletedPanel->setImage (contentRemovedImage);

        stackComponent->addAndMakeVisible (deletedPanel);
        deletedPanel->setBounds (bounds);

        // put deletedPanel in front of the previous panel 
        deletedPanel->toBack();
        if (previousPanel != nullptr && previousPanel->isShowing())
            previousPanel->toBehind (deletedPanel);
    }
    
    /** 
     * Get current bounds for the StackComponent
     */
    Rectangle<int> getCurrentBounds() const
    {
        int w = stackComponent->getWidth();
        int h = stackComponent->getHeight();
        return Rectangle<int> (0, 0, w, h);
    }

    void animateStackFocusChange (Component* newFocusContent, int newIndex, int oldIndex)
    {
        jassert (stackComponent != nullptr);
        Component* panel;

        Rectangle<int> bounds = getCurrentBounds();

        bool expanding = newIndex > oldIndex;

        // Split the old panel in two (one above, one below the focusArea)
        // Take a snapshot of each 
        // Put the resulting Images into an ImageComponent
        // Animate the ImageComponents to move up and down
        // ?? Fade out the focus/row component
        panel = stackComponent->getContentComponentAtIndex (expanding ? oldIndex : newIndex);
        if (panel != 0)
        {
            // TODO: Tidy this code up - work out whats going with the focusArea adjustments 

            Rectangle<int> topBounds (0, 0, bounds.getWidth(), focusArea.getY()+focusArea.getHeight());
            topSnapshot = new ImageComponent("Top Snapshot");
            topSnapshot->setImage (panel->createComponentSnapshot (topBounds));

            int bottomY = focusArea.getY()+(focusArea.getHeight()*2); 
            Rectangle<int> bottomBounds (0, bottomY, bounds.getWidth(), bounds.getHeight() );
            bottomSnapshot = new ImageComponent("Bottom Snapshot");
            bottomSnapshot->setImage (panel->createComponentSnapshot (bottomBounds));

            focusSnapshot = new ImageComponent("Focus Snapshot");
            DBG (focusArea.toString());
            Rectangle<int> actualFocusArea (0, focusArea.getY()+focusArea.getHeight(), bounds.getWidth(), focusArea.getHeight());
            focusSnapshot->setImage (panel->createComponentSnapshot (actualFocusArea));

            stackComponent->addAndMakeVisible (topSnapshot);
            if (! expanding) topBounds.setY (0-topBounds.getHeight());
            topSnapshot->setBounds (topBounds);

            stackComponent->addAndMakeVisible (bottomSnapshot);
            bottomBounds.setTop (focusArea.getHeight());
            if (! expanding) bottomBounds.setY (bottomBounds.getHeight());
            bottomSnapshot->setBounds (bottomBounds);

            stackComponent->addChildComponent (focusSnapshot);
            focusSnapshot->setBounds (actualFocusArea);
            if (expanding)
            {
                focusSnapshot->setVisible (true);
                Desktop::getInstance().getAnimator().fadeOut (focusSnapshot, slideDuration);
            }
            else
            {
                Desktop::getInstance().getAnimator().fadeIn (focusSnapshot, slideDuration);
            }

            if (expanding) topBounds.setY (0-topBounds.getHeight());
            else topBounds.setY (0);
            Desktop::getInstance().getAnimator().animateComponent(topSnapshot, topBounds, 1.0f, slideDuration, true, startSpeed, endSpeed);

            if (expanding) bottomBounds.setY (bottomBounds.getHeight());
            else bottomBounds.setY (focusArea.getY()-focusArea.getHeight());
            Desktop::getInstance().getAnimator().animateComponent(bottomSnapshot, bottomBounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
        }

        if (expanding)
        {
            panel = stackComponent->getContentComponentAtIndex (newIndex);
            if (panel != 0)
            {
                panel->setVisible (true);
                panel->setBounds (bounds);
                // panel->getProperties().set (StackComponentHelpers::deletionFlagId, false);
                previousPanel = nullptr;
            }
        }
        else
        {
            DBG ("---- setting previousPanel");
            previousPanel = stackComponent->getContentComponentAtIndex (newIndex);
            if (previousPanel.getComponent() != nullptr)
            {
                previousPanel->setVisible (false);
            }
 
            panel = stackComponent->getContentComponentAtIndex (oldIndex);
            if (panel != nullptr)
            {
                panel->setVisible (true); 
            }
        }
    }

    void changeListenerCallback (ChangeBroadcaster *source) 
    {
        if (! Desktop::getInstance().getAnimator().isAnimating (topSnapshot) && topSnapshot->getY() == 0)
            finishedAnimating = true;
        
        if (finishedAnimating)
        {
            DBG ("***** finishedAnimating");
            if (! previousPanel.getComponent()) DBG ("previousPanel null");
        }
        
        if (previousPanel.getComponent() != nullptr && finishedAnimating)
        {
            if (deletedPanel != nullptr && deletedPanel->isVisible())
                deletedPanel->setVisible (false);
            if (previousPanel != nullptr && !previousPanel->isShowing())
            {
                previousPanel->setVisible (true);
                int currentIndex = stackComponent->indexOfContentComponent(previousPanel.getComponent());
                DBG ("Index: " << currentIndex << ", size: " << stackComponent->getStackSize());
                
                if (currentIndex < stackComponent->getStackSize()-1)
                {
                    // hide next component if were going backwards and not deleted the last component 
                    stackComponent->getContentComponentAtIndex (currentIndex + 1)->setVisible (false);
                }
            }
            topSnapshot->setVisible (false);
            bottomSnapshot->setVisible (false);
            focusSnapshot->setVisible (false);

            DBG ("removed snapshots");
            finishedAnimating = false;
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
    bool finishedAnimating = false;

    ScopedPointer<ImageComponent> topSnapshot;
    ScopedPointer<ImageComponent> bottomSnapshot;
    ScopedPointer<ImageComponent> focusSnapshot;
    ScopedPointer<ImageComponent> deletedPanel;
    Component::SafePointer<Component> previousPanel;

    Rectangle<int> focusArea;

};



#endif  // EXPANDANIMATOR_H_INCLUDED
