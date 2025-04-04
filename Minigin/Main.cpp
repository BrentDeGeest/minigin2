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
#include "ScoreComponent.h"

void CreateBackground(dae::Scene& scene)
{
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
}


void RegisterUIText(
	dae::Scene& scene,
	const std::string& key,
	const std::string& initialText,
	float x, float y,
	std::shared_ptr<dae::GameObject> owner,
	dae::GameEventType eventType)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);

	// Determine initial value based on event type
	int initialValue = 0;
	if (eventType == dae::GameEventType::ScoreChanged)
	{
		auto scoreComp = owner->GetComponent<dae::ScoreComponent>();
		if (scoreComp) initialValue = scoreComp->GetScore();
	}
	else if (eventType == dae::GameEventType::HealthChanged)
	{
		auto healthComp = owner->GetComponent<dae::HealthComponent>();
		if (healthComp) initialValue = healthComp->GetLives();
	}

	// Replace `{value}` in initial text with actual starting value
	std::string formattedText = initialText;
	size_t valuePos = formattedText.find("{value}");
	if (valuePos != std::string::npos) {
		formattedText.replace(valuePos, 7, std::to_string(initialValue));
	}

	// Create UI GameObject and TextComponent
	auto go = std::make_shared<dae::GameObject>();
	auto textComp = go->AddComponent<dae::TextComponent>(formattedText, font);
	go->SetPosition(x, y);
	scene.Add(go);

	// Register UI Element with an event listener
	dae::UIManager::GetInstance().RegisterUIElement(
		key,
		textComp,
		initialText,
		[textComp, owner, eventType, initialText](std::shared_ptr<dae::Event> event)
		{
			auto gameEvent = std::dynamic_pointer_cast<dae::GameEvent>(event);
			if (!gameEvent || gameEvent->GetEntityId() != owner->GetId()) return;
			if (gameEvent->GetType() == eventType) {
				std::string updatedText = initialText;
				size_t valuePos = updatedText.find("{value}");
				if (valuePos != std::string::npos) {
					updatedText.replace(valuePos, 7, std::to_string(gameEvent->GetValue()));
				}
				textComp->SetText(updatedText);
			}
		});
}

