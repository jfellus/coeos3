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
		properties.add_ref("comments", link->comments, "multiline");
		properties.add_ref("computation_mode", link->computation_mode);
		properties.add_ref("dst", link->dst->no_name);
		properties.add_ref("mem_time_in", link->mem_time_in);
		properties.add_ref("mem_time_out", link->mem_time_out);
		properties.add_ref("name", link->name);
		properties.add_ref("nb", link->nb);
		properties.add_ref("norm", link->norm);
		properties.add_ref("secondary", link->secondary);
		properties.add_ref("src", link->src->no_name);
		properties.add_ref("type", link->type);

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


private:
	void realize();
	friend std::ostream& operator<<(std::ostream& os, LinkPromLink* a);
};

std::ostream& operator<<(std::ostream& os, LinkPromLink* a);




#endif /* LINKPROMLINK_H_ */
