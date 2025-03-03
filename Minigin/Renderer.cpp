#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_plot.h>
#include <chrono>
#include <vector>
#include <algorithm>



// Benchmark Data
int numSamples = 10;  // Default number of samples
std::vector<float> step_sizes_ex1;
std::vector<float> times;

void RunExercise1Benchmark() {
    step_sizes_ex1.clear();
	times.clear();

	constexpr size_t BUFFER_SIZE = 1 << 26;
	int* arr = new int[BUFFER_SIZE]();  // Allocate large buffer

	for (int step = 1; step <= 1024; step *= 2) {
		std::vector<long long> timeResults;

		for (int run = 0; run < numSamples; ++run) {
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t i = 0; i < BUFFER_SIZE; i += static_cast<size_t>(step)) {
				arr[i] *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			timeResults.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
		}

		// Remove outliers safely
		if (timeResults.size() > 2) {
			std::sort(timeResults.begin(), timeResults.end());
			timeResults.erase(timeResults.begin());  // Remove smallest
			timeResults.pop_back();                 // Remove largest
		}

		// Compute average time
		long long avg_time = 0;
		for (size_t i = 0; i < timeResults.size(); ++i) {
			avg_time += timeResults[i];
		}
		if (!timeResults.empty()) {
			avg_time /= static_cast<long long>(timeResults.size());
		}

        step_sizes_ex1.push_back(static_cast<float>(step));
		times.push_back(static_cast<float>(avg_time));
	}

	delete[] arr;
}

void ShowExercise1Window() {
	ImGui::Begin("Exercise 1", nullptr, ImGuiWindowFlags_None);

	ImGui::Text("Adjust Sample Count:");
	ImGui::SameLine();
	//if (ImGui::Button("-")) numSamples = std::max(1, numSamples - 1);
	ImGui::SameLine();
    ImGui::PushItemWidth(100);
	ImGui::InputInt("##samples", &numSamples);
	ImGui::PopItemWidth();
	//ImGui::SameLine();

	//if (ImGui::Button("+")) numSamples++;

	if (ImGui::Button("Trash the Cache")) {
		RunExercise1Benchmark();
	}

	if (!step_sizes_ex1.empty()) {
		ImGui::Text("Benchmark Results:");
		ImGui::PlotLines("Performance", times.data(), static_cast<int>(times.size()), 0, nullptr, 0.0f,
			*std::max_element(times.begin(), times.end()), ImVec2(200, 100));
	}

	ImGui::End();
}

// Benchmark Data
int numSamplesExercise2 = 10; // Default samples
std::vector<float> step_sizes_ex2;
std::vector<float> times_gameobject3d;
std::vector<float> times_gameobject3dalt;
bool hasRunGameObject3D = false;
bool hasRunGameObject3DAlt = false;

struct Transform {
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class GameObject3D {
public:
    Transform transform;
    int ID;
};

class GameObject3DAlt {
public:
    static constexpr size_t BUFFER_SIZE = 1 << 26;
    Transform* transforms;
    int* IDs;

    GameObject3DAlt() {
        transforms = new Transform[BUFFER_SIZE]();
        IDs = new int[BUFFER_SIZE]();
    }

    ~GameObject3DAlt() {
        delete[] transforms;
        delete[] IDs;
    }
};

// **Benchmark Function for GameObject3D**
void RunGameObject3DBenchmark() {
    step_sizes_ex2.clear();
    times_gameobject3d.clear();
    hasRunGameObject3D = true;

    constexpr size_t BUFFER_SIZE = 1 << 26;
    GameObject3D* objects = new GameObject3D[BUFFER_SIZE]();

    for (int step = 1; step <= 1024; step *= 2) {
        std::vector<long long> timeResults;

        for (int run = 0; run < numSamplesExercise2; ++run) {
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < BUFFER_SIZE; i += static_cast<size_t>(step)) {
                objects[i].ID *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            timeResults.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        }

        std::sort(timeResults.begin(), timeResults.end());
        timeResults.erase(timeResults.begin());
        timeResults.pop_back();

        long long avg_time = 0;
        for (size_t i = 0; i < timeResults.size(); ++i) {
            avg_time += timeResults[i];
        }
        avg_time /= static_cast<long long>(timeResults.size());

        step_sizes_ex2.push_back(static_cast<float>(step));
        times_gameobject3d.push_back(static_cast<float>(avg_time));
    }

    delete[] objects;
}

// **Benchmark Function for GameObject3DAlt**
void RunGameObject3DAltBenchmark() {
    step_sizes_ex2.clear();
    times_gameobject3dalt.clear();
    hasRunGameObject3DAlt = true;

    constexpr size_t BUFFER_SIZE = 1 << 26;
    GameObject3DAlt gameObjects;

    for (int step = 1; step <= 1024; step *= 2) {
        std::vector<long long> timeResults;

        for (int run = 0; run < numSamplesExercise2; ++run) {
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < BUFFER_SIZE; i += static_cast<size_t>(step)) {
                gameObjects.IDs[i] *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            timeResults.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        }

        std::sort(timeResults.begin(), timeResults.end());
        timeResults.erase(timeResults.begin());
        timeResults.pop_back();

        long long avg_time = 0;
        for (size_t i = 0; i < timeResults.size(); ++i) {
            avg_time += timeResults[i];
        }
        avg_time /= static_cast<long long>(timeResults.size());

        step_sizes_ex2.push_back(static_cast<float>(step));
        times_gameobject3dalt.push_back(static_cast<float>(avg_time));
    }
}

// **Exercise 2 UI**
void ShowExercise2Window() {
    ImGui::Begin("Exercise 2");

    // Samples Input Box
    ImGui::Text("Adjust Sample Count:");
    ImGui::SameLine();
   // if (ImGui::Button("-")) numSamplesExercise2 = std::max(1, numSamplesExercise2 - 1);
    ImGui::SameLine();
	ImGui::PushItemWidth(100);
    ImGui::InputInt("##samplesExercise2", &numSamplesExercise2);
	ImGui::PopItemWidth();
   // ImGui::SameLine();
    //if (ImGui::Button("+")) numSamplesExercise2++;

    // Run Benchmark for GameObject3D
    if (ImGui::Button("Thrash the cache with GameObject3D")) {
        RunGameObject3DBenchmark();
    }

    // Show Graph for GameObject3D
    if (hasRunGameObject3D && !times_gameobject3d.empty()) {
        ImGui::Text("GameObject3D Results:");
        ImGui::PlotLines("GameObject3D", times_gameobject3d.data(), static_cast<int>(times_gameobject3d.size()), 0, nullptr, 0.0f,
            *std::max_element(times_gameobject3d.begin(), times_gameobject3d.end()), ImVec2(200, 100));
    }

    ImGui::Spacing();

    // Run Benchmark for GameObject3DAlt
    if (ImGui::Button("Thrash the cache with GameObject3DAlt")) {
        RunGameObject3DAltBenchmark();
    }

    // Show Graph for GameObject3DAlt
    if (hasRunGameObject3DAlt && !times_gameobject3dalt.empty()) {
        ImGui::Text("GameObject3DAlt Results:");
        ImGui::PlotLines("GameObject3DAlt", times_gameobject3dalt.data(), static_cast<int>(times_gameobject3dalt.size()), 0, nullptr, 0.0f,
            *std::max_element(times_gameobject3dalt.begin(), times_gameobject3dalt.end()), ImVec2(200, 100));
    }

    ImGui::End();
}
int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
	ShowExercise1Window();
	ShowExercise2Window();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
