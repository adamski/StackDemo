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
        //Desktop::getInstance().getAnimator().fadeIn (newContent, 200);
        
    }

    void animateContentComponentRemoved (Component* contentRemoved, int index)
    {
        // Desktop::getInstance().getAnimator().fadeOut (contentRemoved, 200);
        
        contentRemoved->setVisible (true);
        DBG ("!!!!! contentRemoved - saving image");
        
        Rectangle<int> bounds = getCurrentBounds(); 
        // save deleted content image 
        deletedPanel = new ImageComponent("Deleted Panel Snapshot");
        Image contentRemovedImage = contentRemoved->createComponentSnapshot (contentRemoved->getLocalBounds(), false);
        FileOutputStream fos (File("contentRemoved.png"));
        PNGImageFormat pngImageFormat();
        pngImageFormat.writeImageToStream (contentRemovedImage, fos);

        deletedPanel->setImage (contentRemovedImage);

        DBG ("^^^^^ Show deleted panel");
        deletedPanel->setVisible (true);
        deletedPanel->setBounds (bounds);
        // Desktop::getInstance().getAnimator().fadeOut (deletedPanel, slideDuration);
    }
    
    Rectangle<int> getCurrentBounds() const
    {
        int w = stackComponent->getWidth ();
        int h = stackComponent->getHeight ();
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
            //panel->setVisible (true);

            // Make sure the panel is currently on-screen to exit...
            // bounds.setX (0);
            // panel->setBounds (bounds);

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
            //actualFocusArea.setY (focusArea.getY()+focusArea.getHeight());
            focusSnapshot->setImage (panel->createComponentSnapshot (actualFocusArea));

            stackComponent->addAndMakeVisible (topSnapshot);
            if (! expanding) topBounds.setY (0-topBounds.getHeight());
            topSnapshot->setBounds (topBounds);

            stackComponent->addAndMakeVisible (bottomSnapshot);
            bottomBounds.setTop (focusArea.getHeight());
            if (! expanding) bottomBounds.setY (bottomBounds.getHeight());
            bottomSnapshot->setBounds (bottomBounds);

            stackComponent->addAndMakeVisible (focusSnapshot);
            focusSnapshot->setBounds (actualFocusArea);
            if (expanding)
                Desktop::getInstance().getAnimator().fadeOut (focusSnapshot, slideDuration);
            else
                Desktop::getInstance().getAnimator().fadeIn (focusSnapshot, slideDuration);

            if (expanding) topBounds.setY (0-topBounds.getHeight());
            else topBounds.setY (0);
            Desktop::getInstance().getAnimator().animateComponent(topSnapshot, topBounds, 1.0f, slideDuration, true, startSpeed, endSpeed);

            if (expanding) bottomBounds.setY (bottomBounds.getHeight());
            else bottomBounds.setY (focusArea.getY()-focusArea.getHeight());
            Desktop::getInstance().getAnimator().animateComponent(bottomSnapshot, bottomBounds, 1.0f, slideDuration, true, startSpeed, endSpeed);

            //if (! expanding)
                //panel->setVisible (false);
            //else
                //panel->setBounds (bounds);

            // // Animate it to exit
            // bounds.setX ((oldIndex < newIndex) ? -w : w);
            // Desktop::getInstance().getAnimator().animateComponent(panel, bounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
        }

        if (expanding)
        {
            panel = stackComponent->getContentComponentAtIndex (newIndex);
            if (panel != 0)
            {
                panel->setVisible (true);
                //Desktop::getInstance().getAnimator().fadeIn (panel, 200);

                // // Place the panel in a suitable position for entry...
                // bounds.setX ((newIndex < oldIndex) ? -w : w);
                panel->setBounds (bounds);

                // set so the component is not deleted, 
                // we will do this on the callback
                panel->getProperties().set (StackComponentHelpers::deletionFlagId, false);

                //
                // // Animate it to enter and fill the bounds.
                // bounds.setX (0);
                // Desktop::getInstance().getAnimator().animateComponent(panel, bounds, 1.0f, slideDuration, true, startSpeed, endSpeed);
            }
        }
        else
        {
            DBG ("---- setting previousPanel");
            previousPanel = stackComponent->getContentComponentAtIndex (newIndex);
            if (previousPanel.getComponent() != nullptr)
            {
                //DBG ("______ set new / previous visible false");
                //previousPanel->setVisible (true);
                //previousPanel->setBounds (bounds);
            }
 
            panel = stackComponent->getContentComponentAtIndex (oldIndex);
            if (panel != 0)
            {
                
                DBG ("+++++ set old visible true");
                //panel->setVisible (true); // <-- is this not working?
                //panel->setBounds (bounds);
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
//            int w = stackComponent->getWidth ();
//            int h = stackComponent->getHeight ();
//            Rectangle<int> bounds (0, 0, w, h);

            previousPanel->setVisible (true);
            //previousPanel->setBounds (bounds);
            //Desktop::getInstance().getAnimator().fadeIn (previousPanel, 200);
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
