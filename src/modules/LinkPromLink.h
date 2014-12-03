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


class LinkPromLink : public Link, IPropertiesListener {
public:
	PromLink* link;
public:
	LinkPromLink(PromLink* link) {
		this->link = link;
		properties.add("comments", &link->comments, "multiline");
		properties.set("computation_mode", &link->computation_mode);
		properties.set("dst", &link->dst->no_name);
		properties.set("mem_time_in", &link->mem_time_in);
		properties.set("mem_time_out", &link->mem_time_out);
		properties.set("name", &link->name);
		properties.set("nb", &link->nb);
		properties.set("norm", &link->norm);
		properties.set("secondary", &link->secondary);
		properties.set("src", &link->src->no_name);
		properties.set("type", &link->type);

		add_properties_listener(this);

		text = link->name;
		realize();
	}

	virtual ~LinkPromLink();

	virtual void dump(std::ostream& os) {
		os << "LPL(" << link << ")";
	}

	virtual void on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val) {
		if(name=="name") { text = val; }
	}


	virtual void detach(bool bSlave = false);
	virtual void attach();

	void scale(float amount);

private:
	void realize();
	friend std::ostream& operator<<(std::ostream& os, LinkPromLink* a);
};

std::ostream& operator<<(std::ostream& os, LinkPromLink* a);




#endif /* LINKPROMLINK_H_ */
