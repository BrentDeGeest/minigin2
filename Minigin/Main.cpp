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
#include "HealthComponent.h"
#include "EventQueue.h"
#include "PlayerDiedEvent.h"
#include "GameEvent.h"
#include "UIManager.h"
#include "GainScoreCommand.h"
#include "TakeDamageCommand.h"

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





	// Player
	auto player = std::make_shared<dae::GameObject>();
	player->AddComponent<dae::SpriteComponent>("kirbyR.png");
	player->SetPosition(150, 150);
	player->AddComponent<dae::HealthComponent>(3, true);
	scene.Add(player);


	auto& input = dae::InputManager::GetInstance();
	float movementSpeed{ 150.f };

	input.BindKeyboardInput(SDLK_UP, std::make_shared<dae::MoveCommand>(player.get(), 0.0f, -1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_DOWN, std::make_shared<dae::MoveCommand>(player.get(), 0.0f, 1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_LEFT, std::make_shared<dae::MoveCommand>(player.get(), -1.0f, 0.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_RIGHT, std::make_shared<dae::MoveCommand>(player.get(), 1.0f, 0.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_w, std::make_shared<dae::MoveCommand>(player.get(), 0.0f, -1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_s, std::make_shared<dae::MoveCommand>(player.get(), 0.0f, 1.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_a, std::make_shared<dae::MoveCommand>(player.get(), -1.0f, 0.0f, movementSpeed));
	input.BindKeyboardInput(SDLK_d, std::make_shared<dae::MoveCommand>(player.get(), 1.0f, 0.0f, movementSpeed));

	// Enemy
	auto enemy = std::make_shared<dae::GameObject>();
	enemy->AddComponent<dae::SpriteComponent>("kirbyR.png");
	enemy->SetPosition(200, 200);
	scene.Add(enemy);
	movementSpeed *= 2;

	input.BindControllerInput(dae::Gamepad::Button::DPadUp, std::make_shared<dae::MoveCommand>(enemy.get(), 0.f, -1.f, movementSpeed));
	input.BindControllerInput(dae::Gamepad::Button::DPadDown, std::make_shared<dae::MoveCommand>(enemy.get(), 0.f, 1.f, movementSpeed));
	input.BindControllerInput(dae::Gamepad::Button::DPadLeft, std::make_shared<dae::MoveCommand>(enemy.get(), -1.f, 0.f, movementSpeed));
	input.BindControllerInput(dae::Gamepad::Button::DPadRight, std::make_shared<dae::MoveCommand>(enemy.get(), 1.f, 0.f, movementSpeed));

	// Create Score Display
	auto scoreDisplay = std::make_shared<dae::GameObject>();
	auto scoreText = scoreDisplay->AddComponent<dae::TextComponent>("Score: 0", font);
	scoreDisplay->SetPosition(20, 100);
	scene.Add(scoreDisplay);
	//dae::UIManager::GetInstance().RegisterUIElement("Player" + std::to_string(player->GetId()) + "_Score", scoreText);
	dae::UIManager::GetInstance().RegisterUIElement("Score_" + std::to_string(player->GetId()), scoreText);

	// Create Lives Display
	auto livesDisplay = std::make_shared<dae::GameObject>();
	auto livesText = livesDisplay->AddComponent<dae::TextComponent>("Lives: 3", font);
	livesDisplay->SetPosition(20, 150);
	scene.Add(livesDisplay);
	//dae::UIManager::GetInstance().RegisterUIElement("Player" + std::to_string(player->GetId()) + "_Lives", livesText);
	dae::UIManager::GetInstance().RegisterUIElement("Lives_" + std::to_string(player->GetId()), livesText);

	// Register UIManager as Event Listener
	dae::EventQueue::GetInstance().AddListener(
		[](std::shared_ptr<dae::Event> event)
		{
			dae::UIManager::GetInstance().OnNotify(event);
		});


	// Test UI - Press A to gain score, B to lose a life
	input.BindControllerInput(dae::Gamepad::Button::A, std::make_shared<dae::GainScoreCommand>(player.get(), 100));
	input.BindControllerInput(dae::Gamepad::Button::B, std::make_shared<dae::TakeDamageCommand>(player.get()));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}