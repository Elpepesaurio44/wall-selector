#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

class WallSelector : public Gtk::Window {
public:
    WallSelector() {
        Gtk::Settings::get_default()->property_gtk_application_prefer_dark_theme() = true;

        set_title("ZeroSignal Wallpaper Selector");
        set_default_size(800, 600);
        set_position(Gtk::WIN_POS_CENTER);
        set_name("main-window");

        m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
        add(m_box);

        m_scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        m_box.pack_start(m_scrolled_window, Gtk::PACK_EXPAND_WIDGET);

        m_flowbox.set_max_children_per_line(3);
        m_flowbox.set_selection_mode(Gtk::SELECTION_SINGLE);
        m_flowbox.set_homogeneous(true);
        m_flowbox.set_row_spacing(15);
        m_flowbox.set_column_spacing(15);
        m_scrolled_window.add(m_flowbox);

        // Contenedor horizontal para los botones
        m_button_box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        m_button_box.set_spacing(10);
        m_button_box.set_margin_bottom(10);
        m_button_box.set_margin_top(10);
        m_button_box.set_margin_left(20);
        m_button_box.set_margin_right(20);
        m_box.pack_start(m_button_box, Gtk::PACK_SHRINK);

        // Botón Aplicar Seleccionado
        m_apply_button.set_label("APLICAR SELECCIÓN");
        m_apply_button.set_name("apply-button");
        m_apply_button.set_hexpand(true);
        m_apply_button.signal_clicked().connect(sigc::mem_fun(*this, &WallSelector::on_apply_clicked));
        m_button_box.pack_start(m_apply_button, Gtk::PACK_EXPAND_WIDGET);

        // Botón Aleatorio
        m_random_button.set_label("ALEATORIO");
        m_random_button.set_name("random-button");
        m_random_button.set_hexpand(true);
        m_random_button.signal_clicked().connect(sigc::mem_fun(*this, &WallSelector::on_random_clicked));
        m_button_box.pack_start(m_random_button, Gtk::PACK_EXPAND_WIDGET);

        load_pywal_colors();
        load_config();
        refresh_grid();

        show_all_children();
    }

protected:
    void load_pywal_colors() {
        std::string json_path = std::string(getenv("HOME")) + "/.cache/wal/colors.json";
        if (!fs::exists(json_path)) return;

        try {
            std::ifstream f(json_path);
            json data = json::parse(f);

            std::string bg = data["special"]["background"];
            std::string fg = data["special"]["foreground"];
            std::string accent = data["colors"]["color2"];

            auto css_provider = Gtk::CssProvider::create();
            std::string custom_css = 
                "#main-window { background-color: " + bg + "; }"
                "#apply-button, #random-button { background-color: " + accent + "; color: " + fg + "; border-radius: 8px; padding: 12px; font-weight: bold; border: none; }"
                "flowboxchild:selected { background-color: " + accent + "; border-radius: 10px; }";

            css_provider->load_from_data(custom_css);
            Gtk::StyleContext::add_provider_for_screen(
                Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
        } catch (...) {}
    }

    void refresh_grid() {
        for (auto child : m_flowbox.get_children()) m_flowbox.remove(*child);
        m_file_paths.clear();

        if (!fs::exists(m_current_path)) return;

        for (const auto& entry : fs::directory_iterator(m_current_path)) {
            std::string ext = entry.path().extension();
            if (ext == ".jpg" || ext == ".png" || ext == ".jpeg") {
                try {
                    auto pixbuf = Gdk::Pixbuf::create_from_file(entry.path().string(), 220, 140, true);
                    auto image = Gtk::make_managed<Gtk::Image>(pixbuf);
                    auto label = Gtk::make_managed<Gtk::Label>(entry.path().filename().string());
                    label->set_ellipsize(Pango::ELLIPSIZE_END);
                    
                    auto item_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
                    item_box->pack_start(*image);
                    item_box->pack_start(*label);
                    
                    m_flowbox.add(*item_box);
                    m_file_paths.push_back(entry.path().string());
                } catch (...) { continue; }
            }
        }
        show_all_children();
    }

    void apply_wallpaper(const std::string& path) {
        std::cout << "Aplicando: " << path << std::endl;
        std::string cmd = "killall swaybg 2>/dev/null; swaybg -i " + path + " -m fill & wal -i " + path + " -q";
        system(cmd.c_str());
        Glib::signal_timeout().connect_once([this](){ load_pywal_colors(); }, 600);
    }

    void on_apply_clicked() {
        auto selected = m_flowbox.get_selected_children();
        if (!selected.empty()) {
            apply_wallpaper(m_file_paths[selected[0]->get_index()]);
        }
    }

    void on_random_clicked() {
        if (m_file_paths.empty()) return;

        // Generador de números aleatorios
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, m_file_paths.size() - 1);

        int random_index = dis(gen);
        apply_wallpaper(m_file_paths[random_index]);
    }

    bool on_key_press_event(GdkEventKey* key_event) override {
        if ((key_event->state & GDK_CONTROL_MASK) && key_event->keyval == GDK_KEY_s) {
            select_folder();
            return true;
        }
        return Gtk::Window::on_key_press_event(key_event);
    }

    void select_folder() {
        Gtk::FileChooserDialog dialog(*this, "Carpeta de Wallpapers", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
        dialog.add_button("_Cancelar", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Seleccionar", Gtk::RESPONSE_OK);
        if (dialog.run() == Gtk::RESPONSE_OK) {
            m_current_path = dialog.get_filename();
            std::ofstream(std::string(getenv("HOME")) + "/.config/wall-selector-path") << m_current_path;
            refresh_grid();
        }
    }

    void load_config() {
        std::string config_file = std::string(getenv("HOME")) + "/.config/wall-selector-path";
        if (fs::exists(config_file)) {
            std::ifstream file(config_file);
            std::getline(file, m_current_path);
        }
        if (m_current_path.empty() || !fs::exists(m_current_path)) {
            m_current_path = std::string(getenv("HOME")) + "/Pictures/Wallpapers";
        }
    }

    Gtk::Box m_box, m_button_box;
    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::FlowBox m_flowbox;
    Gtk::Button m_apply_button, m_random_button;
    std::vector<std::string> m_file_paths;
    std::string m_current_path;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.zerosignal.selector");
    WallSelector window;
    return app->run(window);
}
