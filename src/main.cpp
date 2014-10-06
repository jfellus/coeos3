

#include "util/utils.h"
#include "widget/ZoomableDrawingArea.h"
#include <gtkmm.h>
#include <giomm.h>
#include "components/SVGComponent.h"
#include "components/BoundingBox.h"
#include "components/LinkComponent.h"

// Manage actions
#define START_ACTIONS()				Glib::RefPtr<Gio::SimpleActionGroup> refActionGroup = Gio::SimpleActionGroup::create()
#define ADD_ACTION(label, callback) refActionGroup->add_action(label, sigc::mem_fun(*this, &Window::callback))
#define END_ACTIONS(group_name) 				insert_action_group(group_name,refActionGroup)


class Window : public Gtk::Window {
public:

	ZoomableDrawingArea* canvas;
	Gtk::MenuBar* menubar;
	Gtk::Toolbar* toolbar;


private:

	Gtk::Box box;
	Glib::RefPtr<Gtk::Builder> m_refBuilder;

	//Two sets of choices:
	Glib::RefPtr<Gio::SimpleAction> m_refChoice, m_refChoiceOther;
	Glib::RefPtr<Gio::SimpleAction> m_refToggle;

public:

	Window() : box(Gtk::ORIENTATION_VERTICAL) {
		maximize();
		set_title("Coeos++");
		set_events(Gdk::ALL_EVENTS_MASK);

		create_actions();
		create_menu();
		create_toolbar();

		canvas = new ZoomableDrawingArea(*this);
		box.pack_end(*canvas);
		add(box);
	}
	~Window() {

	}

protected:
	void on_menu_file_new_generic() {}
	void on_menu_file_quit() {this->close();}
	void on_menu_others() {}

	void on_menu_choices(const Glib::ustring& parameter) {}
	void on_menu_choices_other(int parameter) {}
	void on_menu_toggle() {}


	void create_actions() {
		START_ACTIONS();

		// File menu:
		ADD_ACTION("newstandard", on_menu_file_new_generic);
		ADD_ACTION("newfoo", on_menu_file_new_generic);
		ADD_ACTION("newgoo", on_menu_file_new_generic);
		ADD_ACTION("quit",   on_menu_file_quit);

		// Edit menu:
		ADD_ACTION("copy",   on_menu_others);
		ADD_ACTION("paste",  on_menu_others);
		ADD_ACTION("something", on_menu_others);

		// Choices menu:
		m_refChoice = refActionGroup->add_action_radio_string("choice",   			sigc::mem_fun(*this, &Window::on_menu_choices), "a");
		m_refChoiceOther = refActionGroup->add_action_radio_integer("choiceother", 	sigc::mem_fun(*this, &Window::on_menu_choices_other), 1);
		m_refToggle = refActionGroup->add_action_bool("sometoggle",  				sigc::mem_fun(*this, &Window::on_menu_toggle),  false);

		// Help menu:
		ADD_ACTION("about",  on_menu_others );

		END_ACTIONS("main");
	}

	void create_menu() {
		m_refBuilder = Gtk::Builder::create();
		try  {   m_refBuilder->add_from_file("menus.xml"); }
		catch(const Glib::Error& ex)  {   return;  }
		Glib::RefPtr<Glib::Object> object =  m_refBuilder->get_object("menu");
		Glib::RefPtr<Gio::Menu> gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
		menubar = Gtk::manage(new Gtk::MenuBar(gmenu));
		box.pack_start(*menubar, Gtk::PACK_SHRINK);
	}

	void create_toolbar() {
		toolbar = Gtk::manage(new Gtk::Toolbar());

		Gtk::ToolButton* button = Gtk::manage(new Gtk::ToolButton());
		button->set_icon_name("document-new");
		gtk_actionable_set_detailed_action_name (GTK_ACTIONABLE (button->gobj()), "main.newstandard");
		toolbar->add(*button);

		button = Gtk::manage(new Gtk::ToolButton());
		button->set_icon_name("application-exit");
		gtk_actionable_set_detailed_action_name (GTK_ACTIONABLE (button->gobj()), "main.quit");
		toolbar->add(*button);

		box.pack_start(*toolbar, Gtk::PACK_SHRINK);
	}
};


static Window* win = 0;

static void create_random_graph() {
	std::vector<SVGComponent*> modules;
	for(int i=0; i<100; i++) {
		SVGComponent* svg2 = new SVGComponent(rand()%2 ? "test2.svg" : "test2.svg");
		win->canvas->add(svg2);
		svg2->set_pos(rand()%50000,rand()%50000);
		svg2->set_selectable();
		modules.push_back(svg2);
	}

	for(int i=0; i<1000; i++) {
		LinkComponent* l = new LinkComponent(modules[rand()%100], modules[rand()%100]);
		win->canvas->add(l);
	}
}

int main(int argc, char** argv) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	win = new Window;
//	create_random_graph();

	win->show_all();
	return app->run(*win);
}
