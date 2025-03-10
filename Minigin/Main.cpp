#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "FPSComponent.h"
#include "RotateComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SpriteComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SpriteComponent>("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>(); 
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	go->SetPosition(80, 40);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>(font);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SpriteComponent>("kirbyR.png");
	go->SetPosition(150, 150);
	scene.Add(go);

	auto& input = dae::InputManager::GetInstance();
	float movementSpeed{ 150.f };

	input.BindKeyboardInput(SDLK_UP, std::make_shared<dae::MoveCommand>(go.get(), 0.0f, -1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_DOWN, std::make_shared<dae::MoveCommand>(go.get(), 0.0f, 1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_LEFT, std::make_shared<dae::MoveCommand>(go.get(), -1.0f, 0.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_RIGHT, std::make_shared<dae::MoveCommand>(go.get(), 1.0f, 0.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_w, std::make_shared<dae::MoveCommand>(go.get(), 0.0f, -1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_s, std::make_shared<dae::MoveCommand>(go.get(), 0.0f, 1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_a, std::make_shared<dae::MoveCommand>(go.get(), -1.0f, 0.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_d, std::make_shared<dae::MoveCommand>(go.get(), 1.0f, 0.0f, movementSpeed));

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::SpriteComponent>("kirbyR.png");
	go->SetPosition(200, 200);
	scene.Add(go);
	movementSpeed *= 2;

	input.BindControllerInput(dae::Gamepad::Button::DPadUp, std::make_shared<dae::MoveCommand>(go.get(), 0.f, -1.f, movementSpeed));
	input.BindControllerInput(dae::Gamepad::Button::DPadDown, std::make_shared<dae::MoveCommand>(go.get(), 0.f, 1.f, movementSpeed));
	input.BindControllerInput(dae::Gamepad::Button::DPadLeft, std::make_shared<dae::MoveCommand>(go.get(), -1.f, 0.f, movementSpeed));
	input.BindControllerInput(dae::Gamepad::Button::DPadRight, std::make_shared<dae::MoveCommand>(go.get(), 1.f, 0.f, movementSpeed));



}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}