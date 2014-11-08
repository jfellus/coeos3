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
		properties.add("comments", link->comments);
		properties.add("computation_mode", link->computation_mode);
		properties.add("dst", link->dst->no_name);
		properties.add("mem_time_in", link->mem_time_in);
		properties.add("mem_time_out", link->mem_time_out);
		properties.add("name", link->name);
		properties.add("nb", link->nb);
		properties.add("norm", link->norm);
		properties.add("secondary", link->secondary);
		properties.add("src", link->src->no_name);
		properties.add("type", link->type);

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

private:
	void realize();
	friend std::ostream& operator<<(std::ostream& os, LinkPromLink* a);
};

std::ostream& operator<<(std::ostream& os, LinkPromLink* a);




#endif /* LINKPROMLINK_H_ */
