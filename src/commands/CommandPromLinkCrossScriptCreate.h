/*
 * CommandPromLinkCrossScriptCreate.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandPromLinkCrossScriptCreate_H_
#define CommandPromLinkCrossScriptCreate_H_


#include <commands/Command.h>
#include "../promethe/PromProject.h"



using namespace libboiboites;
namespace coeos {


class CommandPromLinkCrossScriptCreate : public Command {
public:
	PromProject* project;
	LinkPromLink* l;
	ModulePromGroup* src;
	ModulePromGroup* dst;

	ModulePromGroup* m_f_send;
	ModulePromGroup* m_f_receive;
	LinkPromLink* l_l1;
	LinkPromLink* l_l2;

	bool bFirstTime = true;


public:
	CommandPromLinkCrossScriptCreate(PromProject* project, LinkPromLink* l) : project(project), l(l) {
		src = project->get(l->link->src);
		dst = project->get(l->link->dst);
		Vector2D A = src->component->center();
		Vector2D B = dst->component->center();

		PromGroup* f_send = new PromGroup(src->group->script, "f_send");
		f_send->posx = (int)((A.x + (B.x-A.x)*0.3)/10);
		f_send->posy = (int)((A.y + (B.y-A.y)*0.3)/10);
		m_f_send = new ModulePromGroup(f_send);
		project->add(m_f_send);
		m_f_send->attach();
		m_f_send->component->center(A + (B-A)*0.3);
		Group* g1 = src->parent;
		if(g1) g1->add(m_f_send);

		PromGroup* f_receive = new PromGroup(dst->group->script, "f_recv");
		f_receive->posx = (int)((A.x + (B.x-A.x)*0.7)/10);
		f_receive->posy = (int)((A.y + (B.y-A.y)*0.7)/10);
		m_f_receive = new ModulePromGroup(f_receive);
		project->add(m_f_receive);
		m_f_receive->attach();
		m_f_receive->component->center(A + (B-A)*0.7);
		Group* g2 = dst->parent;
		if(g2) g2->add(m_f_receive);

		PromLink* l1 = new PromLink(project, No_l_algorithmique);
		l1->src = src->group;
		l1->dst = f_send;
		l_l1 = new LinkPromLink(l1);
		l_l1->attach();
		project->add(l_l1);


		PromLink* l2 = new PromLink(project, No_l_algorithmique);
		l2->src = f_receive;
		l2->dst = dst->group;
		l_l2 = new LinkPromLink(l2);
		l_l2->attach();
		project->add(l_l2);

		delete l;
	}
	virtual ~CommandPromLinkCrossScriptCreate() {}

	virtual void execute() {
		if(!bFirstTime) {
			project->add(m_f_receive);
			project->add(m_f_send);
			project->add(l_l2);
			project->add(l_l1);

			m_f_receive->attach();
			m_f_send->attach();
			l_l1->attach();
			l_l2->attach();
		}
		bFirstTime = false;
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		project->remove(m_f_receive);	m_f_receive->detach();
		project->remove(m_f_send);		m_f_send->detach();
		project->remove(l_l2);			l_l2->detach();
		project->remove(l_l1);			l_l1->detach();
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return "Create link";
	}
};


}

#endif /* CommandPromLinkCrossScriptCreate_H_ */
