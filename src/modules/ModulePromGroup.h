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
		properties.add("no_name", group->no_name);
		properties.add("alpha", group->alpha);
		properties.add("debug", group->debug);
		properties.add("nb neurons", group->nb_neurons);
		properties.add("width", group->width);
		properties.add("height", group->height);
		properties.add("group", group->group, "group");
		properties.add("learning rate", group->learning_rate);
		properties.add("p_posx", group->p_posx);
		properties.add("p_posy", group->p_posy);
		properties.add("threshold", group->threshold);
		properties.add("type2", group->type2);
		properties.add("simulation speed", group->simulation_speed);

		text = group->get_text();

		add_properties_listener(this);

		realize();
	}

	virtual ~ModulePromGroup();

	virtual void on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val) {
		if(name=="group") { text = val; }
	}

	virtual void dump(std::ostream& os) {
		os << "MPG(" << group << ")";
	}

private:
	void realize();
	friend std::ostream& operator<<(std::ostream& os, ModulePromGroup* a);
};

std::ostream& operator<<(std::ostream& os, ModulePromGroup* a);



#endif /* MODULEPROMGROUP_H_ */
