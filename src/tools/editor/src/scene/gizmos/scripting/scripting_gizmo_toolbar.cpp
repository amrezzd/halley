#include "scripting_gizmo_toolbar.h"
#include "scripting_gizmo.h"
using namespace Halley;

ScriptingGizmoToolbar::ScriptingGizmoToolbar(UIFactory& factory, ScriptingBaseGizmo& gizmo)
	: UIWidget("scripting_gizmo_toolbar", {}, UISizer())
	, gizmo(gizmo)
	, factory(factory)
{
	factory.loadUI(*this, "halley/scripting_gizmo_toolbar");
	setInteractWithMouse(true);
}

void ScriptingGizmoToolbar::onMakeUI()
{
	setHandle(UIEventType::ButtonClicked, "addNode", [=] (const UIEvent& event)
	{
		gizmo.addNode();
	});
}

void ScriptingGizmoToolbar::onAddedToRoot(UIRoot& root)
{
	root.setFocus(shared_from_this());
	root.registerKeyPressListener(shared_from_this(), 1);
}

void ScriptingGizmoToolbar::onRemovedFromRoot(UIRoot& root)
{
	root.removeKeyPressListener(*this);
}

