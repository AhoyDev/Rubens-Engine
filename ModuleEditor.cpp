#include "Application.h"

#include "ModuleEditor.h"
#include "SDL\include\SDL.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "imgui\imgui.h"
#include "EditorMainMenu.h"

ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{
	main_menu = new EditorMainMenu();
}

ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init()
{
	

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	LOG("EDITOR");

	


	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	update_status status = UPDATE_CONTINUE;

	//Bool that handles imgui demo
	if(main_menu->demo)
		ImGui::ShowTestWindow();
	
	//If quit it returns UPDATE_STOP
	
	if(main_menu->show)
		status = main_menu->ShowMainMenu();


	//Pressing M hides the main menu for focused work

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		if (main_menu->show)
			main_menu->show = false;
		else
			main_menu->show = true;
		
		

	}

	




	return status;
}

bool ModuleEditor::CleanUp()
{
	main_menu->CleanUp();
	delete main_menu;
	return true;
}
