/*
 * Tizen Fundamental Classes - TFC
 * Copyright (c) 2016-2017 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *    Components/Pager.h
 *
 * Components to provide pagination in layout
 *
 * Created on:  Mar 29, 2016
 * Author:      Gilang Mentari Hamidy (g.hamidy@samsung.com)
 * Contributor: Kevin Winata (k.winata@samsung.com)
 */

#ifndef TFC_COMP_PAGER_H_
#define TFC_COMP_PAGER_H_

#include "TFC/Components/ComponentBase.h"

namespace TFC {
namespace Components {
/**
 * Component that provides pagination UI.
 * It consists of a page indicator and two buttons, next and previous, to navigate through pages.
 * It also provides an event that will indicate navigation, and
 * can be used to attach callback that implements the actual content changes.
 */
class LIBAPI Pager:
		public ComponentBase,
		public EFL::EFLProxyClass,
		EventEmitterClass<Pager>,
		PropertyClass<Pager>
{
	using PropertyClass<Pager>::Property;
	using EventEmitterClass<Pager>::Event;

public:
	/**
	 * Constructor for Pager component.
	 */
	Pager();

	/**
	 * Event that will be triggered when next or previous button is clicked.
	 * It will contain an integer that indicates which page that has to be displayed.
	 */
	Event<int> eventNavigate;

	/**
	 * Event that will be triggered when user click previous button while on the first page.
	 * This will only happen when UnderflowEnable is true.
	 */
	Event<void*> eventUnderflow;

	/**
	 * Event that will be triggered when user click next button while on the last page.
	 * This will only happen when OverflowEnable is true.
	 */
	Event<void*> eventOverflow;
protected:
	/**
	 * Method overriden from ComponentBase, creates the UI elements of the component.
	 *
	 * @param root The root/parent given for this component.
	 *
	 * @return A box (Elm_Box) that contains both the content and the pagination UI.
	 */
	virtual Evas_Object* CreateComponent(Evas_Object* root) override;
private:
	std::string buttonPrevStyle;
	std::string buttonNextStyle;

	std::string underflowString;
	std::string overflowString;

	int currentIndex;
	int maxIndex;

	Evas_Object* pagerBox;
	Evas_Object* pagerTop;
	Evas_Object* pagerBottom;
	Evas_Object* pagerContent;

	EdjeSignalEvent eventClickSignal;

	bool underflowEnable;
	bool overflowEnable;

	void OnClickSignal(Evas_Object* source, EFL::EdjeSignalInfo signalInfo);
	void NextPage();
	void PrevPage();

	Evas_Object* GetPagerContent();
	int GetCurrentIndex() const;
	int GetMaxIndex() const;
	bool GetUnderflowEnable() const;
	bool GetOverflowEnable() const;
	std::string const& GetUnderflowString() const;
	std::string const& GetOverflowString() const;

	void SetPagerContent(Evas_Object* content);
	void SetMaxIndex(int const& maxIndex);
	void SetUnderflowEnable(bool const& underflow);
	void SetOverflowEnable(bool const& overflow);
	void SetUnderflowString(std::string const& str);
	void SetOverflowString(std::string const& str);
public:
	/**
	 * Contents that want to be paginated.
	 *
	 * @note The contents will not change itself when user navigates the pages.
	 * 		 You will have to implement how the content between pages change on Navigate event
	 * 		 by changing what's inside the Evas_Object that PagerContent points to.
	 */
	Property<Evas_Object*>::Get<&Pager::GetPagerContent>::Set<&Pager::SetPagerContent> PagerContent;

	/**
	 * Current page.
	 */
	Property<int>::Get<&Pager::GetCurrentIndex> CurrentIndex;

	/**
	 * Number of pages.
	 */
	Property<int>::Get<&Pager::GetMaxIndex>::Set<&Pager::SetMaxIndex> MaxIndex;

	/**
	 * True if you want to display some content before the first page.
	 * False if you don't want to display anything and simply disables previous navigation in the first page.
	 */
	Property<bool>::Get<&Pager::GetUnderflowEnable>::Set<&Pager::SetUnderflowEnable> UnderflowEnable;

	/**
	 * True if you want to display some content after the last page.
	 * False if you don't want to display anything and simply disables next navigation in the last page.
	 */
	Property<bool>::Get<&Pager::GetOverflowEnable>::Set<&Pager::SetOverflowEnable> OverflowEnable;

	/**
	 * Text that will be displayed in-between buttons before the first page.
	 * Replaces page indicator.
	 */
	Property<std::string const&>::Get<&Pager::GetUnderflowString>::Set<&Pager::SetUnderflowString> UnderflowString;

	/**
	 * Text that will be displayed in-between buttons after the last page.
	 * Replaces page indicator.
	 */
	Property<std::string const&>::Get<&Pager::GetOverflowString>::Set<&Pager::SetOverflowString> OverflowString;
};
}
}



#endif /* PAGER_H_ */
