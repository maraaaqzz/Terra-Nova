#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"
#include "OpenGL/Renderer/Renderer.h"
#include "OpenGL/Renderer/Camera.h"
#include "System/System.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define screen_width 1600.0f
#define screen_height 900.0f

enum class application_state
{
    main_menu,
    navigation,
    information_tab,
};

struct pinpoint
{
    glm::vec2 position;
    float size = 30.0f;

    static float scale;

    pinpoint() = default;
    pinpoint(const glm::vec2& p) : position(p) { }

    bool is_hovered(const glm::vec2& mousepos) const
    {
        if (position.x + size / 2.0f / scale > mousepos.x && position.x - size / 2.0f / scale < mousepos.x &&
            position.y + size / 2.0f / scale > mousepos.y && position.y - size / 2.0f / scale < mousepos.y)
            return true;
        return false;
    }

    bool is_pressed(const glm::vec2& mousepos) const
    {
        if (is_hovered(mousepos) && input::is_mouse_key_pressed(GLFW_MOUSE_BUTTON_LEFT))
            return true;
        return false;
    }

    void operator=(const pinpoint& other)
    {
        position.x = other.position.x;
        position.y = other.position.y;
    }
};

float pinpoint::scale = 1.0f;

void read_pinpoints_from_file(const std::string& filepath, std::vector<pinpoint>& pp)
{
    std::ifstream f(filepath.c_str());
    if (f.good())
    {
        int size;
        f >> size;
        pp.resize(size);

        for (int i = 0; i < size; i++)
        {
            f >> pp[i].position.x >> pp[i].position.y;
            pp[i].position.x *= screen_width;
            pp[i].position.y *= screen_height;
        }
    }
}

void read_locations_from_file(const std::string& filepath, std::vector<std::pair<pinpoint, std::string>>& pp)
{
    std::ifstream f(filepath.c_str());
    if (f.good())
    {
        int size;
        f >> size;
        pp.resize(size);

        for (int i = 0; i < size; i++)
        {
            f >> pp[i].first.position.x >> pp[i].first.position.y >> pp[i].second;
            pp[i].first.position.x *= screen_width;
            pp[i].first.position.y *= screen_height;
        }
    }
}

