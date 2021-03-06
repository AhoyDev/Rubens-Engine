#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePhysics3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"

#include "FileManager.h"
#include "TimeManager.h"
#include "RandomGenerator.h"
#include "GUI_Console.h"
#include "JSONNode.h"
#include "Globals.h"
#include "GUI_Config.h"

#include "Brofiler/include/Brofiler.h"

Application::Application() :
	fm(nullptr), time(nullptr),
	window(nullptr), input(nullptr), audio(nullptr), physics3D(nullptr),
	camera(nullptr), scene_intro(nullptr), editor(nullptr), renderer3D(nullptr)
{
	request_restart = request_save = false;

	fm = new WindowsFileManager();
	time = new TimeManager();
	rnd = new RandomGenerator();

	// Modules
	window = new ModuleWindow("Window");
	input = new ModuleInput("Input");
	audio = new ModuleAudio("Audio");
	physics3D = new ModulePhysics3D("Physics");
	camera = new ModuleCamera3D("Camera");
	scene_intro = new ModuleSceneIntro("Scene Intro");
	editor = new ModuleEditor("Editor");
	renderer3D = new ModuleRenderer3D("Render");

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(audio);
	AddModule(physics3D);
	AddModule(camera);

	AddModule(scene_intro);

	AddModule(editor);
	AddModule(renderer3D);
}

Application::~Application()
{
	/* Triggers error in Release: can't access elements
	delete fm;
	delete time;
	delete config;*/

	std::list<Module*>::reverse_iterator i = list_modules.rbegin();
	for (; i != list_modules.rend(); ++i)
	{
		delete (*i);
	}
}

bool Application::Init()
{
	bool ret = true;

	SetConfig();
	Load();

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	for (; ret && item != list_modules.end(); item++)
	{
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------\n");
	item = list_modules.begin();
	for (; ret && item != list_modules.end(); item++)
	{
		if((*item)->IsEnabled()) ret = (*item)->Start();
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	//BROFILER_FRAME("GameLoop")
	update_status ret = UPDATE_CONTINUE;

	if (request_restart)
	{
		request_restart = false;
		ret = UPDATE_RESTART;
	}
	else
	{
		float dt = time->UpdateDeltaTime();

		std::list<Module*>::iterator item = list_modules.begin();
		for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->PreUpdate(dt);
		}

		item = list_modules.begin();
		for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->Update(dt);
		}

		item = list_modules.begin();
		for (; ret == UPDATE_CONTINUE && item != list_modules.end(); item++)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->PostUpdate(dt);
		}

		FinishUpdate();
	}
	
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	Save();

	std::list<Module*>::reverse_iterator rit = list_modules.rbegin();
	for (; rit != list_modules.rend() && ret; ++rit)
	{
		ret = (*rit)->CleanUp();
	}

	return ret;
}

update_status Application::Restart()
{
	return (CleanUp() && Init()) ? UPDATE_CONTINUE : UPDATE_STOP;
}

void Application::RequestRestart()
{
	request_restart = true;
}

void Application::RequestSave()
{
	request_save = true;
}

void Application::OpenURL(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::SetConfig()
{
	char* buffer = nullptr;

	if (!fm->LoadFileToBuffer(&buffer, "Configuration.json")) // couldn't load config
	{
		LOG("Error while loading Configuration file. Creating new JSON stream\n");

		config = new JSONNode();

		std::list<Module*>::iterator i = list_modules.begin();
		for (; i != list_modules.end(); ++i)
			config->PushJObject((*i)->GetName());

		uint size = config->Serialize(&buffer);
		fm->Save("Configuration.json", buffer);
	}
	else
	{
		config = new JSONNode(buffer);
	}

	delete[] buffer;
}

void Application::SetConfigValues()
{
	config_values.config_max_fps = config_values.max_fps;
	config_values.config_width = config_values.width;
	config_values.config_height = config_values.height;
	config_values.config_vsync = config_values.vsync;
	config_values.config_fullScreen = config_values.fullScreen;
	config_values.config_resizable = config_values.resizable;
	config_values.config_borderless = config_values.borderless;
	config_values.config_fullscreenDesktop = config_values.fullscreenDesktop;
}

void Application::FinishUpdate()
{
	time->ManageFrameTimers();

	if (request_save)
	{
		request_save = false;
		Save();
		Load();
	}

	// manage events
	// check if needed resources must load for next frame
}

void Application::Save() const
{
	std::list<Module*>::const_iterator item = list_modules.begin();
	for (; item != list_modules.end(); item++)
	{
		(*item)->Save(&config->PullJObject((*item)->GetName()));
	}

	char* buffer = nullptr;
	uint size = config->Serialize(&buffer);
	fm->Save("Configuration.json", buffer);
	delete[] buffer;
}

void Application::Load()
{
	config_values.max_fps = config->PullInt("FPS Cap", 60);
	std::list<Module*>::const_iterator item = list_modules.begin();
	for (; item != list_modules.end(); item++)
	{
		(*item)->Load(&config->PullJObject((*item)->GetName()));
	}

	SetConfigValues();
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

bool ConfigValues::NeedSaving()
{
	return (config_max_fps != max_fps ||
		config_width != width ||
		config_height != height ||
		config_vsync != vsync ||
		config_fullScreen != fullScreen ||
		config_resizable != resizable ||
		config_borderless != borderless ||
		config_fullscreenDesktop != fullscreenDesktop);
}

bool ConfigValues::NeedRestart()
{
	return false;
}

