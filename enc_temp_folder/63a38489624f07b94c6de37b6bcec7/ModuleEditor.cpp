#include "Application.h"

#include "ModuleEditor.h"
#include "SDL\include\SDL.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "imgui\imgui.h"
#include "EditorMainMenu.h"
#include "RandomGenerator.h"

#include "MathGeoLib\include\MathGeoLib.h"
ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{
	main_menu = new EditorMainMenu();
}

ModuleEditor::~ModuleEditor()
{}

bool ModuleEditor::Init()
{
	random_machine = new RandomGenerator();

	sphere = new R_Sphere(vec(0, 0, 0), 3.0f);
	math::Line line =  math::Line(vec(0, 0, 0), vec(0, 5, 0));

	capsule = new R_Capsule(LineSegment(line, 3.0f), 3.0f);
	
	
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
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

	bool result_intersection = capsule->Intersects(*sphere);

	LOG("%d",result_intersection);
	LOG("Random Number 1-100 = %d ", random_machine->RandomInt(1,100));



	return status;
}

bool ModuleEditor::CleanUp()
{
	main_menu->CleanUp();
	delete main_menu;
	delete random_machine;
	return true;
}