#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

class SDL_Window;
class SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow(const char* name, bool start_enabled = true);
	~ModuleWindow();

	bool Init(JSONNode config);
	bool CleanUp();
	void Save(JSONNode* config);
	void Load(JSONNode* config);

	int GetWidth() const;
	int GetHeight() const;
	float GetBrightness()const;
	bool CheckFlag(uint flag) const;

	void SetBrightness(const float brightness);
	void SetTitle(const char* new_title = nullptr);
	void SetFullScreen(const bool flag_value);
	void SetBorderless(const bool flag_value);
	void SetFullDesktop(const bool flag_value);
	void SetWindowSizei(int new_width, int new_height);

	void SwapFullScreen();
	void SwapBorderless();
	void SwapFullDesktop();

	//Outdated
	void SetWindowSize(float, float);

public:

	SDL_Window* window;
	SDL_Surface* screen_surface;

private:

	const char*	title;
	int			width;
	int			height;
	uint		flags;

};

#endif // __ModuleWindow_H__