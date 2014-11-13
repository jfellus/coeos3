/*
 * CreateForm.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef CREATEFORM_H_
#define CREATEFORM_H_

#include <widget/Browser.h>

class CreateForm : public Browser {
public:
	CreateForm() : Browser("create") {
		open("src/js/create_form.html");
	}
	virtual ~CreateForm() {}


	virtual std::string answer(const std::string& request, const std::string& data);


	void update() {
		if(is_loaded())	{
			script("update();");
		}
	}

	virtual void on_load() {update();}
};

#endif /* CREATEFORM_H_ */
