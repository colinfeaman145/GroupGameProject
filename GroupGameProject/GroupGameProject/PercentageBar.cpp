#include "PercentageBar.hpp"
#include "Renderer.hpp"

PercentageBar::PercentageBar(const PercentageBar& other) {
    current = other.current;
    max = other.max;
    w = other.w;
    h = other.h;
    x = other.x;
    y = other.y;
    fillColor = other.fillColor;
    outlineColor = other.outlineColor;
    offsetX = other.offsetX;
    offsetY = other.offsetY;
}

PercentageBar::PercentageBar(float current, float max, int w, int h, Color fillColor, Color outlineColor)
    : current(current), max(max), w(w), h(h), x(0), y(0)
    , fillColor(fillColor), outlineColor(outlineColor) {
}

void PercentageBar::SetValues(float c, float m) {
    current = c;
    max = m;
}

void PercentageBar::SetPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void PercentageBar::SetOffset(int offX, int offY) {
    offsetX = offX;
    offsetY = offY;
}

void PercentageBar::SetFillColor(Color c) { fillColor = c; }
void PercentageBar::SetOutlineColor(Color c) { outlineColor = c; }

void PercentageBar::Draw(Renderer* renderer) {
    float pct = (max > 0) ? current / max : 0.0f;
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;

    int fillWidth = (int)(w * pct);

    // fill
    if (fillWidth > 0) {
		float sections = max / 20.0f;

        renderer->AddFilledRect(x + offsetX, y + offsetY, fillWidth, h, fillColor, RenderLayer::PERCENTBAR);

		for (int i = 0; i < sections; ++i) {
			float sectionPct = (i + 1) / sections;
			if (sectionPct > pct) break;
			int sectionX = x + offsetX + (int)(w * sectionPct);
			renderer->AddFilledRect(sectionX, y + offsetY, 2, h, { 0, 0, 0, 150 }, RenderLayer::PERCENTBAR);
		}

    }

    // outline
    renderer->AddDrawRect(x + offsetX, y + offsetY, w, h, outlineColor, RenderLayer::PERCENTBAR);
}