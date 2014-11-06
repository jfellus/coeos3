#include "promethe/PromWorkbench.h"
#include "promethe/promethe_modules.h"
#include "dynamics/ModuleBody.h"
#include "promethe/PromProject.h"
#include "layout/LayoutFlow.h"
#include "components/style/CSSDefinitions.h"
#include "promethe/PromNet.h"


static void create_random_graph(int N = 100, int NL = 2) {
	for(int i=0; i<N; i++) {
		Module* m = new Module();
		m->create_component("module_neural");
		m->component->set_pos(rand()%5000,rand()%5000);
	}

	for(int i=0; i<NL; i++) {
		int s = rand()%N;
		int r = rand()%N;
		if(s==r) i--;
		else {
			DBG(s << "->" << r);
			new Link(Document::cur()->get_module(s), Document::cur()->get_module(r));
		}
	}
}

DynEngine* engine = new DynEngine();
static void on_pause() {
	engine->bPause = !engine->bPause;
}

void create_engine() {
	PromWorkbench* w = ((PromWorkbench*)Workbench::cur());
	for(uint i=0; i<w->project->groups.size(); i++) {
			ModulePromGroup* mpg = w->project->groups[i];
			engine->add(new ModuleBody(mpg));
		}
		for(uint k=0; k<w->project->links.size(); k++) {
			LinkPromLink* l = w->project->links[k];
			if(l->src == l->dst) continue;
			for(uint i=0; i<engine->bodies.size(); i++) {
				ModuleBody* a = ((ModuleBody*)engine->bodies[i]);
				if(l->src == a->module || l->dst == a->module) {
					for(uint j=i+1; j<engine->bodies.size(); j++) {
						ModuleBody* b = ((ModuleBody*)engine->bodies[j]);
						if(l->src == b->module || l->dst == b->module) {
							a->addLink(b);
							b->addLink(a);
							a = b = NULL;
						}
						if(b==NULL) break;
					}
				}
				if(a==NULL) break;
			}
		}

		w->canvas->add_key_listener(new IKeyListener(GDK_KEY_space, 0, on_pause));
		engine->start();
}

void on_key_layout() {
	PromWorkbench* w = (PromWorkbench*)Workbench::cur();
	LayoutFlow* l = new LayoutFlow();
		for(uint i=0; i<w->project->groups.size(); i++) {
			l->add(w->project->groups[i]);
		}
		for(uint i=0; i<w->project->links.size(); i++) {
			l->add(w->project->links[i]);
		}
		l->layout();
}

int main(int argc, char** argv) {
	gtk_init(&argc, &argv);
	PromWorkbench* w = new PromWorkbench();


	CSSDefinitions::add("basic.defs");
	CSSDefinitions::add("test.css");
	SVGDefinitions::add("style/svg");
	SVGDefinitions::add("style/svg2");

	w->open("/home/jfellus/Documents/These/prog/promethe_orig/applications/devel_applis/DM_PerAc_visuomotor_control_Tino/visualPlanning.net");
	//create_engine(); // TEST NEWTON DYNAMICS

	w->canvas->add_key_listener(new IKeyListener(GDK_KEY_l, GDK_CONTROL_MASK, on_key_layout));

//	create_random_graph(2, 1);
	w->run();
	return 0;
}