void RegisterInputBindings(std::shared_ptr<dae::GameObject> player, std::shared_ptr<dae::GameObject> enemy)
{
	auto& input = dae::InputManager::GetInstance();
	float movementSpeed{ 150.f };

	// Lambda for keyboard movement bindings
	auto bindKeyboardMove = [&input, movementSpeed](SDL_Keycode key, std::shared_ptr<dae::GameObject> obj, float x, float y) {
		input.BindKeyboardInput(key, std::make_shared<dae::MoveCommand>(obj.get(), x, y, movementSpeed), dae::InputType::Held);
		};

	// Lambda for gamepad movement bindings
	auto bindGamepadMove = [&input, movementSpeed](dae::Gamepad::Button button, std::shared_ptr<dae::GameObject> obj, float x, float y) {
		input.BindControllerInput(button, std::make_shared<dae::MoveCommand>(obj.get(), x, y, movementSpeed), dae::InputType::Held);
		};

	// Player - Keyboard Movement Bindings
	bindKeyboardMove(SDLK_UP, player, 0.0f, -1.0f);
	bindKeyboardMove(SDLK_DOWN, player, 0.0f, 1.0f);
	bindKeyboardMove(SDLK_LEFT, player, -1.0f, 0.0f);
	bindKeyboardMove(SDLK_RIGHT, player, 1.0f, 0.0f);
	bindKeyboardMove(SDLK_w, player, 0.0f, -1.0f);
	bindKeyboardMove(SDLK_s, player, 0.0f, 1.0f);
	bindKeyboardMove(SDLK_a, player, -1.0f, 0.0f);
	bindKeyboardMove(SDLK_d, player, 1.0f, 0.0f);

	

	// Player - Score & Damage Inputs
	input.BindKeyboardInput(SDLK_z, std::make_shared<dae::GainScoreCommand>(player.get(), 10), dae::InputType::Pressed);
	input.BindKeyboardInput(SDLK_x, std::make_shared<dae::GainScoreCommand>(player.get(), 100), dae::InputType::Pressed);
	input.BindKeyboardInput(SDLK_c, std::make_shared<dae::TakeDamageCommand>(player.get()), dae::InputType::Pressed);

	input.BindControllerInput(dae::Gamepad::Button::A, std::make_shared<dae::GainScoreCommand>(player.get(), 10), dae::InputType::Pressed);
	input.BindControllerInput(dae::Gamepad::Button::B, std::make_shared<dae::GainScoreCommand>(player.get(), 100), dae::InputType::Pressed);
	input.BindControllerInput(dae::Gamepad::Button::X, std::make_shared<dae::TakeDamageCommand>(player.get()), dae::InputType::Pressed);

	// Enemy - Gamepad Movement Bindings
	bindGamepadMove(dae::Gamepad::Button::DPadUp, enemy, 0.0f, -1.0f);
	bindGamepadMove(dae::Gamepad::Button::DPadDown, enemy, 0.0f, 1.0f);
	bindGamepadMove(dae::Gamepad::Button::DPadLeft, enemy, -1.0f, 0.0f);
	bindGamepadMove(dae::Gamepad::Button::DPadRight, enemy, 1.0f, 0.0f);

	// Enemy - Score & Damage Inputs
	input.BindControllerInput(dae::Gamepad::Button::A, std::make_shared<dae::GainScoreCommand>(enemy.get(), 10), dae::InputType::Pressed);
	input.BindControllerInput(dae::Gamepad::Button::B, std::make_shared<dae::GainScoreCommand>(enemy.get(), 100), dae::InputType::Pressed);
	input.BindControllerInput(dae::Gamepad::Button::X, std::make_shared<dae::TakeDamageCommand>(enemy.get()), dae::InputType::Pressed);
}



void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// Create Background & FPS UI
	CreateBackground(scene);

	// Create tutorial text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Player 1: Arrow Keys or WASD to move, C to inflict damage, Z and X to add score", font);
	go->SetPosition(20, 80);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextComponent>("Player 2: Gamepad DPad to move, X to inflict damage, A and B to add score", font);
	go->SetPosition(20, 100);
	scene.Add(go);

	// Create Players
	auto player = std::make_shared<dae::GameObject>();
	player->AddComponent<dae::SpriteComponent>("kirbyR.png");
	player->SetPosition(200, 200);
	player->AddComponent<dae::HealthComponent>(3, true);
	player->AddComponent<dae::ScoreComponent>();
	scene.Add(player);

	auto enemy = std::make_shared<dae::GameObject>();
	enemy->AddComponent<dae::SpriteComponent>("kirbyR.png");
	enemy->SetPosition(200, 250);
	enemy->AddComponent<dae::HealthComponent>(3, true);
	enemy->AddComponent<dae::ScoreComponent>();
	scene.Add(enemy);

	// Setup UI
	// UI Elements
	RegisterUIText(scene, "Score_" + std::to_string(player->GetId()), "P1 - Score: {value}", 20, 170, player, dae::GameEventType::ScoreChanged);
	RegisterUIText(scene, "Lives_" + std::to_string(player->GetId()), "P1 - Lives: {value}", 20, 200, player, dae::GameEventType::HealthChanged);
	RegisterUIText(scene, "Score_" + std::to_string(enemy->GetId()), "P2 - Score: {value}", 20, 230, enemy, dae::GameEventType::ScoreChanged);
	RegisterUIText(scene, "Lives_" + std::to_string(enemy->GetId()), "P2 - Lives: {value}", 20, 250, enemy, dae::GameEventType::HealthChanged);


	// Setup Input Bindings
	RegisterInputBindings(player, enemy);

	// Register UI Manager as an Event Listener
	dae::EventQueue::GetInstance().AddListener(
		[](std::shared_ptr<dae::Event> event)
		{
			dae::UIManager::GetInstance().OnNotify(event);
		});
	
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}