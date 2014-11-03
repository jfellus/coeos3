/*
 * PropertiesForm.h
 *
 *  Created on: 9 oct. 2014
 *      Author: jfellus
 */

#ifndef PROPERTIESFORM_H_
#define PROPERTIESFORM_H_

#include "Browser.h"
#include "../module/Property.h"
#include "../module/Module.h"
#include "../promethe/promethe_modules.h"

class PropertiesForm : public Browser {
public:
	Properties* properties = 0;
	Properties* multiproperties = 0;
	std::vector<Module*>* selection = 0;
public:
	PropertiesForm(Properties* properties = NULL) : Browser("properties") {
		this->properties = properties;
		open("src/js/properties_form.html");
	}
	virtual ~PropertiesForm() {

	}

	void update(std::vector<Module*>* selection) {
		this->selection = selection;
		if(multiproperties) {delete multiproperties; multiproperties = 0;}
		if(selection->empty()) {
			properties = multiproperties = 0;
			update();
		} else if(selection->size()==1) {
			multiproperties = 0;
			properties = &(selection->at(0))->properties;
		} else {
			properties = 0;
			multiproperties = create_multiproperties(selection);
		}
		update();
	}

	Properties* create_multiproperties(std::vector<Module*>* selection) {
		Properties* p = new Properties();
		int nbSelected = 0;
		p->add("__NBSELECTED__", nbSelected);
		for(uint i=0; i<selection->size(); i++) {
			ModulePromGroup* g = dynamic_cast<ModulePromGroup*>(selection->at(i));
			if(!g) continue;
			nbSelected++;
			p->set("NBSELECTED", nbSelected);
			for(uint i=0; i<g->properties.size(); i++) {
				Property* pp = g->properties[i];
				Property* mypp = p->get(pp->name);
				if(mypp==NULL) p->add(pp->copy());
				else if(mypp->get_value_as_string() != pp->get_value_as_string()) mypp->set_undefined();
			}
		}
		return p;
	}


	virtual std::string answer(const std::string& request, const std::string& data) {
		if(str_starts_with(request, "set/")) {
			std::string field = str_between(request, "set/", "=");
			for(uint i=0; i<selection->size(); i++) {
				Module* m = selection->at(i);
				m->set_property(field, data);
			}
			return "ok";
		} else {
			if(multiproperties) return multiproperties->to_JSON();
			else if(!properties) return "";
			else return properties->to_JSON();
		}
	}


	void update() {
		if(is_loaded())	script("update();");
	}
};

#endif /* PROPERTIESFORM_H_ */
