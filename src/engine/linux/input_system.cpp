#include "engine/input_system.h"
#include "engine/associative_array.h"
#include "engine/profiler.h"
#include "engine/string.h"
#include "engine/vec.h"


namespace Lumix
{
	struct InputSystemImpl : public InputSystem
	{
		InputSystemImpl(IAllocator& allocator) : m_allocator(allocator)
			, m_mouse_rel_pos(0, 0)
			, m_injected_mouse_rel_pos(0, 0)
		{}


		bool create() { return true; }


		void enable(bool enabled) override {}


		void update(float) override
		{
			PROFILE_FUNCTION();

			m_mouse_rel_pos = m_injected_mouse_rel_pos;
			m_injected_mouse_rel_pos = { 0, 0 };
		}


		float getActionValue(uint32 action) override { return 0; }


		void injectMouseXMove(float value) override
		{
			m_injected_mouse_rel_pos.x = value;
		}


		void injectMouseYMove(float value) override
		{
			m_injected_mouse_rel_pos.y = value;
		}


		float getMouseXMove() const override { return m_mouse_rel_pos.x; }
		float getMouseYMove() const override { return m_mouse_rel_pos.y; }


		void addAction(uint32 action, InputType type, int key, int controller_id) override {}

		IAllocator& m_allocator;
		Vec2 m_injected_mouse_rel_pos;
		Vec2 m_mouse_rel_pos;
	};


	InputSystem* InputSystem::create(IAllocator& allocator)
	{
		auto* system = LUMIX_NEW(allocator, InputSystemImpl)(allocator);
		if (!system->create())
		{
			LUMIX_DELETE(allocator, system);
			return nullptr;
		}
		return system;
	}


	void InputSystem::destroy(InputSystem& system)
	{
		auto* impl = static_cast<InputSystemImpl*>(&system);
		LUMIX_DELETE(impl->m_allocator, impl);
	}


} // namespace Lumix