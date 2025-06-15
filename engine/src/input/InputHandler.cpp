#include <revid_engine/input/InputHandler.h>
#include <revid_engine/ServiceLocater.h>

#include <GLFW/glfw3.h>

void Revid::InputHandler::HandleInput()
{

    	// TODO: Abstract this out to a InputHandler
		if (ServiceLocator::GetWindow()->IsKeyPressed('W'))
		{
			ServiceLocator::GetCamera()->MoveForward();
		}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('S'))
    	{
			ServiceLocator::GetCamera()->MoveBackward();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('A'))
    	{
    		ServiceLocator::GetCamera()->MoveLeft();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('D'))
    	{
    		ServiceLocator::GetCamera()->MoveRight();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('L'))
    	{
    		ServiceLocator::GetCamera()->MoveUp();
    	}
    	if (ServiceLocator::GetWindow()->IsKeyPressed('H'))
    	{
    		ServiceLocator::GetCamera()->MoveDown();
    	}

    	// Process Mouse Movements
		const Maths::Vec2 pos = ServiceLocator::GetWindow()->GetMousePos();
    	if (ServiceLocator::GetWindow()->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    	{
    		double xoffset = m_lastX - pos.x;
    		double yoffset = m_lastY - pos.y;
			ServiceLocator::GetCamera()->ProcessMouseMovement(xoffset, yoffset, true);

			m_lastX = pos.x;
			m_lastY = pos.y;
    	}
		else
		{
			m_lastX = pos.x;
			m_lastY = pos.y;
		}
}