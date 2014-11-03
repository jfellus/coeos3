/*
 * IStyle.cpp
 *
 *  Created on: 31 oct. 2014
 *      Author: jfellus
 */



#include "IStyle.h"

void IStyle::update(const std::string& css_class) {
	default_style();

	// Element styling
	CSSDefinitions::apply_style(this);

	// Class styling
	FOR_EACH_TOKEN(css_class, ' ', cls) {
		if(cls.empty()) continue;
		CSSDefinitions::apply_style(this, cls);
	}
}
