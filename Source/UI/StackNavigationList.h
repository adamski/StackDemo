/*
  ==============================================================================

    StackNavigationList.h
    Created: 13 Mar 2011 12:37:14pm
    Author:  Haydxn

  ==============================================================================
*/

#ifndef __STACKNAVIGATIONLIST_H_D59A3816__
#define __STACKNAVIGATIONLIST_H_D59A3816__

#include "StackComponent.h"

class StackNavigationList	:	public StackComponent::Controller,
								public ListBoxModel
{
public:

	StackNavigationList ();
	~StackNavigationList ();

	int getNumRows ();
	void paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected);
	void selectedRowsChanged (int lastRowSelected);

	void stackComponentContentChanged (StackComponent* stack);
	void stackComponentFocusChanged (StackComponent* stack);

	static void setContentTitle (Component* content, const String& title);
	static String getContentTitle (Component* content);

	static const Identifier titlePropertyId;

private:

	void selectRowForStackFocus ();

	ListBox listbox;

};



#endif  // __STACKNAVIGATIONLIST_H_D59A3816__
