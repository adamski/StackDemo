/*
  ==============================================================================

    StackHeaderComponent.h
    Created: 12 Mar 2011 12:29:38pm
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __StackHeaderComponent_H_8C491CB2__
#define __StackHeaderComponent_H_8C491CB2__

#include "StackComponent.h"
#include "MarginedContainerComponent.h"
#include "HeaderLabel.h"

class StackHeaderComponent	:	public StackComponent::Controller,
									public Button::Listener
{
public:

	StackHeaderComponent ();
	virtual ~StackHeaderComponent ();

	void resized ();

	static void setStackContentTitle (Component* contentComponent, const String& contentTitle);
	static String getStackContentTitle (Component* contentComponent);

	void stackComponentContentChanged (StackComponent* stack);
	void stackComponentFocusChanged (StackComponent* stack);

	HeaderLabel& getTitleLabel ();

	void buttonClicked (Button* button);

	static const Identifier contentTitlePropertyId;
	static const char* titleLabelId;

	void setBackButtonPadding (const BorderSize<int>& padding);
	void setBackButtonWidth (int width);

	void refreshTitles ();

private:

	void setBackButtonVisibility (bool showBack);

	MarginedContainerComponent container;

	HeaderLabel titleLabel;
	TextButton backButton;

	BorderSize<int> backButtonPadding;
	int backButtonWidth;
	bool backButtonVisible;
};



#endif  // __StackHeaderComponent_H_8C491CB2__
