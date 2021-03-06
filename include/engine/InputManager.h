#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#define INCLUDE_SDL
#include "engine/SDL_include.h"
#include <unordered_map>

enum class KEYS {
	A_KEY = SDLK_a,
	D_KEY = SDLK_d,
	W_KEY = SDLK_w,
	S_KEY = SDLK_s,
	LEFT_ARROW_KEY = SDLK_LEFT,
	RIGHT_ARROW_KEY = SDLK_RIGHT,
	UP_ARROW_KEY = SDLK_UP,
	DOWN_ARROW_KEY = SDLK_DOWN,
	SPACE_KEY = SDLK_SPACE,
	ESCAPE_KEY = SDLK_ESCAPE,
	LEFT_MOUSE_BUTTON = SDL_BUTTON_LEFT,
	RIGHT_MOUSE_BUTTON = SDL_BUTTON_RIGHT,
};

class InputManager {
	private:
		// * State => Last performed action, Update => Time of the last action.
		// * Layout: Left, Middle, Right, X1, X2
		bool mousePressed[6];
		uint32_t mouseUpdate[6];
		
		std::unordered_map<int, bool> keyPressed;
		std::unordered_map<int, uint32_t> keyUpdate;

		bool quitRequested;

		uint32_t updateCounter;

		int mouseX, mouseY;

	public:
		InputManager();
		~InputManager();

		void Update();
		
		// Check if 'key' is been pressed in the current frame.
		bool KeyPress(KEYS key);
		// Check if 'key' is been released in the current frame. 
		bool KeyRelease(KEYS key);
		// Check if 'key' is pressed.
		bool IsKeyDown(KEYS key);

		int GetMouseX();
		int GetMouseY();

		bool QuitRequested();

		static InputManager& GetInstance();
};

#endif