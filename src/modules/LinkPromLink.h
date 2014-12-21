/*
 * LinkPromLink.h
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#ifndef LINKPROMLINK_H_
#define LINKPROMLINK_H_


#include <module/Link.h>
#include "../promethe/promscript/PromLink.h"


using namespace libboiboites;
namespace coeos {


class ModulePromGroup;


class LinkPromLink : public Link, IPropertiesListener {
public:
	PromLink* link;
public:
	LinkPromLink(PromLink* link) {
		this->link = link;
		properties.add("type", LINK_NO_TO_TYPE(link->type), "link_type");
		properties.set("name", &link->name);
		properties.set("src", &link->src->no_name);
		properties.set("dst", &link->dst->no_name);
		properties.add("comments", &link->comments, "multiline");
		properties.set("secondary", &link->secondary);
		properties.set("norm", &link->norm);
		properties.set("nb", &link->nb);
		properties.set("computation_mode", &link->computation_mode);
		properties.set("mem_time_in", &link->mem_time_in);
		properties.set("mem_time_out", &link->mem_time_out);

		add_properties_listener(this);

		text = link->name;
		realize();
	}

	virtual ~LinkPromLink();

	virtual void connect(ModulePromGroup* src, ModulePromGroup* dst);
	virtual void connect(Module* src, Module* dst);

	virtual void dump(std::ostream& os) {
		os << "LPL(" << link << ")";
	}

	virtual void on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val) {
		if(name=="name") { text = val; }
		else if(name=="type") {link->type = LINK_TYPE_TO_NO(val);}

		if(link->is_type_algo())
			add_class("algo"); else remove_class("algo");
		if(link->type == No_l_1_a || link->type == No_l_1_t)
			add_class("one_to_all"); else remove_class("one_to_all");
		if(link->type == No_l_1_1_modif || link->type == No_l_1_1_non_modif)
			add_class("one_to_one"); else remove_class("one_to_one");
		if(link->type == No_l_neuro_mod)
			add_class("neuromod"); else remove_class("neuromod");

		if(link->secondary > 0)
			add_class("secondary"); else remove_class("secondary");
	}


	virtual void on_change();

	virtual Link* copy();
	virtual void detach(bool bSlave = false);
	virtual void attach();

	void scale(float amount);

private:
	void realize();
	friend std::ostream& operator<<(std::ostream& os, LinkPromLink* a);
};

std::ostream& operator<<(std::ostream& os, LinkPromLink* a);


}



#endif /* LINKPROMLINK_H_ */

