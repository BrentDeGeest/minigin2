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

	// invisible object orbitingobject will orbit around
	auto centerObject = std::make_shared<dae::GameObject>();
	centerObject->SetPosition(250, 250); // Center position
	scene.Add(centerObject);

	// Create an object that orbits around the center object and will act as a parent
	auto orbitingObject = std::make_shared<dae::GameObject>();
	orbitingObject->AddComponent<dae::SpriteComponent>("KirbyR.png");
	orbitingObject->AddComponent<dae::RotateComponent>(50.f, 180.f, centerObject); 

	scene.Add(orbitingObject);

	// Create an object that orbits around the orbiting object
	auto orbitingObjectChild = std::make_shared<dae::GameObject>();
	orbitingObjectChild->AddComponent<dae::SpriteComponent>("KirbyR.png");
	orbitingObjectChild->AddComponent<dae::RotateComponent>(100.f, 90.f);
	orbitingObjectChild->SetParent(orbitingObject);

	// Testing multiple levels of orbiting
	auto orbitingObject3 = std::make_shared<dae::GameObject>();
	orbitingObject3->AddComponent<dae::SpriteComponent>("KirbyR.png");
	orbitingObject3->AddComponent<dae::RotateComponent>(50.f, -90.f);
	orbitingObject3->SetParent(orbitingObjectChild);

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}