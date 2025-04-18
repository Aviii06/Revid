#pragma once

namespace Revid
{
	class InputHandler
	{
	public:
		InputHandler() = default;
		~InputHandler() = default;

		void Shutdown();
		void HandleInput();

	private:
		double m_lastX = 0;
		double m_lastY = 0;
	};
}