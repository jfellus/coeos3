/*
 * ScriptsForm.h
 *
 *  Created on: 7 nov. 2014
 *      Author: jfellus
 */

#ifndef SCRIPTSFORM_H_
#define SCRIPTSFORM_H_

#include <widget/Browser.h>
#include <module/Property.h>
#include <module/Module.h>
#include <util/utils.h>


class ScriptsForm : public Browser {
public:

public:
	ScriptsForm() : Browser("scripts") {
		open("src/js/scripts_form.html");
	}
	virtual ~ScriptsForm() {

	}

	virtual std::string answer(const std::string& request, const std::string& data);


	void update() {
		if(is_loaded())	{
			script("update();");
		}
	}

	virtual void on_load() {update();}
};

#endif /* SCRIPTSFORM_H_ */
