#pragma once
// #include "../pch.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define USE_GAME_MODULES                                                       \
  using Bored::renderer;                                                       \
  using Bored::audio;                                                          \
  using Bored::input;                                                          \
  using Bored::actorManager;                                                   \
  using Bored::gamemode;                                                       \
  using Bored::addons;
//extern std::shared_ptr<Bored::Renderer> Bored::renderer; \
  //extern std::shared_ptr<Bored::Audio> Bored::audio;     \
  //extern std::shared_ptr<Bored::Input> Bored::input;      \
  //extern std::shared_ptr<Bored::ActorManager> Bored::actorManager; \
  //extern std::shared_ptr<Bored::Gamemode> Bored::gamemode;      \
  //extern std::vector<std::shared_ptr<Bored::Module>> Bored::addons; \

#include "../pch.h"
#include "Bases.h"
#include "Render/Utils.h"
#include "Camera.h"
// #include "ActorManager.h"
// #include "Render/Utils.h"

// Just define the general interfaces here
namespace Bored
{
	struct Transform
	{
		glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

		[[nodiscard]] const glm::mat4 GetMat() const;
	};

	/**
	 * @brief Holds the logics of the game
	 */
	class Gamemode : public Module {};

	/**
	 * @brief Provide chrono functionalities
	 */
	class Timer : public Module
	{
		std::chrono::system_clock::time_point last_frame;

	public:
		virtual void OnSetup() override
		{
			last_frame = std::chrono::system_clock::now();
		}

		double GetDt() const
		{
			return std::chrono::duration<double>(std::chrono::system_clock::now() -
				last_frame)
				.count();
		}

		virtual bool OnTick(double) override
		{
			last_frame = std::chrono::system_clock::now();
			return false;
		}
	};

	class Actor;
	struct IDToPtr
	{
		std::shared_ptr<Actor> ptr;
		IDToPtr(const IDToPtr& o) { ptr = o.ptr; }
		IDToPtr(std::shared_ptr<Actor> p) : ptr(p) {}
		operator std::shared_ptr<Actor>() { return ptr; }
	};
	/**
	 * @brief A special data structure to manage actors effectively
	 */
	class ActorManager : public Module
	{
	public:
		entt::registry actor_registry;
		/**
		 * @brief Create an actor or subtype of actor
		 * @tparam T Actor or subtype of Actor
		 * @tparam ...Args
		 * @tparam
		 * @param ...args params to create actor
		 * @return shared pointer to the created actor
		 */
		template <typename T, class... Args,
			std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
		std::shared_ptr<T> Create(Args &&...args)
		{
			auto id = actor_registry.create();
			std::shared_ptr<T> a = std::make_shared<T>(args...);
			a->id = id;
			actor_registry.emplace<Transform>(id);
			actor_registry.emplace<IDToPtr>(id, a);
			return a;
		}

		/**
		 * @brief Get components of actors with those components
		 * @tparam ...Comps
		 * @return entt::view of those components
		 */
		template <typename... Comps>
		entt::basic_view<entt::entity, entt::exclude_t<>, Comps...> Get()
		{
			return actor_registry.view<Comps...>();
		}

		template <typename... Comps>
		decltype(auto) Get(entt::entity id)
		{
			return actor_registry.get_or_emplace<Comps...>(id);
		}

		template <typename T, class... Args>
		T& AddComponent(entt::entity id, Args &&...args)
		{
			if (!actor_registry.valid(id))
			{
				std::cout << "[Warning]: Adding component " << typeid(T).name()
					<< " for invalid actor: " << (int)id << std::endl;
			}

			if (actor_registry.has<T>(id))
			{
				std::cout << "[Warning]: Component '" << typeid(T).name()
					<< "' already exist for actor " << (uint64_t)id
					<< ". Getting the existed component instead!" << std::endl;

				return actor_registry.get<T>(id);
			}

			return actor_registry.emplace_or_replace<T>(id, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent(entt::entity id)
		{
			if (typeid(T) == typeid(Transform))
			{
				std::cout << "[Warning]: Cannot remove Transform component." << std::endl;
				return;
			}

			if (!actor_registry.has<T>(id))
			{
				std::cout << "[Warning]: Actor " << (uint64_t)id << " does not have Component '"
					<< typeid(T).name() << "'. Not removing any component!" << std::endl;
				return;
			}

			actor_registry.remove<T>(id);
			return;
		}

		void Delete(entt::entity id);

		// Inherited via Module
		virtual void OnSetup() override {};
		virtual bool OnTick(double dt) override;
		virtual void OnImGui() override;
	};

	// Output classes - depends on the device. Implement these for each platform.
	class Renderer : public Module
	{
	public:
		unsigned int width = 1200, height = 800;
	protected:
		std::shared_ptr<Actor> active_cam;
		std::unique_ptr<Projector> projector =
			//std::make_unique<OrthoProjector>(-1.0f, 1.0f, -1.0f, 1.0f);
			std::make_unique<PerspProjector>(width, height);
		std::vector<std::shared_ptr<Actor>> light_srcs;

		[[nodiscard]] const glm::mat4& GetProjMat() const;
	public:
		void UseCamera(std::shared_ptr<Actor> cam);
	};
	class Audio : public Module {};
	class Input : public Module {};

	/* ===================== VARIABLES ==================== */
	extern std::string title;
	extern std::shared_ptr<Renderer> renderer;
	extern std::shared_ptr<Audio> audio;
	extern std::shared_ptr<Input> input;

	extern std::shared_ptr<ActorManager> actorManager;
	extern std::shared_ptr<Gamemode> gamemode;
	extern std::shared_ptr<Timer> timer;

	extern std::vector<std::shared_ptr<Module>> addons;

	/* ====================== FUNCTIONNS =====================*/
	/**
	 * @brief Run the game
	 */
	void Run();

	/**
	 * @brief Stop the game. Call the OnStop method of all modules.
	 */
	void Stop();

	/* ===================== Others ==========================*/

	/**
	 * @brief Component-based entity
	 */
	class Actor : public Tickable
	{
		friend class ActorManager;

	private:

	public:
		entt::entity id;
		Actor() : id(entt::null) {}
		Actor(const entt::entity& id_) : id(id_) {}
		~Actor() {};
		operator entt::entity() const { return id; }
		virtual void OnImGui() {};

		inline const bool IsValid() const { return id != entt::null; }

		template <typename T, class... Args> T& AddComponent(Args &&...args)
		{
			return Bored::actorManager->AddComponent<T>(id,
				std::forward<Args>(args)...);
		}

		template <class... Comps>
		inline decltype(auto) Get() const { return Bored::actorManager->Get<Comps...>(id); }
	};

} // namespace Bored
