/*
 *  This file is part of gPortScope
 *
 *  Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@iit.it>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.h"
#include "DataPlot.h"
#include "Debug.h"

#include <glibmm/i18n.h>

#include <gtkmm/aboutdialog.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/box.h>
#include <gtkmm/stock.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/menubar.h>



namespace GPortScope {

class MainWindow::Private
{
public:
    Private(MainWindow *parent) :
        parent(parent),
        refActionGroup(Gtk::ActionGroup::create()),
        refUIManager(Gtk::UIManager::create()),
        running(true)
    {
    }

    // Signal handlers:
    void on_action_file_quit();
    void on_action_help_about();
    void on_action_actions_stop_start();

    // parent window
    MainWindow * const parent;

    // Child widgets:
    Glib::RefPtr<Gtk::ActionGroup> refActionGroup;
    Glib::RefPtr<Gtk::UIManager> refUIManager;

    Gtk::VBox windowBox;
    DataPlot databox;

    // Other private members
    bool running;
};

} // GPortScope



void GPortScope::MainWindow::Private::on_action_file_quit()
{
    debug() << "Quit clicked";
    parent->hide();
}

void GPortScope::MainWindow::Private::on_action_help_about()
{
    debug() << "About clicked";

    Gtk::AboutDialog dialog;
    dialog.set_transient_for(*parent);

    dialog.set_comments("A simple graphical user interface for visualizing the numerical content of a yarp port.");

    std::list<Glib::ustring> authors;
    authors.push_back("Daniele E. Domenichelli <daniele.domenichelli@iit.it>");
    dialog.set_authors(authors);

    dialog.set_license("Copyright (C) 2012 Daniele E. Domenichelli <daniele.domenichelli@iit.it>\n"
                       "Copyright (C) 2006 Francesco Nori <francesco.nori@iit.it>\n"
                       "\n"
                       "This program is free software: you can redistribute it and/or modify\n"
                       "it under the terms of the GNU General Public License as published by\n"
                       "the Free Software Foundation, either version 2 of the License, or\n"
                       "(at your option) any later version.\n"
                       "\n"
                       "This program is distributed in the hope that it will be useful,\n"
                       "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                       "GNU General Public License for more details.\n"
                       "\n"
                       "You should have received a copy of the GNU General Public License\n"
                       "along with this program.  If not, see <http://www.gnu.org/licenses/>.");

    dialog.run();
}

void GPortScope::MainWindow::Private::on_action_actions_stop_start()
{
    Glib::RefPtr<Gtk::Action> stopStartAction = refActionGroup->get_action("StopStart");
    if(!stopStartAction) {
        fatal() << "Action \"StopStart\" is missing.";
    }

    if (running) {
        debug() << "Stop clicked";
        stopStartAction->set_icon_name("media-playback-start");
        stopStartAction->set_label(_("Start"));
        stopStartAction->set_tooltip(_("Start plotting"));
    } else {
        debug() << "Start clicked";
        stopStartAction->set_icon_name("media-playback-pause");
        stopStartAction->set_label(_("Stop"));
        stopStartAction->set_tooltip(_("Stop plotting"));
    }

    running = !running;
}





GPortScope::MainWindow::MainWindow()
    : mPriv(new Private(this))
{
    set_border_width(3);
    set_default_size(640, 480);
    set_icon_name("gportscope"); // FIXME

    add(mPriv->windowBox);

    // Setup actions
    mPriv->refActionGroup = Gtk::ActionGroup::create();

    mPriv->refActionGroup->add(Gtk::Action::create("MenuFile", _("_File")));
    mPriv->refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
                sigc::mem_fun(*mPriv, &MainWindow::Private::on_action_file_quit));
    mPriv->refActionGroup->add(Gtk::Action::create("MenuActions", _("_Actions")));
    mPriv->refActionGroup->add(Gtk::Action::create("StopStart", _("Stop"), _("Stop plotting")),
                sigc::mem_fun(*mPriv, &MainWindow::Private::on_action_actions_stop_start));
    mPriv->refActionGroup->add(Gtk::Action::create("MenuHelp", _("Help")));
    mPriv->refActionGroup->add(Gtk::Action::create("About", Gtk::Stock::ABOUT),
                sigc::mem_fun(*mPriv, &MainWindow::Private::on_action_help_about));

    Glib::RefPtr <Gtk::Action> stopStartAction = mPriv->refActionGroup->get_action("StopStart");
    if(!stopStartAction) {
        fatal() << "Action \"StopStart\" is missing.";
    }
    stopStartAction->set_icon_name("media-playback-pause");


    mPriv->refUIManager->insert_action_group(mPriv->refActionGroup);
    add_accel_group(mPriv->refUIManager->get_accel_group());

    Glib::ustring ui_info =
    "<ui>"
    "  <menubar name='MenuBar'>"
    "    <menu action='MenuFile'>"
    "      <separator />"
    "      <menuitem action='Quit'/>"
    "    </menu>"
    "    <menu action='MenuActions'>"
    "      <menuitem action='StopStart'/>"
    "    </menu>"
    "    <menu action='MenuHelp'>"
    "      <menuitem action='About'/>"
    "    </menu>"
    "  </menubar>"
    "  <toolbar  name='ToolBar'>"
    "    <toolitem action='StopStart'/>"
    "  </toolbar>"
    "</ui>";

    try {
        mPriv->refUIManager->add_ui_from_string(ui_info);
    }
    catch(const Glib::Error& ex) {
        fatal() << "building menus failed: " <<  ex.what();
    }

    // Setup menu bar
    Gtk::MenuBar* menubar = dynamic_cast<Gtk::MenuBar*>(mPriv->refUIManager->get_widget("/MenuBar"));
    if(menubar) {
        mPriv->windowBox.pack_start(*menubar, Gtk::PACK_SHRINK);
    } else {
        fatal() << "building menus failed: \"/MenuBar\" is missing";
    }

    // Setup toolbar
    Gtk::Toolbar* toolbar = dynamic_cast<Gtk::Toolbar*>(mPriv->refUIManager->get_widget("/ToolBar"));
    if(toolbar) {
        mPriv->windowBox.pack_start(*toolbar, Gtk::PACK_SHRINK);
        toolbar->set_toolbar_style(Gtk::TOOLBAR_BOTH);
    } else {
        fatal() << "building menus failed: \"/ToolBar\" is missing";
    }

    mPriv->windowBox.pack_start(mPriv->databox);

    show_all_children();
}

GPortScope::MainWindow::~MainWindow()
{
    delete mPriv;
}
