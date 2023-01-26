#ifndef _DESCENDEROPTION_H
#define _DESCENDEROPTION_H

class DescenderOption
	: public MenuOption
{
	protected:
		Menu		*childMenu;
	public:
		DescenderOption(const char *_text, Menu *_childMenu = NULL, double _marginTop = 15.0, double _marginBottom = 15.0,
				ACTION_CALLBACK _callback = NULL)
			: MenuOption(_text, _marginTop, _marginBottom, _callback), childMenu(_childMenu) {};
		~DescenderOption() {delete childMenu;};
	
		void		setParentMenu(Menu *menu) {parentMenu = menu; childMenu->setParentMenu(parentMenu);};
		void		setChildMenu(Menu *_childMenu) {childMenu = _childMenu;};
		
		void		action(int keyScancode) {parentMenu->giveFocus(childMenu); MenuOption::action(keyScancode);};
};

#endif
