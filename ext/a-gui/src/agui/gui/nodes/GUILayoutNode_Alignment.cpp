#include <agui/gui/nodes/GUILayoutNode_Alignment.h>

#include <string>

#include <agui/agui.h>
#include <agui/utilities/Enum.h>

using std::string;

using agui::gui::nodes::GUILayoutNode_Alignment;
using agui::utilities::Enum;

GUILayoutNode_Alignment::GUILayoutNode_Alignment(const string& name, int ordinal)
	: Enum(name, ordinal)
{
}

GUILayoutNode_Alignment* agui::gui::nodes::GUILayoutNode_Alignment::VERTICAL = new GUILayoutNode_Alignment("VERTICAL", 0);
GUILayoutNode_Alignment* agui::gui::nodes::GUILayoutNode_Alignment::HORIZONTAL = new GUILayoutNode_Alignment("HORIZONTAL", 1);
GUILayoutNode_Alignment* agui::gui::nodes::GUILayoutNode_Alignment::NONE = new GUILayoutNode_Alignment("NONE", 2);

GUILayoutNode_Alignment* GUILayoutNode_Alignment::valueOf(const string& name)
{
	if (HORIZONTAL->getName() == name) return HORIZONTAL;
	if (NONE->getName() == name) return NONE;
	if (VERTICAL->getName() == name) return VERTICAL;
	// TODO: throw exception here maybe
	return nullptr;
}
