/*
 * ModulePromGroup.cpp
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#include "ModulePromGroup.h"
#include "../promethe/PromProject.h"

using namespace libboiboites;
namespace coeos {


class ComponentPromGroup : public ModuleComponent {
public:
	ComponentPromGroup(ModulePromGroup* mpg, const char* spec) : ModuleComponent(mpg, spec, mpg->text, mpg->text2) {

	}

	virtual ~ComponentPromGroup() {}

	virtual void render_infos(Graphics& g, bool hover) {
		ModuleComponentStyle* style = (ModuleComponentStyle*)this->style;

		double x = hover ? MAX(22,28/canvas->_zoom) : 22;
		double f = hover ? MAX(9,13/canvas->_zoom) : 9;
		g.set_font(f, style->font, style->font_style);

		render_noname(g, 0,0, hover);
		render_nbneurons(g, x, -5, hover);
	}

	void render_noname(Graphics& g, double x, double y, bool hover) {
		ModulePromGroup* mpg = (ModulePromGroup*)module;

		double r = hover ? MAX(10,15/canvas->_zoom) : 10;
		g.set_opacity(0.8);
		g.circle(x,y,r); g.fill(RGB(0,0,0.3));
		g.set_color(RGB_WHITE);
		g.text_center(mpg->group->no_name, Vector2D(x,y));
	}

	void render_nbneurons(Graphics& g, double x, double y, bool hover) {
		ModulePromGroup* mpg = (ModulePromGroup*)module;
		ModuleComponentStyle* style = (ModuleComponentStyle*)this->style;
		g.set_opacity(1);

		double w = hover ? MAX(20,30/canvas->_zoom) : 20;
		double h = hover ? MAX(20,30/canvas->_zoom) : 20;
		double arrowSize = hover ? MAX(0.5, 0.09375/canvas->_zoom) : 0.5;
		double r = hover ? MAX(1, 2/canvas->_zoom) : 2;
		double d = hover ? MAX(10, 10/canvas->_zoom) : 10;
		double stroke = hover ? MAX(1, 2/canvas->_zoom) : 1;
		double f = hover ? MAX(9,13/canvas->_zoom) : 9;

		g.set_font(f, style->font, 0x001);


		if(mpg->group->width!="1" && mpg->group->height!="1") {
			g.set_color(RGB(0,0.5,0));
			g.drawArrow(x-w*0.28,y+h*0.25,x+w*0.37,y+h*0.25, stroke * canvas->_zoom, arrowSize );
			g.drawArrow(x-w*0.25,y+h*0.28,x-w*0.25,y-h*0.37, stroke * canvas->_zoom, arrowSize );
			g.text_bottom_left(TOSTRING(mpg->group->width << " x " << mpg->group->height), Vector2D(x+d,y));
		} else if(mpg->group->width!="1") {
			g.set_color(RGB(0,0.5,0));
			g.drawArrow(x+w*0.2,y,x+w*0.3,y, stroke * canvas->_zoom, arrowSize );
			g.text_bottom_left(TOSTRING(mpg->group->width), Vector2D(x+d,y));
		} else if(mpg->group->height!="1") {
			g.set_color(RGB(0,0.5,0));
			g.drawArrow(x-w*0.25,y+h*0.2,x-w*0.25,y-h*0.37, stroke * canvas->_zoom, arrowSize );
			g.text_bottom_left(TOSTRING(mpg->group->height), Vector2D(x,y));
		} else {
			if(!hover) return;
			g.set_color(RGB(0,0.5,0));
			g.drawCircle(x-w*0.25,y, r);
			g.text_bottom_left(TOSTRING(mpg->group->width), Vector2D(x,y));
		}
	}
};








void ModulePromGroup::realize() {
	if(group->type==14) {
		if(!group->custom_function.empty()) {
			try { create_component(group->custom_function.c_str()); }
			catch(...) {
				try { create_component(group->group.c_str()); }
				catch(...) { create_component("module_algo"); }
			}
		} else {
			try { create_component(group->group.c_str()); }
			catch(...) { create_component("module_algo"); }
		}
	} else {
		try { create_component(group->get_type().c_str()); }
		catch(...) { create_component("module_neural"); }
	}

	component->set_pos(group->posx, group->posy);

	component->add_class(group->is_type_algo() ? "algo" : "neural");
	if(!group->custom_function.empty()) {
		component->add_class("custom_cpp");
		component->add_class(group->custom_function);
	}

	component->add_class(group->group);
}

void ModulePromGroup::create_component(const char* component_spec) {
	component = new ComponentPromGroup(this, component_spec);
	component->set_selectable();
	component->set_user_data("Module", this);
	component->add_selection_listener(this);
	component->add_translate_listener(this);
	visible = true;
}


void ModulePromGroup::update_component() {
	if(group->type==14) {
		if(!group->custom_function.empty()) {
			try { ((SVGComponent*)component)->set(group->custom_function.c_str()); }
			catch(...) {
				try { ((SVGComponent*)component)->set(group->group.c_str()); }
				catch(...) { ((SVGComponent*)component)->set("module_algo"); }
			}
		} else {
			try { ((SVGComponent*)component)->set(group->group.c_str()); }
			catch(...) { ((SVGComponent*)component)->set("module_algo"); }
		}
	} else {
		try { ((SVGComponent*)component)->set(group->get_type().c_str()); }
		catch(...) { ((SVGComponent*)component)->set("module_neural"); }
	}

	component->remove_all_classes();

	component->add_class(group->is_type_algo() ? "algo" : "neural");
	if(!group->custom_function.empty()) component->add_class("custom_cpp");

	component->add_class(group->group);
}


ModulePromGroup::~ModulePromGroup() {
	group->project->remove(this);
	if(group->script) group->script->remove_group(group);
	delete group;
}




std::ostream& operator<<(std::ostream& os, ModulePromGroup* a) {
	a->dump(os);
	return os;
}

void ModulePromGroup::detach(bool bSlave) {
	if(!bAttached) return;
	Module::detach(bSlave);
	if(group->script) group->script->remove_group(this->group);
	if(group->project) group->project->remove(this);
}

void ModulePromGroup::attach() {
	if(bAttached) return;
	Module::attach();
	if(group->script) group->script->add_group(this->group);
	if(group->project) group->project->add(this);
}

Module* ModulePromGroup::copy() {
	ModulePromGroup* m = new ModulePromGroup(group->copy());
	group->project->add(m);
	m->component->set_pos(component->x,component->y);
	m->attach();
	return m;
}



void ModulePromGroup::on_translate(double x, double y) {
	set_property("posx", TOSTRING((int)(x)));
	set_property("posy", TOSTRING((int)(y)));
	fire_change_event();
}

void ModulePromGroup::scale(double amount) {
	((ModuleComponent*)component)->scale((float)amount);
}

void ModulePromGroup::on_parent_change(Group* newparent) {
	Group* g = newparent;
	while(g && !dynamic_cast<GroupPromScript*>(g)) g = g->parent;
	GroupPromScript* gps = dynamic_cast<GroupPromScript*>(g);
	if(!gps) return;
	if(group->script) group->script->remove_group(group);
	std::string no_name = gps->script->new_noname();
	gps->script->add_group(group);
	group->no_name = no_name;

	process_cross_script_links();
}


void ModulePromGroup::process_cross_script_links() {
	GroupPromScript* script_dst = get_script();
	GroupPromScript* script_src = get_script();
	for(uint i=0; i<in_links.size(); i++) {
		GroupPromScript* script_src = ((ModulePromGroup*)in_links[i]->src)->get_script();
		if(script_src == script_dst) continue;
		group->project->process_cross_script_link((LinkPromLink*)in_links[i]);
	}
	for(uint i=0; i<out_links.size(); i++) {
		GroupPromScript* script_dst = ((ModulePromGroup*)out_links[i]->dst)->get_script();
		if(script_src == script_dst) continue;
		group->project->process_cross_script_link((LinkPromLink*)out_links[i]);
	}
}

GroupPromScript* ModulePromGroup::get_script() {
	Group* g = parent;
	while(g && !dynamic_cast<GroupPromScript*>(g)) g = g->parent;
	return dynamic_cast<GroupPromScript*>(g);
}


void ModulePromGroup::on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val) {
	if(name=="type") {
		group->set_type(val);
		text = type = group->get_type();
		group->set_name(this->name);
		text2 = this->name = group->get_name();
		update_component();
	}
	else if(name=="name") {
		group->set_name(val);
		text2 = this->name = group->get_name();
	} else if(name=="posx" || name=="posy") {
		if(component) component->set_pos(group->posx, group->posy);
	}
}

}
