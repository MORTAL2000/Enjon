#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Physics/AABB.h"

#include "GUI/Signal.h"
#include "GUI/Property.h"

#include "Graphics/Animations.h"

#include <vector>
#include <string>

namespace Enjon { namespace GUI {

	enum ButtonState { INACTIVE, ACTIVE };
	enum HoveredState { OFF_HOVER, ON_HOVER };
	enum GUIType { BUTTON, TEXTBOX };

	// GUI Element
	struct GUIElementBase
	{
		virtual void Init() = 0;

		GUIElementBase* Parent;
		EM::Vec2 Position;
		EP::AABB AABB;
		GUIType Type;
	};

	template <typename T>
	struct GUIElement : public GUIElementBase
	{
		void Init()
		{
			static_cast<T*>(this)->Init();
		}
	};

	// Button
	struct GUIButton : GUIElement<GUIButton>
	{
		void Init()
		{
			std::cout << "Initialized Button..." << std::endl;
		}

		std::vector<ImageFrame> Frames;   // Could totally put this in a resource manager of some sort
		ButtonState State;
		HoveredState HoverState;
		EGUI::Signal<> on_click;
		EGUI::Signal<> on_hover;
		EGUI::Signal<> off_hover;
	};

	// TextBox
	struct GUITextBox : GUIElement<GUITextBox>
	{
		void Init()
		{}

		ButtonState State;
		HoveredState HoverState;
		std::string Text;
		int32_t CursorIndex;
		EGUI::Signal<> on_hover;
		EGUI::Signal<> off_hover;
		EGUI::Signal<> on_click;
		EGUI::Signal<std::string> on_keyboard;
		EGUI::Signal<> on_backspace;
	};

	// Group is responsible for holding other gui elements and then transforming them together
	struct GUIGroup : GUIElement<GUIGroup>
	{
		void Init()
		{
			std::cout << "Yeah..." << std::endl;
		}

		// Vector of children
		std::vector<GUIElementBase*> Children;
	};

	inline GUIGroup* AddToGroup(GUIGroup* Group, GUIElementBase* Element)
	{
		// Push back into group's children
		Group->Children.push_back(Element);

		// Set Group as parent of child
		Element->Parent = Group;

		return Group;
	}

}}


#endif