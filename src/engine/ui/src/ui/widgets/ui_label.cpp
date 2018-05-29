#include "widgets/ui_label.h"
#include "halley/text/i18n.h"

using namespace Halley;

UILabel::UILabel(const String& id, TextRenderer style, const LocalisedString& text)
	: UIWidget(id, {})
	, renderer(style)
	, text(text)
{
	updateText();
}

void UILabel::draw(UIPainter& painter) const
{
	if (needsClip) {
		painter.withClip(Rect4f(getPosition(), getPosition() + getMinimumSize())).draw(renderer);
	} else {
		painter.draw(renderer);
	}

	UIWidget::draw(painter);
}

void UILabel::update(Time t, bool moved)
{
	if (text.checkForUpdates()) {
		updateText();
	}
	if (moved) {
		renderer.setPosition(getPosition());
	}
}

void UILabel::updateMinSize()
{
	needsClip = false;
	auto extents = renderer.getExtents();
	if (extents.x > maxWidth) {
		renderer.setText(renderer.split(maxWidth));
		extents = renderer.getExtents();
	}
	if (extents.y > maxHeight) {
		extents.y = maxHeight;
		needsClip = true;
	}
	setMinSize(extents);
}

void UILabel::updateText() {
	renderer.setText(text);
	updateMinSize();
}

void UILabel::setText(const LocalisedString& t)
{
	if (text != t) {
		text = t;
		updateText();
	}
}

void UILabel::setColourOverride(const std::vector<ColourOverride>& overrides)
{
	renderer.setColourOverride(overrides);
}

void UILabel::setMaxWidth(float m)
{
	maxWidth = m;
	updateMinSize();
}

void UILabel::setMaxHeight(float m)
{
	maxHeight = m;
	updateMinSize();
}

void UILabel::setTextRenderer(TextRenderer r)
{
	r.setText(renderer.getText()).setPosition(renderer.getPosition());
	renderer = r;
	updateMinSize();
}

void UILabel::setColour(Colour4f colour)
{
	renderer.setColour(colour);
}

void UILabel::setSelectable(Colour4f normalColour, Colour4f selColour)
{
	setHandle(UIEventType::SetSelected, [=] (const UIEvent& event)
	{
		if (event.getBoolData()) {
			setColour(selColour);
		} else {
			setColour(normalColour);
		}
	});
}
