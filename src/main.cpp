

#include "util/utils.h"
#include "widget/ZoomableDrawingArea.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "components/SVGComponent.h"

int main(int argc, char** argv) {
	   Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	   Gtk::Window win;
	   win.set_title("Coeos++");
	   win.set_events(Gdk::ALL_EVENTS_MASK);


	   ZoomableDrawingArea c;
	   win.add(c);
	   c.show();


	   SVGComponent* svg = new SVGComponent("test.svg");
	   c.add(svg);

	   SVGComponent* svg2 = new SVGComponent("test.svg");
	   c.add(svg2);
	   svg2->set_pos(500,500);

	   return app->run(win);
}
