/*
 * ModulePromGroup.h
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#ifndef MODULEPROMGROUP_H_
#define MODULEPROMGROUP_H_


#include <module/Module.h>
#include "../promethe/promscript/PromGroup.h"

class ModulePromGroup : public Module, IPropertiesListener {
public:
	PromGroup* group;
public:
	ModulePromGroup(PromGroup* group) {
		this->group = group;
		properties.add("comments", group->comments, "multiline");
		properties.set("no_name", &group->no_name);
		properties.set("alpha", &group->alpha);
		properties.set("debug", &group->debug);
		properties.set("nb neurons", &group->nb_neurons);
		properties.set("width", &group->width);
		properties.set("height", &group->height);
		properties.add("group", &group->group, "group");
		properties.set("learning rate", &group->learning_rate);
		properties.set("p_posx", &group->p_posx);
		properties.set("p_posy", &group->p_posy);
		properties.set("threshold", &group->threshold);
		properties.set("type2", &group->type2);
		properties.set("simulation speed", &group->simulation_speed);

		text = group->get_text();

		add_properties_listener(this);

		realize();
	}

	virtual ~ModulePromGroup();

	virtual void on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val) {
		if(name=="group") {text = val;}
	}

	virtual void dump(std::ostream& os) {
		os << "MPG(" << group << ")";
	}

	virtual void detach(bool bSlave = false);
	virtual void attach();


private:
	void realize();
	friend std::ostream& operator<<(std::ostream& os, ModulePromGroup* a);
};

std::ostream& operator<<(std::ostream& os, ModulePromGroup* a);



#endif /* MODULEPROMGROUP_H_ */