int main()
{
    GLFWwindow* window; //declaram fereastra 

    if (glfwInit() == false) // initializam glfw ul care na ajuta la crearea ferestrei
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, false); // nu poti sa dai resize
    window = glfwCreateWindow(screen_width, screen_height, "Terra Nova", NULL, NULL); // creez fereastra

    if (!window) // daca fereastra are vbreo eraore ies
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // ba opengl deseanaza aici pe fereastra asta

    if (glewInit() != GLEW_OK) // glew initializam, asta face leagutra la functiile din opengl care ne ajuta la desenat
        return -1;

    glEnable(GL_BLEND); // 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderer::init(); // dam init la renderer, declaram vertex buffer(colturi poligon, index buffer (ordinea colturilor), vertex array(da layout la buffer)
    input::init(window); // dam init la input ne ajuta la preluare evenimente mouse si tastatura

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) -> void {
            input::scoll_state = yoffset; }); // cat de mult e rotita rotita in jos sau sus

    application_state app_state = application_state::main_menu; // declaram state-ul aplicatiei, main menu

    // creez camera
    camera map_camera(screen_width, screen_height); // camera hartii care se misca
    camera navigation_camera(screen_width, screen_height); // camera aplicatiei care e statica

    // incarc texturi
    texture loading("Resources/Textures/loading.png"); 
    texture main_menu("Resources/Textures/main_menu.png");

    // afisez inainte sa incarc texturile ca sa nu fie ecranul alb
    renderer::set_camera(map_camera);
    renderer::clear(0.0f, 0.0f, 0.0f);
    renderer::draw(main_menu, { screen_width / 2.0f, screen_height / 2.0f }, { screen_width, screen_height });
    renderer::draw(loading, { screen_width / 2.0f, screen_height / 2.0f - 100.0f }, { 300.0f, 50.0f });
    glfwSwapBuffers(window);

    // incarc alte texturi
    texture world_map("Resources/Textures/world_map.png");
    texture pin_point("Resources/Textures/pin_point.png");
    texture scroll("Resources/Textures/scroll.png");

    texture left_handle("Resources/Textures/left_handle.png");
    texture right_handle("Resources/Textures/right_handle.png");

    // delta_time ca sa fie fluida miscarea
    float delta_time;
    float beg_time = glfwGetTime();
    float end_time = beg_time;

    int map_position = -50.0f;

    // variabilele pentru state ul de navigatie

    /*
    std::vector este o optiune mai moderna pentru vectori pe care ii facem la scoala.
    la scoala pentru a face un vector de int scriem:
    int v[100]; // trebuie neaparat sa specificam dimensiunea

    int c++ mai modern (c++03)
    std::vector<int> v; // variabila asta are dimensiunea dinamica si cand nu stim exact cu cate elemente ne ocupam folosim vector ca sa nu folosim prea mult sau prea putin memorie

    */
    std::vector<pinpoint> pinpoints;

    /*
    un pair este o variabila care "leaga" 2 tipuri de date.
    de exemplu fiecare "pinpoint" de pe harta noastra reprezinta o locatie reprezentata printr-un string
    std::pair<pinpoint, std::string> castelul_corvinilor; // de exemplu
    castelul_corvinilor.first; // asa chem prima variabila din pair
    castelul_corvinilor.second; // asa chem a doua variabila, respectiv std::string-ul
    */
    std::vector<std::pair<pinpoint, std::string>> locations;
    std::vector<texture*> location_textures;

    read_pinpoints_from_file("continents.txt", pinpoints); // incarc locatiile
    read_locations_from_file("locations.txt", locations); // incarc locatiile

    // aici incarc imaginile specifice fiecarei locatii
    for(const auto& location : locations)
        location_textures.push_back(new texture("Resources/Textures/Locatii/" + location.second + ".png"));

    pinpoint last_selected; // memorez ce am selectat ultima oara
    bool is_last_selected = false; // memorez daca a fost selectat inainte de vrun event sau cv
    float cam_speed = 0.0f; // viteza camerei, o retin pentru ca trebuie calculata in momentul in care apas pe un pinpoint
    bool cam_moved = false; // daca s-a miscat camera sau nu
    glm::vec2 mousepos; // imi trebuie in fiecare frame in mai multe locuri asa ca mai bine retin odata sus si gata!
    bool camera_can_move = true;

    // variabile pentru state-ul de informatii
    int start_x = screen_width / 2.0f;
    int handle_width = 70.0f, handle_height = 700.0f;
    glm::vec2 left_handle_position = {start_x - handle_width / 2.0f, screen_height / 2.0f },
        right_handle_position = { start_x + handle_width / 2.0f, screen_height / 2.0f };
    float main_paper_width = 1200.0f, main_paper_height = 580.0f;
    bool close_scroll = false;

    int open_img_id;

    while (!glfwWindowShouldClose(window))
    {
        // calculez delta_time
        beg_time = glfwGetTime();
        delta_time = beg_time - end_time;
        end_time = beg_time;

        // curat ecranul, trebuie facut indiferent de state
        renderer::clear(0.0f, 0.0f, 0.0f);

        switch (app_state)
        {

        case application_state::main_menu:

            renderer::set_camera(navigation_camera); // setez camera statica pt ca nu vreau sa misc camera de pe ecranul principal
            renderer::draw(main_menu, { screen_width / 2.0f, screen_height / 2.0f }, { screen_width, screen_height }); // desenez maniul principal
            if (input::is_key_pressed(GLFW_KEY_ENTER)) // cand apas enter schimb state-ul la navigation
                app_state = application_state::navigation;
            break;

        case application_state::navigation:

            renderer::set_camera(map_camera); // stez camera care se misca 
            mousepos = input::get_mouse_position();
            // le aduc in sistemul de coordonate care imi trebe mie, pentru mouse (0,0) e sus in stanga dar eu il vreau jos in dreapta, asa e facut tot sistemu meu
            mousepos.y = screen_height - mousepos.y;
            // le aduc in sistemul de coordonate ale camerei, pentru camera (0,0) e in centrul ecranului
            mousepos = map_camera.to_world_coord(mousepos);

            cam_moved = map_camera.update(delta_time, camera_can_move); // ii dau update
            pinpoint::scale = map_camera.get_zoom_level();

            if (map_position < screen_width) // daca harta nu e complet deschisa, o misc spre dreapta
            {
                map_position += 1000.0f * delta_time;
                renderer::draw(main_menu, { screen_width / 2.0f, screen_height / 2.0f }, { screen_width, screen_height });
            }

            // afisez harta partial in functie de cat de mult de deschisa
            renderer::draw(world_map, { map_position / 2.0f, screen_height / 2.0f }, { map_position, screen_height }, map_position / screen_width);
            renderer::draw(scroll, { map_position, 450.0f }, { 130.0f, 930.0f }); // afisez manele hartii

            if (map_position >= screen_width) // daca harta e complet deschisa
            {
                // verific nivelul de zoom, pentru fiecare nivel de zoom fac alte actiuni
                if (map_camera.get_zoom_level() < 2.5f)
                {
                    // trec prin fiecare buscola
                    for (auto& pp : pinpoints)
                    {
                        // daca mouseul e peste busola atunci fac busopla mai mare
                        if (pp.is_hovered(mousepos))
                        {
                            if (pp.size < 45.0f)
                                pp.size += 50.0f * delta_time;
                        }
                        else if (pp.size > 30.0f)
                            pp.size -= 70.0f * delta_time;

                        if (pp.is_pressed(mousepos) && !is_last_selected) // daca apas pe busola dau zoom catre ea
                        {
                            if (map_camera.get_zoom_level() == 1.0f)
                            {
                                input::scoll_state = 1;
                                // calculez viteza in functie de distanta, distanta / viteza, viteza e cam.max_zoom / cam.zoom_speed
                                cam_speed = glm::distance(map_camera.get_position() + glm::vec2(screen_width / 2.0f, screen_height / 2.0f), pp.position) / (2.5f / 1.1f);
                            }
                            last_selected = pp;
                            is_last_selected = true;
                        }

                        // desenez busola
                        renderer::draw(pin_point, pp.position, { pp.size / map_camera.get_zoom_level(), pp.size / map_camera.get_zoom_level() });
                    }
                }
                else // daca camera e zoom pe harta
                {
                    //trec prin fiecare busola
                    for (int i = 0; i < locations.size(); i++)
                    {
                        auto& pp = locations[i].first;
                        // daca e mouseul pe ea o fac mai mare
                        if (pp.is_hovered(mousepos))
                        {
                            if (pp.size < 45.0f)
                                pp.size += 50.0f * delta_time;
                        }
                        else if (pp.size > 30.0f)
                            pp.size -= 70.0f * delta_time;

                        // daca o apas trec la tabul de informatii unde se afiseaza textura cu date despre locatie
                        if (pp.is_pressed(mousepos))
                        {
                            app_state = application_state::information_tab;
                            open_img_id = i;
                        }
                        
                        // desenez busola
                        renderer::draw(pin_point, pp.position, { pp.size / map_camera.get_zoom_level(), pp.size / map_camera.get_zoom_level() });
                    }
                }

                // sa nu pot sa misc camera in imtp ca si da zoom
                if (camera_can_move == is_last_selected)
                    camera_can_move = !is_last_selected;

                // daca a fost selectata o busola ma misc catre ea
                if (is_last_selected)
                {
                    bool location_reached = map_camera.move_towards(last_selected.position, cam_speed * delta_time);
                    // daca ajung in locatie sau misc camera in timp ce se misca sigura se opreste si reiau controlul
                    if (cam_moved || location_reached)
                        is_last_selected = false;
                }
            }

            break;

        case application_state::information_tab:
            float distance = right_handle_position.x - left_handle_position.x;

            // simulez animatia de deschidere pergament
            if (distance < main_paper_width && !close_scroll)
            {
                right_handle_position.x += 500.0f * delta_time;
                left_handle_position.x -= 500.0f * delta_time;
            }
            else if (close_scroll == true)
            {
                right_handle_position.x -= 800.0f * delta_time;
                left_handle_position.x += 800.0f * delta_time;
            }

            // afisez texturile
            renderer::draw(world_map, { map_position / 2.0f, screen_height / 2.0f }, { map_position, screen_height }, map_position / screen_width);
            renderer::draw(scroll, { map_position, screen_height / 2.0f }, { 130.0f, 930.0f });

            renderer::set_camera(navigation_camera);
            
            renderer::draw(*location_textures[open_img_id], {left_handle_position.x + distance / 2.0f, left_handle_position.y}, {distance, main_paper_height}, distance / main_paper_width);
            
            renderer::draw(right_handle, right_handle_position, { handle_width, handle_height });
            renderer::draw(left_handle, left_handle_position, { handle_width, handle_height });

            renderer::set_camera(map_camera);

            if (input::is_key_pressed(GLFW_KEY_ESCAPE) || input::is_mouse_key_pressed(GLFW_MOUSE_BUTTON_RIGHT))
                close_scroll = true;

            if (distance <= handle_width && close_scroll)
            {
                app_state = application_state::navigation;
                close_scroll = false;
                start_x = screen_width / 2.0f;
                left_handle_position = { start_x - handle_width / 2.0f, screen_height / 2.0f };
                right_handle_position = { start_x + handle_width / 2.0f, screen_height / 2.0f };
            }
            break;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }  

    // golesc memoria
    for (auto& tex : location_textures)
        delete tex;

    // inchid tot
    glfwTerminate();
    renderer::close();

    return 0;
}