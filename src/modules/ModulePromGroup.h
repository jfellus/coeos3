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


using namespace libboiboites;
namespace coeos {


class GroupPromScript;


class ModulePromGroup : public Module, IPropertiesListener, Component::ITranslateListener {
public:
	PromGroup* group;
	std::string type;
	std::string name;
public:
	ModulePromGroup(PromGroup* group) {
		this->group = group;
		text = type = group->get_type();
		text2 = name = group->get_name();

//		properties.set("no_name", &group->no_name);
		properties.add("type", &type, "group");
		properties.add("name", &name);

		properties.set("nb neurons", &group->nb_neurons);
		properties.set("width", &group->width);
		properties.set("height", &group->height);

		properties.add("comments", &group->comments, "multiline");

		properties.set("threshold", &group->threshold);
		properties.set("simulation speed", &group->simulation_speed);
		properties.set("posx", &group->posx);
		properties.set("posy", &group->posy);
		properties.set("p_posx", &group->p_posx);
		properties.set("p_posy", &group->p_posy);
		properties.set("alpha", &group->alpha);
		properties.set("debug", &group->debug);
		properties.set("learning rate", &group->learning_rate);
		properties.set("type2", &group->type2);
		properties.set("number of 1", &group->nbre_de_1);

		add_properties_listener(this);

		realize();
	}

	virtual ~ModulePromGroup();

	virtual void on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val);

	virtual void dump(std::ostream& os) {
		os << "MPG(" << group << ")";
	}

	virtual Module* copy();
	virtual void detach(bool bSlave = false);
	virtual void attach();

	GroupPromScript* get_script();

	virtual void on_translate(double x, double y);

	virtual void on_parent_change(Group* newparent);

	void scale(double amount);

	virtual void create_component(const char* component_spec);
private:
	void realize();
	void update_component();
	friend std::ostream& operator<<(std::ostream& os, ModulePromGroup* a);

	void process_cross_script_links();
};

std::ostream& operator<<(std::ostream& os, ModulePromGroup* a);

}


#endif /* MODULEPROMGROUP_H_ */
