#include "App.h"

App::App()
    :
    window(1920, 1080),
    scene("res/scenes/BasicScene.scene")
{
    glfwGetCursorPos(window.Get(), &last_x, &last_y);
}

void App::Loop(float dt)
{
    scene.Draw(dt);
    skybox.Draw(scene.camera);

    for (int i = 0; i < scene.models.size(); i++)
    {
        auto model = scene.models[i];
        if (model.tag == Tags::COLLECTIBLE) 
        {
            if ((scene.camera.position - model.pos).length() < 2.0f)
            {
                inventory.push_back(model.name);
                std::cout << "Inventory item added" << std::endl;
            }
        }
    }
}

void App::Move(float dt)
{
    if (glfwGetKey(window.Get(), GLFW_KEY_W) == GLFW_PRESS)
    {
        scene.camera.Translate(glm::vec3(0.0f, 0.0f, dt));
    }

    if (glfwGetKey(window.Get(), GLFW_KEY_S) == GLFW_PRESS)
    {
        scene.camera.Translate(glm::vec3(0.0f, 0.0f, -dt));
    }

    if (glfwGetKey(window.Get(), GLFW_KEY_A) == GLFW_PRESS)
    {
        scene.camera.Translate(glm::vec3(-dt, 0.0f, 0.0f));
    }

    if (glfwGetKey(window.Get(), GLFW_KEY_D) == GLFW_PRESS)
    {
        scene.camera.Translate(glm::vec3(dt, 0.0f, 0.0f));
    }

    if (!game_paused) 
    {
        double xpos, ypos;
        glfwGetCursorPos(window.Get(), &xpos, &ypos);

        float dx = (float)(xpos - last_x);
        float dy = (float)(ypos - last_y);

        scene.camera.Rotate(dx, dy);

        last_x = xpos;
        last_y = ypos;
    }

    if (glfwGetKey(window.Get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        game_paused = !game_paused;

        if (game_paused)
        {
            glfwSetInputMode(window.Get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window.Get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
}

int App::Go()
{
    while (!glfwWindowShouldClose(window.Get()))
    { 
        window.StartFrame();

        float dt = timer.Mark();

        Loop(dt);
        Move(dt);

        if (ImGui::Begin("Jounral"))
        {
            ImGui::Text("Day 1");
            ImGui::Spacing();
            ImGui::TextWrapped("I have been washed on this shore i must survive till the help comes."
                        "i must move around the island and find all the stuff i've lost from the boat");
            ImGui::Spacing();
            ImGui::BulletText("Find Fishing Rod");
            ImGui::Spacing();
            ImGui::BulletText("Find Coconuts");

        }
        ImGui::End();

        if (ImGui::Begin("Inventory"))
        {
            for (size_t i = 0; i < inventory.size(); i++)
            {
                ImGui::BulletText(inventory[i].c_str());
            }

            if (inventory.size() == 0) ImGui::Text("No items in the inventory");
        }
        ImGui::End();

        window.EndFrame();
    }

    return 0;
}

App::~App()
{
}